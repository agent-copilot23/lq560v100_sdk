#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "ot_eis_type.h"
#include "ot_eis_list.h"
#include "ot_eis_errno.h"
#include "ot_omi_sys_mem.h"

#include "ot_smr.h"

ot_bool smr_init = OT_FALSE;

OT_LIST_HEAD(g_map_info_list);

typedef struct {
    struct ot_list_head list;
    ot_phys_addr phy_addr;
    ot_void* virt_addr;
    ot_bool cached;
    ot_u32 size;
} mmap_info;

static pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER ;

//todo unmap(size 0) && flush(phy_addr virt_addr len map_attr)
static ot_s32 smr_errcode_trans(ot_s32 err)
{
    switch (err) {
        case OT_ERR_MEM_ILLEGAL_PARAM:
            return OT_ERR_SMR_ILLEGAL_PAARAM;
        case OT_ERR_MEM_NULL_PTR:
            return OT_ERR_SMR_NULL_PTR;
        default:
            return OT_ERR_SMR_NOT_SUPPORT;
    }
}

static ot_void smr_mmap_info_cache(ot_phys_addr phy_addr, ot_void* virt_addr, ot_u32 size, ot_bool cached)
{
    mmap_info* map_node = OT_NULL;

    pthread_mutex_lock(&map_mutex);
    map_node = (mmap_info*)malloc(sizeof(mmap_info));
    if(map_node == OT_NULL) {
        printf("mmap info cache malloc fail! virt[%p] end_addr[%p]\n", virt_addr, virt_addr+size);
        goto cache_exit;
    }
    map_node->phy_addr = phy_addr;
    map_node->virt_addr = virt_addr;
    map_node->size = size;
    map_node->cached = cached;
    OT_INIT_LIST_HEAD(&map_node->list);

    ot_list_add(&map_node->list, &g_map_info_list);
cache_exit:
    pthread_mutex_unlock(&map_mutex);
}

static ot_bool smr_mumap_virt(ot_void* virt_addr, ot_u32 len)
{
    mmap_info *p = OT_NULL;
    mmap_info *n = OT_NULL;

    pthread_mutex_lock(&map_mutex);
    ot_list_for_each_entry_safe(p, n, &g_map_info_list, list) {
        if(virt_addr >= p->virt_addr && ((virt_addr + len) <= (p->virt_addr + p->size))) {
            ot_list_del(&p->list);
            free(p);
            pthread_mutex_unlock(&map_mutex);
            return OT_TRUE;
        }
    }
    pthread_mutex_unlock(&map_mutex);
    return OT_FALSE;
}

static ot_s32 smr_query_mmap_phys_by_virt(ot_void* virt_addr, ot_phys_addr* phy_addr, ot_bool *cached)
{
    ot_bool ret;
    ot_phys_addr phys_tmp;
    mmap_info *p = OT_NULL;
    mmap_info *n = OT_NULL;

    pthread_mutex_lock(&map_mutex);
    ot_list_for_each_entry_safe(p, n, &g_map_info_list, list) {
        if(virt_addr >= p->virt_addr && (virt_addr < (p->virt_addr + p->size))) {
            phys_tmp = p->phy_addr + (virt_addr - p->virt_addr);
            *phy_addr = phys_tmp;
            *cached = p->cached;
            ret = 0;
            goto query_exit;
        }
    }
    ret = -1;
query_exit:
    pthread_mutex_unlock(&map_mutex);
    return ret;
}

ot_s32 ot_smr_init(ot_void)
{
    if(smr_init == OT_TRUE){
        return 0;
    }
    smr_init = OT_TRUE;

    return 0;
}

ot_s32 ot_smr_deinit(ot_void)
{
    if(smr_init == OT_FALSE){
        return 0;
    }

    smr_init = OT_FALSE;
    return 0;
}

ot_s32 ot_smr_alloc(const ot_smr_alloc_attr* attr, ot_phys_addr* phy_addr, ot_void** virt_addr)
{
    ot_s32 ret = -1;
    ot_phys_addr phys_tmp;
    ot_void* virt_addr_tmp;

    if(smr_init == OT_FALSE){
        printf("smr not init yet!\n");
        return OT_ERR_SMR_NOT_INIT;
    }

    if(attr == OT_NULL || attr->len <= 0 || phy_addr == OT_NULL || virt_addr == OT_NULL){
        printf("param illegal!\n");
        return OT_ERR_SMR_ILLEGAL_PAARAM;
    }

    if(strlen((const char*)attr->region_name) == 0 || strcmp((const char*)attr->region_name, "anony") == 0){
        if(attr->cached == OT_FALSE){
            ret = ot_omi_sys_smr_alloc(&phys_tmp, &virt_addr_tmp, (const char*)attr->chunk_name, OT_NULL, attr->len);
        }else{
            ret = ot_omi_sys_smr_alloc_cached(&phys_tmp, &virt_addr_tmp, (const char*)attr->chunk_name, OT_NULL, attr->len);
        }
    }else{
        if(attr->cached == OT_FALSE){
            ret = ot_omi_sys_smr_alloc(&phys_tmp, &virt_addr_tmp, (const char*)attr->chunk_name, (const char*)attr->region_name, attr->len);
        }else {
            ret = ot_omi_sys_smr_alloc_cached(&phys_tmp, &virt_addr_tmp, (const char*)attr->chunk_name, (const char*)attr->region_name, attr->len);
        }
    }

    if(ret != 0){
        printf("smr alloc err_code[0x%x]\n", ret);
        return smr_errcode_trans(ret);
    }

    *phy_addr = (ot_phys_addr)phys_tmp;
    *virt_addr = (ot_void*)virt_addr_tmp;
    return 0;
}

ot_s32 ot_smr_free(ot_phys_addr phy_addr)
{
    ot_s32 ret = -1;

    if(smr_init == OT_FALSE){
        printf("smr not init yet!\n");
        return OT_ERR_SMR_NOT_INIT;
    }

    if(phy_addr <= 0){
        printf("param illegal!\n");
        return OT_ERR_SMR_ILLEGAL_PAARAM;
    }

    ret = ot_omi_sys_smr_free((ot_phys_addr)phy_addr, OT_NULL);
    if(ret != 0){
        printf("smr free err_code[0x%x]\n", ret);
        return smr_errcode_trans(ret);
    }
    return 0;
}

ot_s32 ot_smr_mmap(ot_phys_addr phy_addr, ot_u32 size, ot_bool cached, ot_void** virt_addr)
{
   ot_void* virt_addr_tmp = OT_NULL;

   if(smr_init == OT_FALSE){
       printf("smr not init yet!\n");
       return OT_ERR_SMR_NOT_INIT;
   }

   if(size <= 0 || phy_addr <= 0 || virt_addr == OT_NULL){
        printf("param illegal!\n");
        return OT_ERR_SMR_ILLEGAL_PAARAM;
    }

   if(cached == OT_TRUE){
       virt_addr_tmp = ot_omi_sys_mmap_cached((ot_phys_addr)phy_addr, size);
   }else{
       virt_addr_tmp = ot_omi_sys_mmap((ot_phys_addr)phy_addr, size);
   }

   if(virt_addr_tmp == OT_NULL){
       return -1;
   }

   *virt_addr = (ot_void*)virt_addr_tmp;
   smr_mmap_info_cache(phy_addr, *virt_addr, size, cached);
   return 0;

}

ot_s32 ot_smr_munmap(ot_void* virt_addr, ot_u32 size)
{
    ot_s32 ret = -1;

    if(smr_init == OT_FALSE){
        printf("smr not init yet!\n");
        return OT_ERR_SMR_NOT_INIT;
    }

    if(virt_addr == OT_NULL || size <= 0){
         printf("param illegal!\n");
         return OT_ERR_SMR_ILLEGAL_PAARAM;
     }

    if(smr_mumap_virt(virt_addr, size) == OT_FALSE) {
       printf("smr munmap virt[%p] end_addr[%p] size[%d] error\n", virt_addr, virt_addr+size, size);
       return OT_ERR_SMR_ILLEGAL_PAARAM;
    }

    ret = ot_omi_sys_munmap((const void*)virt_addr, size);
    if(ret != 0){
        printf("smr unmap err_code[0x%x]\n", ret);
        return smr_errcode_trans(ret);
    }
    return 0;
}

ot_s32 ot_smr_virt2phy(ot_void* virt_addr, ot_phys_addr* phy_addr)
{
    ot_s32 ret = -1;
    ot_sys_virt_mem_info mem_info;

    if(smr_init == OT_FALSE){
        printf("smr not init yet!\n");
        return OT_ERR_SMR_NOT_INIT;
    }

    if(virt_addr == OT_NULL || phy_addr == OT_NULL){
         printf("param null!\n");
         return OT_ERR_SMR_ILLEGAL_PAARAM;
     }

    ret = smr_query_mmap_phys_by_virt(virt_addr, &mem_info.phys_addr, &mem_info.is_cached);
    if(ret == 0) {
        goto virt_query_exit;
    }

    ret = ot_omi_sys_get_virt_mem_info((const void*)virt_addr, &mem_info);
    if(ret != 0){
        printf("smr get meminfo err_code[0x%x]\n", ret);
        return smr_errcode_trans(ret);
    }
virt_query_exit:
    *phy_addr = (ot_phys_addr)mem_info.phys_addr;
    return 0;
}

ot_bool ot_smr_is_cached(ot_void* virt_addr)
{
    ot_s32 ret = -1;
    ot_sys_virt_mem_info mem_info;

    if(smr_init == OT_FALSE){
        printf("smr not init yet!\n");
        return OT_FALSE;
     }

    if(virt_addr == OT_NULL){
        printf("param virt_addr null!\n");
        return OT_FALSE;
    }

    ret = smr_query_mmap_phys_by_virt(virt_addr, &mem_info.phys_addr, &mem_info.is_cached);
    if(ret == 0) {
        goto is_cache_exit;
    }

    ret = ot_omi_sys_get_virt_mem_info((const void*)virt_addr, &mem_info);
    if(ret != 0){
        printf("smr get is cached err_code[0x%x]\n", ret);
        return OT_FALSE;
    }
is_cache_exit:
    return (ot_bool)mem_info.is_cached;
}
ot_s32 ot_smr_flush_cache(ot_phys_addr phy_addr, ot_void* virt_addr, ot_u32 size)
{
    ot_s32 ret = -1;

    if(smr_init == OT_FALSE){
        printf("smr not init yet!\n");
        return OT_ERR_SMR_NOT_INIT;
    }

    if(virt_addr == OT_NULL || phy_addr <= 0 || size <= 0){
         return OT_ERR_SMR_ILLEGAL_PAARAM;
     }

    ret = ot_omi_sys_flush_cache((ot_phys_addr)phy_addr, (ot_void*)virt_addr, size);
    if(ret != 0){
        printf("ot_smr_flush_cache err_code[0x%x]\n", ret);
        return smr_errcode_trans(ret);
    }

    return 0;
}
