/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */

#include <asm/cacheflush.h>
#include <linux/dma-map-ops.h>
#include "securec.h"
#include "smr_common.h"
#include "allocator.h"

struct cma_zone {
    struct device pdev;
    char name[NAME_LEN_MAX];
    unsigned long gfp;
    unsigned long phys_start;
    unsigned long nbytes;
    unsigned int alloc_type;
    unsigned long block_align;
};

long long max_malloc_size = 0x40000000UL;

#ifndef CONFIG_64BIT
static int dma_update_pte(pte_t *pte, pgtable_t token, unsigned long addr, void *data)
{
    struct page *page = virt_to_page(addr);

    pgprot_t prot = *(pgprot_t*)data;

    ot_unused(token);
    set_pte_ext(pte, mk_pte(page, prot), 0);
    return 0;
}

void dma_clear_buffer(struct page *page, size_t size)
{
    ot_unused(page);
    ot_unused(size);
    return;
}

extern void bsp_flush_tlb_kernel_range(unsigned long start, unsigned long end);
static void dma_remap(struct page *page, size_t size, pgprot_t prot)
{
    unsigned long start = (unsigned long)page_address(page);
    unsigned end = start + size;

    apply_to_page_range(&init_mm, start, size, dma_update_pte, &prot);
    dsb();
    bsp_flush_tlb_kernel_range(start, end);
}
#endif /* CONFIG_64BIT */

static ot_mmb_t *allocator_mmb_alloc(const ot_smr_alloc_para_in *para_in)
{
    ot_smr_t *smr = NULL;
    ot_mmb_t *mmb = NULL;
    struct page *page = NULL;
    ot_smr_t *fixed_smr = NULL;

    unsigned long order, fixed_start;
    size_t count;

    const char *smr_name = para_in->smr_name;
    unsigned long size = para_in->size;
    unsigned long gfp = para_in->gfp;

    if (size == 0 || size > max_malloc_size) {
        return NULL;
    }
    /* aligned to SMR_GRAIN */
    size = smr_grain_align(size);
    order = get_order(size);
    count = size >> PAGE_SHIFT;

    smr_debug_info("anonymous=%s, size=%luKB", smr_name, size / SZ_1K);

    osal_list_for_each_entry(smr, ot_smr_get_smr_list(), list) {
        if (smr_default_filter(smr, gfp, smr_name) != 0) {
            continue;
        }
        page = dma_alloc_from_contiguous(smr->cma_dev, count, order, 0);
        if (page == NULL) {
            break;
        }
        fixed_smr = smr;
        fixed_start = page_to_phys(page);
        break;
    }

    if (fixed_smr == NULL) {
        return NULL;
    }

    if (page != NULL) {
#ifndef CONFIG_64BIT
        dma_clear_buffer(page, size);
#else
        (void)memset_s(page_address(page), size, 0, size);
        __flush_dcache_area(page_address(page), size);
#endif
    }

    mmb = mmb_real_alloc(para_in, fixed_smr, fixed_start, size);
    if (mmb == NULL) {
        dma_release_from_contiguous(fixed_smr->cma_dev, page, count);
    }
    return mmb;
}

static void allocator_mmb_free(ot_mmb_t *mmb)
{
    size_t count = mmb->length >> PAGE_SHIFT;
    struct page *page = phys_to_page(mmb->phys_addr);
    ot_smr_t *smr = mmb->zone;

    if (mmb->flags & OT_MMB_MAP2KERN_CACHED) {
        osal_dcache_region_wb((void*)mmb->kvirt, mmb->phys_addr, mmb->length);
    }

    dma_release_from_contiguous(smr->cma_dev, page, count);

    osal_list_del(&mmb->list);
    osal_kfree(mmb);
}

static void *cma_map_into_vmalloc_space(unsigned long len, struct page *phys_page, pgprot_t prot)
{
    unsigned int i;
    struct page **pages = NULL;
    unsigned int pagesnr = len / PAGE_SIZE;
    struct page *tmp = phys_page;
    unsigned int array_size = sizeof(struct page *) * pagesnr;
    void *kvirt = NULL;

    if (array_size == 0) {
        smr_err_trace("array_size is 0.\n");
        return NULL;
    }

    /*
     * Noted: mmb->length would be very large in some cases(for example:
     * more than one Giga Bytes). and array_size would be very large as
     * well. So, don't use kmalloc here.
     */
    pages = osal_vmalloc(array_size);
    if (pages == NULL) {
        smr_err_trace("ptr array(0x%x) vmalloc failed.\n", array_size);
        return NULL;
    }

    for (i = 0; i < pagesnr; i++) {
        *(pages + i) = tmp;
        tmp++;
    }

    kvirt = vmap(pages, pagesnr, VM_MAP, prot);
    osal_vfree(pages);
    pages = NULL;
    return kvirt;
}

static void *allocator_mmb_map2kern(ot_mmb_t *mmb, int cached)
{
    pgprot_t prot;
    struct page *phys_page = phys_to_page(mmb->phys_addr);

    if (mmb->flags & OT_MMB_MAP2KERN) {
        if ((!!cached * OT_MMB_MAP2KERN_CACHED) != (mmb->flags & OT_MMB_MAP2KERN_CACHED)) {
            smr_err_trace("mmb<%s> has been kernel-mapped %s, can not be re-mapped as %s.",
                mmb->name, (mmb->flags & OT_MMB_MAP2KERN_CACHED) ? "cached" : "non-cached",
                (cached) ? "cached" : "non-cached");
            return NULL;
        }
        mmb->map_ref++;
        return mmb->kvirt;
    }

#ifdef CONFIG_64BIT
    if (cached) {
        mmb->flags |= OT_MMB_MAP2KERN_CACHED;
        prot = PAGE_KERNEL;
    } else {
        mmb->flags &= ~OT_MMB_MAP2KERN_CACHED;
        prot = __pgprot(PROT_NORMAL_NC);
    }
#else
    if (cached) {
        mmb->flags |= OT_MMB_MAP2KERN_CACHED;
        prot = pgprot_kernel;
    } else {
        mmb->flags &= ~OT_MMB_MAP2KERN_CACHED;
        prot = pgprot_noncached(pgprot_kernel);
    }
    dma_remap(phys_page, mmb->length, prot);
#endif
    /* Map into vmalloc space */
    mmb->kvirt = cma_map_into_vmalloc_space(mmb->length, phys_page, prot);
    if (mmb->kvirt == NULL) {
        /* you should never get here. */
        smr_err_trace("mmb[0x%lx, 0x%lx] map to kernel failed\n", mmb->phys_addr, mmb->length);
        return NULL;
    }

    mmb->flags |= OT_MMB_MAP2KERN;
    mmb->map_ref++;
    return mmb->kvirt;
}

static int allocator_mmb_unmap(ot_mmb_t *mmb)
{
    int ref;

    if (mmb->flags & OT_MMB_MAP2KERN_CACHED) {
        osal_dcache_region_wb((void *)mmb->kvirt, mmb->phys_addr, mmb->length);
    }

    if (mmb->flags & OT_MMB_MAP2KERN) {
        ref = --mmb->map_ref;
        if (mmb->map_ref != 0) {
            return ref;
        }
    }

    /* unmap from vmalloc space. */
    {
        vunmap(mmb->kvirt);
    }
    mmb->kvirt = NULL;
    mmb->flags &= ~OT_MMB_MAP2KERN;
    mmb->flags &= ~OT_MMB_MAP2KERN_CACHED;

    if ((mmb->flags & OT_MMB_RELEASED) && (mmb->phy_ref == 0)) {
        allocator_mmb_free(mmb);
    }
    return 0;
}

static void *allocator_mmf_map(phys_addr_t phys, int len, int cache)
{
    pgprot_t prot;
    struct page *phys_page = phys_to_page(phys);

#ifdef CONFIG_64BIT
    if (cache) {
        prot = PAGE_KERNEL;
    } else {
        prot = __pgprot(PROT_NORMAL_NC);
    }
#else
    if (cache) {
        prot = pgprot_kernel;
    } else {
        prot = pgprot_noncached(pgprot_kernel);
    }
#endif

    /* Map into vmalloc space. */
    return cma_map_into_vmalloc_space(len, phys_page, prot);
}

/*
 * this function is added for some extra-requirements from
 * some customers, and the implementation is not strictly
 * in accordance with our early design.
 */
static void allocator_mmf_unmap(void *virt)
{
    unsigned long vaddr = (unsigned long)(uintptr_t)virt;
    if ((vaddr >= VMALLOC_START) && (vaddr < VMALLOC_END)) {
        vunmap(virt);
    }
}

static int allocator_init_parse_args(ot_smr_t **zone, int argc, char **argv, struct cma_zone *cma_area)
{
    ot_smr_t *smr_zone = NULL;

    if (argc == 4) { /* 4:Number of parameters */
        smr_zone = ot_smr_create("null", 0, 0, 0);
    } else {
        smr_err_trace("Input parameter num incorrect!\n");
        return -1;
    }

    if (smr_zone == NULL) {
        return -1;
    }

    if (strncpy_s(smr_zone->name, OT_SMR_NAME_LEN, argv[0], OT_SMR_NAME_LEN - 1) != EOK) {
        smr_err_trace("strncpy_s failed!\n");
        ot_smr_destroy(smr_zone);
        return -1;
    }

    smr_printk(KERN_INFO "cmz zone gfp 0x%lx, phys 0x%lx, nbytes 0x%lx\n",
        cma_area->gfp, cma_area->phys_start, cma_area->nbytes);
    smr_zone->gfp        = cma_area->gfp;
    smr_zone->phys_start = cma_area->phys_start;
    smr_zone->nbytes     = cma_area->nbytes;
    smr_zone->cma_dev    = &cma_area->pdev;
    if (smr_zone->nbytes > max_malloc_size) {
        max_malloc_size = smr_zone->nbytes;
    }
    *zone = smr_zone;
    return 0;
}

static int allocator_init(char *s)
{
#ifdef CONFIG_CMA
    ot_smr_t *zone = NULL;
    char *line = NULL;
    struct cma_zone *cma_area = NULL;
    int ret;

    while ((line = strsep(&s, ":")) != NULL) {
        int i;
        char *argv[SMR_PARAM_NUM];
        extern struct cma_zone *get_cma_zone(const char *name);
        /*
         * We got 4 args in "line", formatted as
         * "argv[0],argv[1],argv[2],argv[3]".
         * eg: "<smr_name>,<gfp>,<phys_start>,<size>"
         * For more convenient, "hard code" are used such as "arg[0]", i.e.
         */
        for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;) {
            if (++i == ARRAY_SIZE(argv)) {
                break;
            }
        }

        cma_area = get_cma_zone(argv[0]);
        if (cma_area == NULL) {
            smr_err_trace("can't get cma zone info:%s\n", argv[0]);
            continue;
        }

        ret = allocator_init_parse_args(&zone, i, argv, cma_area);
        if (ret != 0) {
            continue;
        }

        if (ot_smr_register(zone)) {
            smr_warn_trace("Add SMR failed: " OT_SMR_FMT_S "\n", ot_smr_fmt_arg(zone));
            ot_smr_destroy(zone);
        }

        zone = NULL;
    }
#endif
    return 0;
}

int cma_allocator_setopt(struct smr_allocator *allocator)
{
    if (allocator == NULL) {
        return -1;
    }
    allocator->init         = allocator_init;
    allocator->mmb_alloc    = allocator_mmb_alloc;
    allocator->mmb_map2kern = allocator_mmb_map2kern;
    allocator->mmb_unmap    = allocator_mmb_unmap;
    allocator->mmb_free     = allocator_mmb_free;
    allocator->mmf_map      = allocator_mmf_map;
    allocator->mmf_unmap    = allocator_mmf_unmap;
    return 0;
}
