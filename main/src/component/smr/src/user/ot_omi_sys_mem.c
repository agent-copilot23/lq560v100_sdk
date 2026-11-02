/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */

#include "ot_omi_sys_mem.h"
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "securec.h"
#include "ot_eis_type.h"
#include "smr.h"
#include "ot_common_sys_mem.h"
#include "ot_omi_sys_mem_inner.h"
#include "omk_mem.h"

static ot_s32 g_mem_fd = -1;        /* mem fd for non-cache */
static ot_s32 g_mem_cache_fd = -1;  /* mem fd for cache */

static pthread_mutex_t g_mem_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t g_mem_fd_mutex = PTHREAD_MUTEX_INITIALIZER;

#define OT_PAGE_SHIFT   12
#define OT_PAGE_SIZE    (1UL << OT_PAGE_SHIFT)
#define OT_PAGE_MASK    (~(OT_PAGE_SIZE - 1))

#define mem_mutex_lock()        (void)pthread_mutex_lock(&g_mem_mutex)
#define mem_mutex_unlock()      (void)pthread_mutex_unlock(&g_mem_mutex)
#define mem_fd_mutex_lock()     (void)pthread_mutex_lock(&g_mem_fd_mutex)
#define mem_fd_mutex_unlock()   (void)pthread_mutex_unlock(&g_mem_fd_mutex)

#define mem_check_null_ptr_return(ptr)                          \
    do {                                                        \
        if ((ptr) == NULL) {                                    \
            printf("func: %s null pointer!\n", __FUNCTION__);   \
            return OT_ERR_MEM_NULL_PTR;                         \
        }                                                       \
    } while (0)

#define mem_check_smr_userdev_open_return()     \
    do {                                        \
        ot_s32 ret_;                            \
        ret_ = mem_check_smr_userdev_open();    \
        if (ret_ != OT_SUCCESS) {               \
            return ret_;                        \
        }                                       \
    } while (0)

static ot_s32 mem_check_smr_userdev_open(ot_void)
{
    mem_fd_mutex_lock();
    if (g_mem_fd < 0) {
        g_mem_fd = open("/dev/smr_userdev", O_RDWR | O_SYNC);
        if (g_mem_fd < 0) {
            mem_fd_mutex_unlock();
            perror("open smr_userdev");
            return OT_ERR_MEM_NOT_READY;
        }
    }
    mem_fd_mutex_unlock();
    return OT_SUCCESS;
}

static ot_s32 mem_check_smr_userdev_cache_open(ot_void)
{
    mem_fd_mutex_lock();
    if (g_mem_cache_fd < 0) {
        g_mem_cache_fd = open("/dev/smr_userdev", O_RDWR);
        if (g_mem_cache_fd < 0) {
            mem_fd_mutex_unlock();
            perror("open smr_userdev");
            return OT_ERR_MEM_NOT_READY;
        }
    }
    mem_fd_mutex_unlock();
    return OT_SUCCESS;
}

ot_void *ot_omi_sys_mmap(ot_phys_addr phys_addr, ot_u32 size)
{
    ot_u32 diff;
    ot_phys_addr page_phys;
    ot_u8 *page_addr = OT_NULL;
    ot_ulong page_size;

    /*
     * page_size will be 0 when u32size is 0 and diff is 0,
     * and then mmap will be error (error: invalid argument)
     */
    if ((size == 0) || (size >= 0xFFFFF000)) {
        printf("func: %s size should be in (0, 0xFFFFF000).\n", __FUNCTION__);
        return OT_NULL;
    }

    if (mem_check_smr_userdev_open() != OT_SUCCESS) {
        return OT_NULL;
    }

    /* the mmap address should align with page */
    page_phys = phys_addr & OT_PAGE_MASK;
    /* the mmap size should be multiples of PAGE_SIZE */
    diff = (ot_u32)(phys_addr & (OT_PAGE_SIZE - 1));
    page_size = ((size + diff - 1) & OT_PAGE_MASK) + OT_PAGE_SIZE;

    mem_mutex_lock();
    page_addr = mmap((void *)0, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, g_mem_fd, (ot_ulong)page_phys);
    if (page_addr == MAP_FAILED) {
        mem_mutex_unlock();
        perror("mmap error");
        return OT_NULL;
    }
    mem_mutex_unlock();
    return (ot_void *)(page_addr + diff);
}

ot_void *ot_omi_sys_mmap_cached(ot_phys_addr phys_addr, ot_u32 size)
{
    ot_u32 diff;
    ot_phys_addr page_phys;
    ot_ulong page_size;
    ot_u8 *page_addr = OT_NULL;

    /*
     * page_size will be 0 when u32size is 0 and diff is 0,
     * and then mmap will be error (error: invalid argument)
     */
    if ((size == 0) || (size >= 0xFFFFF000)) {
        printf("func: %s size should be in (0, 0xFFFFF000).\n", __FUNCTION__);
        return OT_NULL;
    }

    if (mem_check_smr_userdev_cache_open() != OT_SUCCESS) {
        return OT_NULL;
    }

    /* the mmap address should align with page */
    page_phys = phys_addr & OT_PAGE_MASK;
    /* the mmap size should be multiples of PAGE_SIZE */
    diff = (ot_u32)(phys_addr & (OT_PAGE_SIZE - 1));
    page_size = ((size + diff - 1) & OT_PAGE_MASK) + OT_PAGE_SIZE;

    mem_mutex_lock();
    page_addr = mmap((void *)0, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, g_mem_cache_fd, (ot_ulong)page_phys);
    if (page_addr == MAP_FAILED) {
        mem_mutex_unlock();
        perror("mmap error");
        return OT_NULL;
    }
    mem_mutex_unlock();
    return (ot_void *)(page_addr + diff);
}

ot_s32 ot_omi_sys_munmap(const ot_void *virt_addr, ot_u32 size)
{
    ot_phys_addr page_addr;
    ot_u32 page_size;
    ot_u32 diff;

    mem_check_null_ptr_return(virt_addr);

    page_addr = (ot_phys_addr)(((uintptr_t)virt_addr) & OT_PAGE_MASK);
    diff = (uintptr_t)virt_addr & (OT_PAGE_SIZE - 1);
    page_size = ((size + diff - 1) & OT_PAGE_MASK) + OT_PAGE_SIZE;
    return munmap((ot_void *)(uintptr_t)page_addr, page_size);
}

/* alloc smr memory in user context */
ot_s32 ot_omi_sys_smr_alloc(ot_phys_addr *phys_addr, ot_void **virt_addr,
    const ot_char *mmb, const ot_char *zone, ot_u32 len)
{
    ot_s32 ret;
    ot_void *mapped = OT_NULL;

    mem_check_null_ptr_return(phys_addr);
    mem_check_null_ptr_return(virt_addr);

    ret = ot_omi_sys_smr_alloc_only(phys_addr, zone, mmb, len);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    mapped = ot_omi_sys_smr_remap_nocache(*phys_addr, len);
    if (mapped == OT_NULL) {
        (ot_void)ot_omi_sys_smr_free_only(*phys_addr, OT_NULL);
        return OT_ERR_MEM_BUSY;
    }

    *virt_addr = mapped;
    return OT_SUCCESS;
}

/* alloc smr memory with cache in user context */
ot_s32 ot_omi_sys_smr_alloc_cached(ot_phys_addr *phys_addr, ot_void **virt_addr,
    const ot_char *mmb, const ot_char *zone, ot_u32 len)
{
    ot_s32 ret;
    ot_void *mapped = OT_NULL;

    mem_check_null_ptr_return(phys_addr);
    mem_check_null_ptr_return(virt_addr);

    ret = ot_omi_sys_smr_alloc_only(phys_addr, zone, mmb, len);
    if (ret != OT_SUCCESS) {
        return ret;
    }

    mapped = ot_omi_sys_smr_remap_cached(*phys_addr, len);
    if (mapped == OT_NULL) {
        (ot_void)ot_omi_sys_smr_free_only(*phys_addr, OT_NULL);
        return OT_ERR_MEM_BUSY;
    }

    *virt_addr = mapped;
    return OT_SUCCESS;
}

/* free smr memory in user context */
ot_s32 ot_omi_sys_smr_free(ot_phys_addr phys_addr, const ot_void *virt_addr)
{
    struct mmb_info mmi = {0};
    ot_s32 ret;

    virt_addr = virt_addr;

    mem_check_smr_userdev_open_return();

    mmi.phys_addr = (ot_ulong)phys_addr;

    mem_mutex_lock();
    ret = ioctl(g_mem_fd, IOC_MMB_USER_UNMAP, &mmi);
    if (ret != OT_SUCCESS) {
        mem_mutex_unlock();
        mem_err_trace("system unmap smr memory failed!\n");
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    ret = ioctl(g_mem_fd, IOC_MMB_FREE, &mmi);
    if (ret != OT_SUCCESS) {
        mem_mutex_unlock();
        mem_err_trace("system free smr memory failed!\n");
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    mem_mutex_unlock();
    return OT_SUCCESS;
}

/* flush cache */
ot_s32 ot_omi_sys_flush_cache(ot_phys_addr phys_addr, ot_void *virt_addr, ot_u32 size)
{
    struct mmb_info mmi = {0};
    ot_phys_addr page_phys;
    ot_u32 page_size;
    ot_u32 diff;
    ot_void *virt_page_addr = OT_NULL;

    mem_check_null_ptr_return(virt_addr);

    if ((size == 0) || (phys_addr == 0)) {
        mem_err_trace("size and addr can't be 0.\n");
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }

    mem_check_smr_userdev_open_return();

    /* the mmap address should align with cache line, 64B */
    page_phys = phys_addr & (~(CACHE_LINE_SIZE - 1));
    diff = (ot_u32)(phys_addr - page_phys);
    virt_page_addr = (ot_u8 *)virt_addr - diff;

    /* the mmap size should be multiples of cache line SIZE */
    page_size = ((size + diff - 1) & (~(CACHE_LINE_SIZE - 1))) + CACHE_LINE_SIZE;

    mmi.phys_addr = page_phys;
    mmi.mapped = virt_page_addr;
    mmi.size = page_size;
    if (ioctl(g_mem_fd, IOC_MMB_SYS_FLUSH_CACHE, &mmi) != OT_SUCCESS) {
        mem_err_trace("mmb flush cache failed!\n");
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    return OT_SUCCESS;
}

/* get virtual meminfo according to virtual addr, should be in one process */
ot_s32 ot_omi_sys_get_virt_mem_info(const ot_void *virt_addr, ot_sys_virt_mem_info *mem_info)
{
    struct mmb_info mmi = {0};
    ot_s32 ret;

    mem_check_null_ptr_return(virt_addr);
    mem_check_null_ptr_return(mem_info);

    mem_check_smr_userdev_open_return();

    mmi.mapped = (void *)virt_addr;

    mem_mutex_lock();
    ret = ioctl(g_mem_fd, IOC_MMB_VIRT_GET_PHYS, &mmi);
    if (ret != OT_SUCCESS) {
        mem_mutex_unlock();
        mem_err_trace("Get virt mem info failed(ret:%d).\n", ret);
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    mem_mutex_unlock();

    mem_info->is_cached = mmi.phys_addr & 0x1;
    mem_info->phys_addr = mmi.phys_addr & (~0x1);
    return OT_SUCCESS;
}

ot_s32 ot_omi_sys_smr_check_phy_addr(ot_phys_addr phys_addr, ot_ulong size)
{
    struct mmb_info mmi = {0};

    if ((size == 0) || (phys_addr == 0)) {
        mem_err_trace("size and addr can't be 0.\n");
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }

    mem_check_smr_userdev_open_return();

    mmi.phys_addr = phys_addr;
    mmi.size = size;
    if (ioctl(g_mem_fd, IOC_MMB_BASE_CHECK_ADDR, &mmi) != OT_SUCCESS) {
        mem_err_trace("system check smr physical address failed!\n");
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    return OT_SUCCESS;
}

/* malloc smr without mmap! */
ot_s32 ot_omi_sys_smr_alloc_only(ot_phys_addr *phys_addr, const ot_char *smr_name,
    const ot_char *buf_name, ot_ulong size)
{
    struct mmb_info mmi = {0};

    mem_check_null_ptr_return(phys_addr);
    mem_check_smr_userdev_open_return();

    mmi.size = size;
    mmi.prot = PROT_READ | PROT_WRITE;
    mmi.flags = MAP_SHARED;
    if (buf_name != OT_NULL) {
        if (strncpy_s(mmi.mmb_name, sizeof(mmi.mmb_name), buf_name, sizeof(mmi.mmb_name) - 1) != EOK) {
            mem_err_trace("copy err!\n");
            return OT_ERR_MEM_ILLEGAL_PARAM;
        }
    }
    if (smr_name != OT_NULL) {
        if (strncpy_s(mmi.smr_name, sizeof(mmi.smr_name), smr_name, sizeof(mmi.smr_name) - 1) != EOK) {
            mem_err_trace("copy err!\n");
            return OT_ERR_MEM_ILLEGAL_PARAM;
        }
    }

    mem_mutex_lock();
    if (ioctl(g_mem_fd, IOC_MMB_ALLOC, &mmi) != OT_SUCCESS) {
        mem_mutex_unlock();
        mem_err_trace("system alloc smr memory failed!\n");
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    mem_mutex_unlock();

    *phys_addr = mmi.phys_addr;
    return OT_SUCCESS;
}

ot_s32 ot_omi_sys_smr_free_only(ot_phys_addr phys_addr, const ot_void *virt_addr)
{
    struct mmb_info mmi = {0};

    mem_check_smr_userdev_open_return();

    mmi.phys_addr = (ot_ulong)phys_addr;

    mem_mutex_lock();
    if (virt_addr != OT_NULL) {
        if (ioctl(g_mem_fd, IOC_MMB_USER_UNMAP, &mmi) != OT_SUCCESS) {
            mem_mutex_unlock();
            mem_err_trace("system unmap smr memory failed!\n");
            return OT_ERR_MEM_ILLEGAL_PARAM;
        }
    }
    if (phys_addr != 0) {
        if (ioctl(g_mem_fd, IOC_MMB_FREE, &mmi) != OT_SUCCESS) {
            mem_mutex_unlock();
            mem_err_trace("system free smr memory failed!\n");
            return OT_ERR_MEM_ILLEGAL_PARAM;
        }
    }
    mem_mutex_unlock();
    return OT_SUCCESS;
}

ot_void *ot_omi_sys_smr_remap_nocache(ot_phys_addr phys_addr, ot_u32 size)
{
    struct mmb_info mmi = {0};
    ot_s32 ret;

    if (mem_check_smr_userdev_open() != OT_SUCCESS) {
        return OT_NULL;
    }

    mmi.size = size;
    mmi.phys_addr = phys_addr;
    mmi.prot = PROT_READ | PROT_WRITE;
    mmi.flags = MAP_SHARED;

    mem_mutex_lock();
    ret = ioctl(g_mem_fd, IOC_MMB_USER_REMAP, &mmi);
    if (ret != OT_SUCCESS) {
        mem_mutex_unlock();
        mem_err_trace("system remap smr nocache failed!\n");
        return OT_NULL;
    }
    mem_mutex_unlock();

    return mmi.mapped;
}

ot_void *ot_omi_sys_smr_remap_cached(ot_phys_addr phys_addr, ot_u32 size)
{
    struct mmb_info mmi = {0};
    ot_s32 ret;

    if (mem_check_smr_userdev_open() != OT_SUCCESS) {
        return OT_NULL;
    }

    mmi.size = size;
    mmi.phys_addr = phys_addr;
    mmi.prot = PROT_READ | PROT_WRITE;
    mmi.flags = MAP_SHARED;

    mem_mutex_lock();
    ret = ioctl(g_mem_fd, IOC_MMB_USER_REMAP_CACHED, &mmi);
    if (ret != OT_SUCCESS) {
        mem_mutex_unlock();
        mem_err_trace("system remap smr cached failed!\n");
        return OT_NULL;
    }
    mem_mutex_unlock();

    return mmi.mapped;
}

ot_s32 ot_omi_sys_smr_unmap(const ot_void *virt_addr)
{
    struct mmb_info mmi = {0};

    mem_check_null_ptr_return(virt_addr);
    mem_check_smr_userdev_open_return();

    mmi.mapped = (void *)virt_addr;

    mem_mutex_lock();
    if (ioctl(g_mem_fd, IOC_MMB_VIRT_GET_PHYS, &mmi) != OT_SUCCESS) {
        mem_mutex_unlock();
        mem_err_trace("system get smr phys_addr failed!\n");
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    if (ioctl(g_mem_fd, IOC_MMB_USER_UNMAP, &mmi) != OT_SUCCESS) {
        mem_mutex_unlock();
        mem_err_trace("system unmap smr memory failed!\n");
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    mem_mutex_unlock();

    return OT_SUCCESS;
}

static ot_s32 omi_sys_mem_convert_smr_errno(ot_s32 smr_errno)
{
    if (smr_errno == 0) {
        return OT_SUCCESS;
    }
    /* errno is thread-local */
    if (errno == EPERM) {
        return OT_ERR_MEM_NOT_PERM;
    } else {
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
}

static ot_s32 omi_sys_mem_do_share(ot_ulong cmd, const ot_void *mem_handle, ot_s32 pid, ot_bool is_share_all)
{
    ot_s32 ret;
    struct mmb_share_info info = {0};

    info.mem_handle = (void *)mem_handle;
    if (is_share_all == OT_FALSE) {
        info.shared_pid = pid;
    }

    mem_check_smr_userdev_open_return();

    mem_mutex_lock();
    ret = ioctl(g_mem_fd, cmd, &info);
    mem_mutex_unlock();
    return omi_sys_mem_convert_smr_errno(ret);
}

ot_s32 ot_omi_sys_mem_share(const ot_void *mem_handle, ot_s32 pid)
{
    ot_s32 ret;

    mem_check_null_ptr_return(mem_handle);

    if (pid < 0) {
        mem_err_trace("invalid pid %d!\n", pid);
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    if (getpgid(pid) < 0) {
        mem_err_trace("the process (pid %d) does not exist!\n", pid);
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    ret = omi_sys_mem_do_share(IOC_MMB_MEM_SHARE, mem_handle, pid, OT_FALSE);
    if (ret != OT_SUCCESS) {
        mem_err_trace("mem share failed!\n");
    }
    return ret;
}

ot_s32 ot_omi_sys_mem_unshare(const ot_void *mem_handle, ot_s32 pid)
{
    ot_s32 ret;

    mem_check_null_ptr_return(mem_handle);

    if (pid < 0) {
        mem_err_trace("invalid pid %d!\n", pid);
        return OT_ERR_MEM_ILLEGAL_PARAM;
    }
    ret = omi_sys_mem_do_share(IOC_MMB_MEM_UNSHARE, mem_handle, pid, OT_FALSE);
    if (ret != OT_SUCCESS) {
        mem_err_trace("mem unshare failed!\n");
    }
    return ret;
}

ot_s32 ot_omi_sys_mem_share_all(const ot_void *mem_handle)
{
    ot_s32 ret;

    mem_check_null_ptr_return(mem_handle);

    ret = omi_sys_mem_do_share(IOC_MMB_MEM_SHARE_ALL, mem_handle, 0, OT_TRUE);
    if (ret != OT_SUCCESS) {
        mem_err_trace("mem share all failed!\n");
    }
    return ret;
}

ot_s32 ot_omi_sys_mem_unshare_all(const ot_void *mem_handle)
{
    ot_s32 ret;

    mem_check_null_ptr_return(mem_handle);

    ret = omi_sys_mem_do_share(IOC_MMB_MEM_UNSHARE_ALL, mem_handle, 0, OT_TRUE);
    if (ret != OT_SUCCESS) {
        mem_err_trace("mem unshare all failed!\n");
    }
    return ret;
}

static ot_void omi_sys_mem_get_mem_info(const struct mmb_share_info *share_info, ot_sys_mem_info *mem_info)
{
    mem_info->phys_addr = share_info->phys_addr;
    mem_info->offset = share_info->offset;
    mem_info->mem_handle = share_info->mem_handle;
}

ot_s32 ot_omi_sys_get_mem_info_by_virt(const ot_void *virt_addr, ot_sys_mem_info *mem_info)
{
    ot_s32 ret;
    struct mmb_share_info info = {0};

    mem_check_null_ptr_return(virt_addr);
    mem_check_null_ptr_return(mem_info);

    info.virt_addr = (void *)virt_addr;

    mem_check_smr_userdev_open_return();

    mem_mutex_lock();
    ret = ioctl(g_mem_fd, IOC_MMB_VIRT_GET_SYS_MEM, &info);
    mem_mutex_unlock();
    ret = omi_sys_mem_convert_smr_errno(ret);
    if (ret != OT_SUCCESS) {
        mem_err_trace("get mem info by virt failed!\n");
        return ret;
    }
    omi_sys_mem_get_mem_info(&info, mem_info);
    return OT_SUCCESS;
}

ot_s32 ot_omi_sys_get_mem_info_by_phys(ot_phys_addr phys_addr, ot_sys_mem_info *mem_info)
{
    ot_s32 ret;
    struct mmb_share_info info = {0};

    mem_check_null_ptr_return(mem_info);

    info.phys_addr = phys_addr;

    mem_check_smr_userdev_open_return();

    mem_mutex_lock();
    ret = ioctl(g_mem_fd, IOC_MMB_PHYS_GET_SYS_MEM, &info);
    mem_mutex_unlock();
    ret = omi_sys_mem_convert_smr_errno(ret);
    if (ret != OT_SUCCESS) {
        mem_err_trace("get mem info by phys failed!\n");
        return ret;
    }
    omi_sys_mem_get_mem_info(&info, mem_info);
    return OT_SUCCESS;
}

ot_s32 ot_omi_sys_get_mem_info_by_handle(const ot_void *mem_handle, ot_sys_mem_info *mem_info)
{
    ot_s32 ret;
    struct mmb_share_info info = {0};

    mem_check_null_ptr_return(mem_handle);
    mem_check_null_ptr_return(mem_info);

    info.mem_handle = (void *)mem_handle;

    mem_check_smr_userdev_open_return();

    mem_mutex_lock();
    ret = ioctl(g_mem_fd, IOC_MMB_HANDLE_GET_SYS_MEM, &info);
    mem_mutex_unlock();
    ret = omi_sys_mem_convert_smr_errno(ret);
    if (ret != OT_SUCCESS) {
        mem_err_trace("get mem info by handle failed!\n");
        return ret;
    }
    omi_sys_mem_get_mem_info(&info, mem_info);
    return OT_SUCCESS;
}
