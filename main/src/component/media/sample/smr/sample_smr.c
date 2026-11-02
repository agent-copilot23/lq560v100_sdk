#include <stdio.h>
#include <string.h>
#include "ot_eis_type.h"
#include "ot_smr.h"

static void smr_proc(){
//    FILE *file;
//    char buffer[10240];
//
//    file = fopen("/proc/umap/smr", "r");
//    if(file == NULL){
//        printf("open media_res_proc file fail!\n");
//        return;
//    }
//
//    while(fgets(buffer, 10240, file) != NULL){
//        printf("%s\n",buffer);
//    }
//
//    fclose(file);
    return;

}

static void case1()
{

    ot_s32 ret = -1;
    ot_smr_alloc_attr alloc_attr ={0};
    ot_phys_addr phys, phys2;
    ot_void* virt;
    ot_void* virt2;
    unsigned long long len1;
    len1 = 1024*1024*50;

    ret = ot_smr_init();
    if(ret != 0){
        printf("case1 smr init fail!ret[%d]\n",ret);
        return;
    }

    strcpy(alloc_attr.region_name, "");
    alloc_attr.len = len1;

    ret = ot_smr_alloc(&alloc_attr, &phys, &virt);
    if(ret != 0){
        printf("case1 smr alloc fail!ret[%d]\n",ret);
        return;
    }
    printf("case1 smr alloc success ! phys[0x%llx] virt[%p]--------\n", phys, virt);
    smr_proc();

    ret = ot_smr_virt2phy(virt, &phys2);
    if(ret != 0){
        printf("case1 smr get phy by virt fail!ret[%d]\n",ret);
        return;
    }
    printf("case1 get phy[0x%llx] by virt[0x%p] !\n", phys2, virt);

    ret = ot_smr_mmap(phys, alloc_attr.len, 1, &virt2);
    if(ret != 0){
        printf("case1 smr mmap cached fail!ret[%d]\n",ret);
        ot_smr_free(phys);
        return;
    }
    printf("case1 smr mmap cached success ! virt[%p]\n", virt2);
    ret = ot_smr_virt2phy(virt2, &phys2);
    if(ret != 0){
        printf("case1 smr get phy by virt fail!ret[%d]\n",ret);
        return;
    }
    printf("case1 ueser mmap virt get phy[0x%llx] by virt[0x%p] !\n", phys2, virt2);
    printf("case1 user mmap cached get status[%d]\n",ot_smr_is_cached(virt2));

    ret = ot_smr_munmap(virt2, alloc_attr.len);
    if(ret != 0){
        printf("case1 smr ummap  fail!ret[%d]\n",ret);
        return;
    }

    ret = ot_smr_virt2phy(virt, &phys2);
    if(ret != 0){
        printf("case1 smr get phy by virt fail!ret[%d]\n",ret);
        return;
    }
    printf("case1 ker get phy[0x%llx] by virt[0x%p] !\n", phys2, virt);

    ret = ot_smr_virt2phy(virt2, &phys2);
    if(ret != 0){
        printf("case1 user mmap get phy by virt[0x%p] fail!ret[%d]\n", virt2, ret);
    }
    printf("case1 mmap virt get phy[0x%llx] by virt[0x%p] !\n", phys2, virt2);

    ret = ot_smr_free(phys);
    if(ret != 0){
        printf("case1 smr free fail!ret[%d]\n",ret);
        return;
    }
    printf("case1 smr free 1----------\n");
    smr_proc();

    ret = ot_smr_virt2phy(virt, &phys);
    if(ret != 0){
        printf("case1 smr after free get phy by virt fail!ret[%d]\n",ret);
    }
    printf("case1 ker get phy[0x%llx] by virt[0x%p] !\n", phys, virt);

    printf("case1 smr free 2---------\n");

    ret = ot_smr_deinit();
    if(ret != 0){
        printf("case1 smr deinit fail!ret[%d]\n",ret);
        return;
    }

    printf("case1 end\n");

}

int main()
{
    printf("main start\n");

    case1();

    return 0;
}


