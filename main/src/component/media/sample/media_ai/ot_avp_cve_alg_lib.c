#include "ot_avp_cve_alg_lib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ot_smr.h"
#include <sys/time.h>
#include <math.h>


ot_s32 ot_avp_cve_create_mem_info(ot_mem_info *buf, ot_u32 size)
{
    if (buf == OT_NULL || size == 0) {
        printf("ot_avp_cve_create_mem_info buf is OT_NULL or size == 0.\n");
        return OT_FAILURE;
    }
    ot_smr_alloc_attr alloc_attr;
    ot_phys_addr phys_addr = 0;
    alloc_attr.len = size;
    alloc_attr.align = 1;

    strcpy(alloc_attr.region_name, "anony");

    buf->size = size;
    if (ot_smr_alloc(&alloc_attr, &phys_addr, (ot_void **)&buf->virt_addr) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    buf->phys_addr = (ot_u64)phys_addr;
    memset((ot_void *)buf->virt_addr, 0, size);
    ot_smr_flush_cache((ot_phys_addr)buf->phys_addr, (ot_void *)buf->virt_addr, size);

    return OT_AVP_CVE_SUCCESS;
}

ot_void ot_avp_cve_destory_mem_info(ot_mem_info *buf)
{
    if (buf->virt_addr != 0) {
        ot_smr_free((ot_phys_addr)buf->phys_addr);
    }
    memset(buf, 0, sizeof(ot_mem_info));
}

ot_void ot_avp_cve_set_mem_info(ot_mem_info *buf, ot_u8 val, ot_u32 size)
{
    memset((ot_void *)buf->virt_addr, val, size);
    ot_smr_flush_cache((ot_phys_addr)buf->phys_addr, (ot_void *)buf->virt_addr, size);
}

ot_s32 ot_avp_cve_create_data_info(ot_data_info *buf, ot_u32 width, ot_u32 height)
{
    if (buf == OT_NULL || width == 0 || height == 0) {
        printf("ot_avp_cve_create_mem_info buf is OT_NULL or size == 0.\n");
        return OT_FAILURE;
    }

    ot_smr_alloc_attr alloc_attr;
    ot_phys_addr phys_addr = 0;

    buf->width = width;
    buf->height = height;
    buf->stride = CVE_ALIGN_UP(width, OT_AVP_CVE_DEFAULT_ALIGN);
    buf->capacity = buf->stride * buf->height;

    alloc_attr.len = buf->capacity;
    alloc_attr.align = 1;

    strcpy(alloc_attr.region_name, "anony");

    if (ot_smr_alloc(&alloc_attr, &phys_addr, (ot_void **)&buf->virt_addr) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    buf->phys_addr = (ot_u64)phys_addr;
    memset((ot_void *)buf->virt_addr, 0, buf->capacity);
    ot_smr_flush_cache((ot_phys_addr)buf->phys_addr, (ot_void *)buf->virt_addr, buf->capacity);

    return OT_AVP_CVE_SUCCESS;
}

ot_void ot_avp_cve_destory_data_info(ot_data_info *buf)
{
    if (buf->virt_addr != 0) {
        ot_smr_free((ot_phys_addr)buf->phys_addr);
    }
    memset(buf, 0, sizeof(ot_data_info));
}


ot_s32 ot_avp_cve_create_single_planar_img(ot_avp_cve_img *img)
{
    ot_u32 stride, img_size, size;
    ot_smr_alloc_attr alloc_attr;
    ot_phys_addr phys_addr = 0;
    switch (img->type) {
        case OT_IMAGE_FORMAT_U8C1:
        case OT_IMAGE_FORMAT_S8C1:
        case OT_IMAGE_FORMAT_YUV_400:
            size = sizeof(ot_u8);
            break;
        case OT_IMAGE_FORMAT_U16C1:
        case OT_IMAGE_FORMAT_S16C1:
            size = sizeof(ot_u16);
            break;
        case OT_IMAGE_FORMAT_U32C1:
        case OT_IMAGE_FORMAT_S32C1:
            size = sizeof(ot_u32);
            break;
        case OT_IMAGE_FORMAT_U64C1:
        case OT_IMAGE_FORMAT_S64C1:
            size = sizeof(ot_u64);
            break;
        default:
            return OT_FAILURE;
    }

    stride = CVE_ALIGN_UP(img->width *size, OT_AVP_CVE_DEFAULT_ALIGN);
    printf("[ot_avp_cve_create_single_planar_img] stride=%u\n",stride);

    img_size = img->height * stride;

    alloc_attr.len = img_size;
    alloc_attr.align = 1;

    strcpy(alloc_attr.region_name, "anony");

    if (ot_smr_alloc(&alloc_attr, &phys_addr, (ot_void **)&img->virt_addr[0]) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    memset((ot_void *)img->virt_addr[0], 0, img_size);
    img->capacity = img_size;
    img->stride[0] = stride;
    img->phys_addr[0] = (ot_u64)phys_addr;
    printf("[ot_avp_cve_create_single_planar_img]img->virt_addr[0] =%llx,img->phys_addr[0] =%llx,img->stride[0] = %u;\n",img->virt_addr[0],img->phys_addr[0],stride);

    return OT_AVP_CVE_SUCCESS;
}

ot_s32 ot_avp_cve_create_semi_planar_img(ot_avp_cve_img *img)
{
    ot_u32 stride, img_size;
    ot_smr_alloc_attr alloc_attr = {0};
    ot_phys_addr phys_addr = 0;

    stride = CVE_ALIGN_UP(img->width, OT_AVP_CVE_DEFAULT_ALIGN);
    img->stride[0] = stride;
    img->stride[1] = stride;

    switch (img->type) {
        case OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR:
        case OT_IMAGE_FORMAT_YVU_420_SEMIPLANAR:
            img_size = img->height * stride * 3 / 2;
            break;
        case OT_IMAGE_FORMAT_YUV_422_SEMIPLANAR:
        case OT_IMAGE_FORMAT_YVU_422_SEMIPLANAR:
            img_size = img->height * stride * 2;
            break;
        default:
            return OT_FAILURE;
    }

    alloc_attr.len = img_size;
    alloc_attr.align = 1;

    strcpy(alloc_attr.region_name, "anony");

    if (ot_smr_alloc(&alloc_attr, &phys_addr, (ot_void **)&img->virt_addr[0]) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    memset((ot_void *)img->virt_addr[0], 0, img_size);
    img->virt_addr[1] = img->virt_addr[0] + img->stride[0] * img->height;
    img->phys_addr[0] = (ot_u64)phys_addr;
    img->phys_addr[1] = img->phys_addr[0] + img->stride[0] * img->height;
    img->capacity = img_size;
    // printf("[ot_avp_cve_create_semi_planar_img]img->virt_addr[0] =%llx,img->phys_addr[0] =%llx,img->stride[0] = %u;\n",img->virt_addr[0],img->phys_addr[0],stride);
    return OT_AVP_CVE_SUCCESS;
}

ot_s32 ot_avp_cve_create_planar_img(ot_avp_cve_img *img)
{
    ot_u32 stride = 0;
    ot_u32 img_size = 0;
    ot_smr_alloc_attr alloc_attr = {0};
    ot_phys_addr phys_addr = 0;

    alloc_attr.align = 1;

    strcpy(alloc_attr.region_name, "anony");

    switch (img->type) {
        case OT_IMAGE_FORMAT_YUV_420_PLANAR:
        case OT_IMAGE_FORMAT_YVU_420_PLANAR:
            img->stride[0] = CVE_ALIGN_UP(img->width, OT_AVP_CVE_DEFAULT_ALIGN);
            img->stride[1] = CVE_ALIGN_UP(img->width / 2, OT_AVP_CVE_DEFAULT_ALIGN);
            img->stride[2] = CVE_ALIGN_UP(img->width / 2, OT_AVP_CVE_DEFAULT_ALIGN);
            img_size = img->height * img->stride[0] + img->height * img->stride[1] + img->height * img->stride[2];

            alloc_attr.len = img_size;
            if (ot_smr_alloc(&alloc_attr, &phys_addr, (ot_void **)&img->virt_addr[0]) != OT_SUCCESS) {
                return OT_FAILURE;
            }
            img->virt_addr[1] = img->virt_addr[0] + img->stride[0] * img->height;
            img->virt_addr[2] = img->virt_addr[1] + img->stride[1] * img->height / 2;

            img->phys_addr[0] = (ot_u64)phys_addr;
            img->phys_addr[1] = img->phys_addr[0] + img->stride[0] * img->height;
            img->phys_addr[2] = img->phys_addr[1] + img->stride[1] * img->height / 2;

            break;
        case OT_IMAGE_FORMAT_YUV_422_PLANAR:
        case OT_IMAGE_FORMAT_YVU_422_PLANAR:
            img->stride[0] = CVE_ALIGN_UP(img->width, OT_AVP_CVE_DEFAULT_ALIGN);
            img->stride[1] = CVE_ALIGN_UP(img->width / 2, OT_AVP_CVE_DEFAULT_ALIGN);
            img->stride[2] = CVE_ALIGN_UP(img->width / 2, OT_AVP_CVE_DEFAULT_ALIGN);
            img_size = img->height * img->stride[0] + img->height * img->stride[1] + img->height * img->stride[2];

            alloc_attr.len = img_size;
            if (ot_smr_alloc(&alloc_attr, &phys_addr, (ot_void **)&img->virt_addr[0]) != OT_SUCCESS) {
                return OT_FAILURE;
            }

            img->virt_addr[1] = img->virt_addr[0] + img->stride[0] * img->height;
            img->virt_addr[2] = img->virt_addr[1] + img->stride[1] * img->height;

            img->phys_addr[0] = (ot_u64)phys_addr;
            img->phys_addr[1] = img->phys_addr[0] + img->stride[0] * img->height;
            img->phys_addr[2] = img->phys_addr[1] + img->stride[1] * img->height;

            break;
        default:
            return OT_FAILURE;
    }
    printf("[ot_avp_cve_create_planar_img]img->virt_addr[0] =%llx,img->phys_addr[0] =%llx,img->stride[0] = %u;\n",img->virt_addr[0],img->phys_addr[0],stride);
    img->capacity = img_size;
    return OT_AVP_CVE_SUCCESS;
}

ot_s32 ot_avp_cve_create_multi_planar_img(ot_avp_cve_img *img)
{
    ot_u32 stride, img_size;
    ot_smr_alloc_attr alloc_attr = {0};
    ot_phys_addr phys_addr = 0;
    ot_void *virt_addr = OT_NULL;

    stride = CVE_ALIGN_UP(img->width, OT_AVP_CVE_DEFAULT_ALIGN);
    img->stride[0] = stride;
    img->stride[1] = stride;
    img->stride[2] = stride;

    img_size = img->height * img->stride[0] * 3;

    alloc_attr.align = 1;

    strcpy(alloc_attr.region_name, "anony");
    alloc_attr.len = img_size;
    if (ot_smr_alloc(&alloc_attr, &phys_addr, (ot_void **)&img->virt_addr[0]) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    img->virt_addr[0] = (ot_u64)virt_addr;
    img->virt_addr[1] = img->virt_addr[0] + img->stride[0] * img->height;
    img->virt_addr[2] = img->virt_addr[1] + img->stride[1] * img->height;

    img->phys_addr[0] = (ot_u64)phys_addr;
    img->phys_addr[1] = img->phys_addr[0] + img->stride[0] * img->height;
    img->phys_addr[2] = img->phys_addr[1] + img->stride[1] * img->height;
    img->capacity = img_size;
    printf("[ot_avp_cve_create_multi_planar_img] img->virt_addr[0]=%llx, img->phys_addr[0]=%llx, img->stride[0]=%u;\n",
        img->virt_addr[0], img->phys_addr[0], stride);

    return OT_AVP_CVE_SUCCESS;
}

ot_s32 ot_avp_cve_create_packed_img(ot_avp_cve_img *img)
{
    ot_u32 stride, img_size;
    ot_smr_alloc_attr alloc_attr = {0};
    ot_phys_addr phys_addr = 0;
    ot_void *virt_addr = OT_NULL;

    switch (img->type) {
        case OT_IMAGE_FORMAT_U8C3_PACKED:
        case OT_IMAGE_FORMAT_S8C3_PACKED:
            stride = CVE_ALIGN_UP(img->width * 3, OT_AVP_CVE_DEFAULT_ALIGN);
            break;
        default:
            stride = CVE_ALIGN_UP(img->width * 2, OT_AVP_CVE_DEFAULT_ALIGN);
            break;
    }

    img->stride[0] = stride;
    img->stride[1] = 0;
    img->stride[2] = 0;

    img_size = img->height * stride;

    alloc_attr.align = 1;

    strcpy(alloc_attr.region_name, "anony");
    alloc_attr.len = img_size;
    if (ot_smr_alloc(&alloc_attr, &phys_addr, (ot_void **)&virt_addr) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    img->virt_addr[0] = (ot_u64)virt_addr;
    img->virt_addr[1] = img->virt_addr[0] + 1;
    img->virt_addr[2] = img->virt_addr[1] + 1;

    img->phys_addr[0] = (ot_u64)phys_addr;
    img->phys_addr[1] = img->phys_addr[0] + 1;
    img->phys_addr[2] = img->phys_addr[1] + 1;
    img->capacity = img_size;
    printf("[ot_avp_cve_create_packed_img] img->virt_addr[0]=%llx, img->phys_addr[0]=%llx, img->stride[0]=%u;\n",
        img->virt_addr[0], img->phys_addr[0], stride);

    return OT_AVP_CVE_SUCCESS;
}

ot_s32 ot_avp_cve_create_bitmap_img(ot_avp_cve_img *img)
{
    ot_u32 stride, img_size;
    ot_smr_alloc_attr alloc_attr = {0};
    ot_phys_addr phys_addr = 0;
    ot_void *virt_addr = OT_NULL;

    stride = CVE_ALIGN_UP((img->width >> 3) + ((img->width % 8) ? 1 : 0), OT_AVP_CVE_DEFAULT_ALIGN);

    img->stride[0] = stride;
    img->stride[1] = 0;
    img->stride[2] = 0;

    img_size = img->height * stride;

    alloc_attr.align = 1;
    strcpy(alloc_attr.region_name, "anony");
    alloc_attr.len = img_size;
    if (ot_smr_alloc(&alloc_attr, &phys_addr, (ot_void **)&virt_addr) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    img->virt_addr[0] = (ot_u64)virt_addr;
    img->virt_addr[1] = 0;
    img->virt_addr[2] = 0;

    img->phys_addr[0] = (ot_u64)phys_addr;
    img->phys_addr[1] = 0;
    img->phys_addr[2] = 0;
    img->capacity = img_size;
    printf("[ot_avp_cve_create_packed_img] img->virt_addr[0]=%llx, img->phys_addr[0]=%llx, img->stride[0]=%u;\n",
        img->virt_addr[0], img->phys_addr[0], stride);

    return OT_AVP_CVE_SUCCESS;
}


ot_s32 ot_avp_cve_create_img(ot_avp_cve_img *img, ot_image_pixel_format img_type, ot_u32 width, ot_u32 height)
{
    if (width == 0 || height == 0 || img == NULL || img_type >= OT_IMAGE_FORMAT_BUTT) {
        return OT_FAILURE;
    }

    img->type = img_type;
    img->width = width;
    img->height = height;
    // printf("[ot_avp_cve_create_img----]img=%p, img->type=%d, img->width=%d, img->height=%d.\n", img,img->type, img->width, img->height);
    switch (img_type) {
        case OT_IMAGE_FORMAT_BITMAP:
            return ot_avp_cve_create_bitmap_img(img);
        case OT_IMAGE_FORMAT_U8C1:
        case OT_IMAGE_FORMAT_S8C1:
        case OT_IMAGE_FORMAT_U16C1:
        case OT_IMAGE_FORMAT_S16C1:
        case OT_IMAGE_FORMAT_U32C1:
        case OT_IMAGE_FORMAT_S32C1:
        case OT_IMAGE_FORMAT_U64C1:
        case OT_IMAGE_FORMAT_S64C1:
        case OT_IMAGE_FORMAT_YUV_400:
            return ot_avp_cve_create_single_planar_img(img);
        case OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR:
        case OT_IMAGE_FORMAT_YUV_422_SEMIPLANAR:
        case OT_IMAGE_FORMAT_YVU_420_SEMIPLANAR:
        case OT_IMAGE_FORMAT_YVU_422_SEMIPLANAR:
            return ot_avp_cve_create_semi_planar_img(img);
        case OT_IMAGE_FORMAT_YUV_420_PLANAR:
        case OT_IMAGE_FORMAT_YUV_422_PLANAR:
        case OT_IMAGE_FORMAT_YVU_420_PLANAR:
        case OT_IMAGE_FORMAT_YVU_422_PLANAR:
            return ot_avp_cve_create_planar_img(img);
        case OT_IMAGE_FORMAT_U8C3_PLANAR:
        case OT_IMAGE_FORMAT_S8C3_PLANAR:
        case OT_IMAGE_FORMAT_YUV_444_PLANAR:
            return ot_avp_cve_create_multi_planar_img(img);
        case OT_IMAGE_FORMAT_U8C3_PACKED:
        case OT_IMAGE_FORMAT_S8C3_PACKED:
            return ot_avp_cve_create_packed_img(img);
        default:
            printf("Not support create image for type:%d.\n", img_type);
            break;
    }

    return OT_AVP_CVE_SUCCESS;
}

ot_void ot_avp_cve_destory_img(ot_avp_cve_img *img)
{
    if (img->virt_addr[0] != 0) {
        ot_smr_free(img->phys_addr[0]);
    }
    memset(img, 0, sizeof(ot_avp_cve_img));
}

ot_s32 ot_data_info_read_file(ot_data_info* data_info, const ot_s8 *file_path)
{
    ot_s32 ret = OT_FAILURE;
    FILE* fd = fopen(file_path, "rb");
    if (fd == NULL) {
        printf("fopen fail. file_path:%s\n", file_path);
        return ret;
    }

    ot_u8 *tmp = (ot_u8 *)data_info->virt_addr;
    for (ot_u32 h = 0; h < data_info->height; h++) {
        if (fread(tmp, data_info->width, 1, fd) != 1) {
            printf("Read file fail\n");
            goto EXIT;
        }
        tmp += data_info->stride;
    }

    ot_smr_flush_cache((ot_phys_addr)data_info->phys_addr, (ot_void *)data_info->virt_addr, data_info->capacity);
    printf("[ot_data_info_read_file] read file %s success\n",file_path);
    ret = OT_AVP_CVE_SUCCESS;

EXIT:
    fclose(fd);
    return ret;
}

ot_s32 ot_data_info_write_file(ot_data_info* data_info, const ot_s8 *file_path)
{
    ot_s32 ret = OT_FAILURE;
    FILE* fd = fopen(file_path, "wb");
    if (fd == NULL) {
        printf("fopen fail. file_path:%s\n", file_path);
        ret = 101;
        return ret;
    }

    ot_u8 *tmp = (ot_u8 *)data_info->virt_addr;
    for (ot_u32 h = 0; h < data_info->height; h++) {
        if (fwrite(tmp, data_info->width, 1, fd) != 1) {
            printf("write file fail\n");
            goto EXIT;
        }
        tmp += data_info->stride;
    }

    ret = OT_AVP_CVE_SUCCESS;
EXIT:
    fclose(fd);
    return ret;
}

ot_s32 ot_mem_info_read_file(ot_mem_info* mem_info, const ot_s8 *file_path)
{
    ot_s32 ret = OT_FAILURE;
    FILE* fd = fopen(file_path, "rb");
    if (fd == NULL) {
        printf("fopen fail. file_path:%s\n", file_path);
        return ret;
    }

    ot_u8 *tmp = (ot_u8 *)mem_info->virt_addr;
    if (fread(tmp, mem_info->size, 1, fd) != 1) {
        printf("Read file fail\n");
        goto EXIT;
    }
    ot_smr_flush_cache((ot_phys_addr)mem_info->phys_addr, (ot_void *)mem_info->virt_addr, mem_info->size);
    printf("[ot_mem_info_read_file] read file %s success\n",file_path);
    ret = OT_AVP_CVE_SUCCESS;
EXIT:
    fclose(fd);
    return ret;
}

ot_s32 ot_mem_info_write_file(ot_mem_info* mem_info, const ot_s8 *file_path)
{
    ot_s32 ret = OT_FAILURE;
    FILE* fd = fopen(file_path, "wb");
    if (fd == NULL) {
        printf("fopen fail. file_path:%s\n", file_path);
        ret = 101;
        return ret;
    }

    ot_u8 *tmp = (ot_u8 *)mem_info->virt_addr;
    if (fwrite(tmp, mem_info->size, 1, fd) != 1) {
        printf("write file fail\n");
        ret =102;
        goto EXIT;
    }

    ret = OT_AVP_CVE_SUCCESS;
EXIT:
    fclose(fd);
    return ret;
}

ot_s32 ot_avp_cve_img_read_file(ot_avp_cve_img *img, const ot_s8* file_path)
{
    struct timeval start, end;
    ot_u64 timer;
    gettimeofday(&start, NULL);

    ot_u8 *tmp_u8 = NULL;
    ot_u8 *tmp_u_u8 = NULL;
    ot_u8 *tmp_v_u8 = NULL;
    ot_u32 h, width;
    ot_s32 ret = OT_FAILURE;
    FILE* fd = fopen(file_path, "rb");
    if (fd == NULL) {
        printf("fopen fail. Not support read image for type:%d, file_path:%s\n", img->type, file_path);
        return ret;
    }else
    {
        printf("open file success,filename = %s\n",file_path);
    }
    ot_u32 img_size = 0;
    switch (img->type) {
        case OT_IMAGE_FORMAT_BITMAP: //OT_BITMAP:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            width = (img->width >> 3) + (img->width % 8 == 0 ? 0 : 1);
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, width, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
                img_size += img->stride[0];
            }
            break;
        case OT_IMAGE_FORMAT_U8C1: //OT_U8C1:
        case OT_IMAGE_FORMAT_S8C1:
        case OT_IMAGE_FORMAT_YUV_400: //OT_YUV_PLANAR_400:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width, 1, fd) != 1) {
                    printf("Read file fail,virt_addr=%p,width=%d\n",tmp_u8,img->width);
                    ret = 101;
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
                img_size += img->stride[0];
            }
            break;
        case OT_IMAGE_FORMAT_U16C1:
        case OT_IMAGE_FORMAT_S16C1:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width * sizeof(ot_u16), 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
                img_size += img->stride[0];
            }
            break;
        case OT_IMAGE_FORMAT_YUV_422_SEMIPLANAR: //OT_YUV_SEMIPLANAR_422:
        case OT_IMAGE_FORMAT_YVU_422_SEMIPLANAR: //OT_YVU_SEMIPLANAR_422:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
                img_size += img->stride[0];
            }
            tmp_u8 = (ot_u8 *)img->virt_addr[1];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[1];
                img_size += img->stride[1];
            }
            break;
        case OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR: //OT_YUV_SEMIPLANAR_420:
        case OT_IMAGE_FORMAT_YVU_420_SEMIPLANAR: //OT_YVU_SEMIPLANAR_420:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
                img_size += img->stride[0];
            }
            tmp_u8 = (ot_u8 *)img->virt_addr[1];
            for (h = 0; h < img->height / 2; h++) {
                if (fread(tmp_u8, img->width, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[1];
                img_size += img->stride[1];
            }
            break;
        case OT_IMAGE_FORMAT_YUV_422_PLANAR: //OT_YUV_PLANAR_422:
        case OT_IMAGE_FORMAT_YVU_422_PLANAR: //OT_YVU_PLANAR_422:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
                img_size += img->stride[0];
            }
            tmp_u8 = (ot_u8 *)img->virt_addr[1];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width / 2, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[1];
                img_size += img->stride[1];
            }
            tmp_u8 = (ot_u8 *)img->virt_addr[2];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width / 2, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[2];
                img_size += img->stride[2];
            }
            break;
        case OT_IMAGE_FORMAT_YUV_420_PLANAR: //OT_YUV_PLANAR_420:
        case OT_IMAGE_FORMAT_YVU_420_PLANAR: //OT_YVU_PLANAR_420:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
                img_size += img->stride[0];
            }
            tmp_u8 = (ot_u8 *)img->virt_addr[1];
            for (h = 0; h < img->height / 2; h++) {
                if (fread(tmp_u8, img->width / 2, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[1];
                img_size += img->stride[1];
            }
            tmp_u8 = (ot_u8 *)img->virt_addr[2];
            for (h = 0; h < img->height / 2; h++) {
                if (fread(tmp_u8, img->width / 2, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[2];
                img_size += img->stride[2];
            }
            break;
        case OT_IMAGE_FORMAT_BGR_888: //OT_BGR_PACKED_888:
        case OT_IMAGE_FORMAT_RGB_888: //OT_RGB_PACKED_888:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width * 3, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
                img_size += img->stride[0];
            }
            break;
        case OT_IMAGE_FORMAT_BGR_888_PLANAR: //OT_BGR_PLANAR_888:
        case OT_IMAGE_FORMAT_RGB_888_PLANAR: //OT_RGB_PLANAR_888:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            tmp_u_u8 = (ot_u8 *)img->virt_addr[1];
            tmp_v_u8 = (ot_u8 *)img->virt_addr[2];
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u8, img->width, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
                img_size += img->stride[0];
            }
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_u_u8, img->width, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_u_u8 += img->stride[1];
                img_size += img->stride[1];
            }
            for (h = 0; h < img->height; h++) {
                if (fread(tmp_v_u8, img->width, 1, fd) != 1) {
                    printf("Read file fail\n");
                    goto EXIT;
                }
                tmp_v_u8 += img->stride[2];
                img_size += img->stride[2];
            }
            break;
        default:
            printf("Not support read image for type:%d, file_path:%s\n", img->type, file_path);
            ret = 102;
            goto EXIT;
    }
    ot_smr_flush_cache((ot_phys_addr)img->phys_addr[0], (ot_void *)img->virt_addr[0], img_size);
    ret = OT_AVP_CVE_SUCCESS;

EXIT:
    fclose(fd);
    gettimeofday(&end, NULL);
    timer = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("ot_avp_cve_img_read_file file_name = %s, timer = %lld ms.\n", file_path, timer);
    return ret;
}

ot_s32 ot_avp_cve_img_write_file(ot_avp_cve_img *img, const ot_s8* file_path)
{
    struct timeval start, end;
    ot_u64 timer;
    gettimeofday(&start, NULL);

    ot_u8 *tmp_u8 = NULL;
    ot_u8 *tmp_u_u8 = NULL;
    ot_u8 *tmp_v_u8 = NULL;
    ot_u32 h, width;
    ot_u32 ret = OT_FAILURE;
    FILE* fd = fopen(file_path, "wb");
    if (fd == NULL) {
        printf("fopen fail. Not support write image for type:%d, file_path:%s\n", img->type, file_path);
        return ret;
    }

    switch (img->type) {
        case OT_IMAGE_FORMAT_BITMAP: //OT_BITMAP:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            width = (img->width >> 3) + (img->width % 8 == 0 ? 0 : 1);
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            break;
        case OT_IMAGE_FORMAT_U8C1: //OT_U8C1:
        case OT_IMAGE_FORMAT_S8C1: //OT_U8C1:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, img->width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            break;
        case OT_IMAGE_FORMAT_U16C1:
        case OT_IMAGE_FORMAT_S16C1:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, img->width * sizeof(ot_u16), 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            break;
        case OT_IMAGE_FORMAT_U32C1: //OT_U32C1:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, img->width * sizeof(ot_u32), 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            break;
        case OT_IMAGE_FORMAT_U64C1: //OT_U64C1:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, img->width * sizeof(ot_u64), 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            break;
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_UYVY: //OT_UYVY_PACKED_422:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_YUYV: //OT_YUYV_PACKED_422:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_VYUY: //OT_VYUY_PACKED_422:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_YVYU:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_YYUV:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_YYVU:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_UVYY:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_VUYY:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_Y1UY0V:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_Y1VY0U:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_UY1VY0:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_VY1UY0:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_Y1Y0UV:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_Y1Y0VU:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_UVY1Y0:
        case  OT_IMAGE_FORMAT_YUV_422_PACKED_VUY1Y0:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, img->width * 2, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            break;
        case  OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR: //OT_YUV_SEMIPLANAR_420:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            tmp_u_u8 = (ot_u8 *)img->virt_addr[1];
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, img->width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            for (h = 0; h < img->height / 2; h++) {
                if (fwrite(tmp_u_u8, img->width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u_u8 += img->stride[1];
            }

            break;
        case  OT_IMAGE_FORMAT_YUV_422_SEMIPLANAR: //OT_YUV_SEMIPLANAR_422:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            tmp_u_u8 = (ot_u8 *)img->virt_addr[1];
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, img->width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u_u8, img->width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u_u8 += img->stride[1];
            }

            break;
        case  OT_IMAGE_FORMAT_YUV_420_PLANAR: //OT_YUV_PLANAR_420:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            tmp_u_u8 = (ot_u8 *)img->virt_addr[1];
            tmp_v_u8 = (ot_u8 *)img->virt_addr[2];
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, img->width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            for (h = 0; h < img->height / 2; h++) {
                if (fwrite(tmp_u_u8, img->width / 2, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u_u8 += img->stride[1];
            }

            for (h = 0; h < img->height / 2; h++) {
                if (fwrite(tmp_v_u8, img->width / 2, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_v_u8 += img->stride[2];
            }

            break;
        case  OT_IMAGE_FORMAT_YUV_422_PLANAR: //OT_YUV_PLANAR_422:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            tmp_u_u8 = (ot_u8 *)img->virt_addr[1];
            tmp_v_u8 = (ot_u8 *)img->virt_addr[2];
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, img->width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u_u8, img->width / 2, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u_u8 += img->stride[1];
            }

            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_v_u8, img->width / 2, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_v_u8 += img->stride[2];
            }

            break;
        case  OT_IMAGE_FORMAT_RGB_888_PLANAR: //OT_RGB_PLANAR_888:
            tmp_u8 = (ot_u8 *)img->virt_addr[0];
            tmp_u_u8 = (ot_u8 *)img->virt_addr[1];
            tmp_v_u8 = (ot_u8 *)img->virt_addr[2];
            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u8, img->width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u8 += img->stride[0];
            }

            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_u_u8, img->width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_u_u8 += img->stride[1];
            }

            for (h = 0; h < img->height; h++) {
                if (fwrite(tmp_v_u8, img->width, 1, fd) != 1) {
                    printf("write file fail\n");
                    goto EXIT;
                }
                tmp_v_u8 += img->stride[2];
            }

            break;
        default:
            printf("Not support write image for type:%d\n", img->type);
            goto EXIT;
    }
    ret = OT_AVP_CVE_SUCCESS;
EXIT:
    fclose(fd);

    gettimeofday(&end, NULL);
    timer = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("ot_avp_cve_img_write_file file_name = %s, timer = %lld ms.\n", file_path, timer);

    return ret;
}


ot_s32 ot_avp_cve_check_single_planar_img(ot_avp_cve_img *img, ot_avp_cve_img *expect)
{

    ot_u32 size;
    switch (img->type) {
        case OT_IMAGE_FORMAT_U8C1:
        case OT_IMAGE_FORMAT_S8C1:
        case OT_IMAGE_FORMAT_YUV_400:
            size = sizeof(ot_u8);
            break;
        case OT_IMAGE_FORMAT_U16C1:
        case OT_IMAGE_FORMAT_S16C1:
            size = sizeof(ot_u16);
            break;
        case OT_IMAGE_FORMAT_U32C1:
        case OT_IMAGE_FORMAT_S32C1:
            size = sizeof(ot_u32);
            break;
        case OT_IMAGE_FORMAT_U64C1:
        case OT_IMAGE_FORMAT_S64C1:
            size = sizeof(ot_u64);
            break;
        default:
            return OT_FAILURE;
    }

    ot_u32 stride = CVE_ALIGN_UP(img->width *size, OT_AVP_CVE_DEFAULT_ALIGN);
    ot_u32 img_size = img->height * stride;

    //printf("[ot_avp_cve_check_single_planar_img]img_size=%u(size=%u,stride=%u,img->height=%u)\n",img_size,size,stride,img->height);
    //printf("img->stride=%u,img->width=%u,img->height=%u\n",img->stride[0],img->width,img->height);
    //printf("exp->stride=%u,exp->width=%u,exp->height=%u\n",expect->stride[0],expect->width,expect->height);

    ot_s8 *img_buf = (ot_s8 *)img->virt_addr[0];
    ot_s8 *expect_buf = (ot_s8 *)expect->virt_addr[0];
    for (ot_u32 i = 0; i < img_size; i++) {
        if (img_buf[i] != expect_buf[i]) {
            printf("[not equal!!! inxdex=%d] img_buf=%x, expect_buf=%x\n", i,img_buf[i], expect_buf[i]);
            return OT_FAILURE;
        }
    }

    printf("[ot_avp_cve_check_single_planar_img] check success!\n");
    return OT_AVP_CVE_SUCCESS;
}

ot_s32 ot_avp_cve_check_default_img(ot_avp_cve_img *img, ot_avp_cve_img *expect)
{
    ot_u32 img_size = img->capacity;

    ot_s8 *img_buf = (ot_s8 *)img->virt_addr[0];
    ot_s8 *expect_buf = (ot_s8 *)expect->virt_addr[0];
    for (ot_u32 i = 0; i < img_size; i++) {
        if (img_buf[i] != expect_buf[i]) {
            printf("[ot_avp_cve_check_default_img]not equal!!! inxdex=%d,img_buf=%x, expect_buf=%x\n", i,img_buf[i], expect_buf[i]);
            return OT_FAILURE;
        }
    }
    printf("[ot_avp_cve_check_default_img] check success!\n");
    return OT_AVP_CVE_SUCCESS;
}


ot_s32 ot_avp_cve_img_check_result(ot_avp_cve_img *img, ot_avp_cve_img *expect)
{
    struct timeval start, end;
    ot_u64 timer;
    gettimeofday(&start, NULL);
    ot_s32 ret = OT_AVP_CVE_SUCCESS;

    if ((img == OT_NULL || expect == OT_NULL)) {
        return OT_FAILURE;
    }
    // TODO: 参数检查

    //printf("ot_avp_cve_img_check_result type=%d, width=%d, height=%d.\n", img->type, img->width, img->height);

    switch (img->type) {
        case OT_IMAGE_FORMAT_U8C1:
        case OT_IMAGE_FORMAT_S8C1:
        case OT_IMAGE_FORMAT_U16C1:
        case OT_IMAGE_FORMAT_S16C1:
        case OT_IMAGE_FORMAT_U32C1:
        case OT_IMAGE_FORMAT_S32C1:
        case OT_IMAGE_FORMAT_U64C1:
        case OT_IMAGE_FORMAT_S64C1:
        case OT_IMAGE_FORMAT_YUV_400:
            ret = ot_avp_cve_check_single_planar_img(img, expect);
            break;
        // case OT_IMAGE_FORMAT_YUV_420_SEMIPLANAR:
        // case OT_IMAGE_FORMAT_YUV_422_SEMIPLANAR:
        // case OT_IMAGE_FORMAT_YVU_420_SEMIPLANAR:
        // case OT_IMAGE_FORMAT_YVU_422_SEMIPLANAR:
        //     return ot_avp_cve_check_semi_planar_img(img);
        // case OT_IMAGE_FORMAT_YUV_420_PLANAR:
        // case OT_IMAGE_FORMAT_YUV_422_PLANAR:
        // case OT_IMAGE_FORMAT_YVU_420_PLANAR:
        // case OT_IMAGE_FORMAT_YVU_422_PLANAR:
        //     return ot_avp_cve_check_planar_img(img);
        default:
            printf("Not support write image for type:%d.\n", img->type);
            break;
    }

    gettimeofday(&end, NULL);
    timer = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("ot_avp_cve_img_check_result timer = %lld ms.\n", timer);

    return ret;
}

ot_s32 ot_avp_cve_check_result_with_size(ot_mem_info* src, ot_mem_info* dst, ot_u32 compare_size)
{
    if (src == OT_NULL || dst == OT_NULL) {
        printf("[ot_avp_cve_check_result_with_size]src or dst is NULL\n");
        return OT_FAILURE;
    }

    if (src->size < compare_size || dst->size < compare_size)
    {
        printf("[ot_avp_cve_check_result_with_size]size not equal! srcbufferSize=%u,dstbufferSize=%u, compare_size=%u\n",
            src->size, dst->size, compare_size);
        return OT_FAILURE;
    }

    ot_s8 *img_buf = (ot_s8 *)src->virt_addr;
    ot_s8 *expect_buf = (ot_s8 *)dst->virt_addr;
    for (ot_u32 i = 0; i < compare_size; i++) {
        if (img_buf[i] != expect_buf[i]) {
            printf("[not equal!!! inxdex=%d] img_buf=%x, expect_buf=%x\n", i,img_buf[i], expect_buf[i]);
            return OT_FAILURE;
        }
    }
    printf("[ot_avp_cve_check_result_with_size] check success!\n");
    return OT_AVP_CVE_SUCCESS;
}


ot_s32 ot_avp_cve_mem2d_check_result(ot_data_info* src, ot_data_info* dst)
{
    if(src == OT_NULL || dst == OT_NULL)
    {
        printf("[ot_avp_cve_mem1d_check_result]src or dst is NULL\n");
        return OT_FAILURE;
    }

    if(src->capacity != dst->capacity)
    {
        printf("[ot_avp_cve_mem2d_check_result]size not equal! srcbufferSize=%u,dstbufferSize=%u\n",src->capacity,dst->capacity);
        return OT_FAILURE;
    }

    ot_s8 *img_buf = (ot_s8 *)src->virt_addr;
    ot_s8 *expect_buf = (ot_s8 *)dst->virt_addr;
    for (ot_u32 i = 0; i < src->capacity; i++) {
        if (img_buf[i] != expect_buf[i]) {
            printf("[not equal!!! inxdex=%d] img_buf=%x, expect_buf=%x\n", i,img_buf[i], expect_buf[i]);
            return OT_FAILURE;
        }
    }
    printf("[ot_avp_cve_mem2d_check_result] check success!\n");
    return OT_AVP_CVE_SUCCESS;
}




#if 1

ot_s32 txt_to_cve_mem_info(ot_s8 *filename, ot_mem_info *dst, ot_u32 byte_width, ot_u32 size)
{
    ot_s32 ret = 0;
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("txt_to_cve_image_u16c1 fopen fail. file_path:%s\n", filename);
        return ret;
    }
    ot_u8* pdst = (ot_u8*)dst->virt_addr;
    ot_s32 data = 0;
    for (ot_s32 j = 0; j < size; j++) {
        ret = fscanf(fp, "%d", &data);
        if (ret == EOF) {
            break;
        }

        *(ot_u16*)(pdst + byte_width * j) = (ot_u16)data;
    }
    ot_smr_flush_cache((ot_phys_addr)dst->phys_addr, (ot_void *)dst->virt_addr, dst->size);
    fclose(fp);

    return OT_AVP_CVE_SUCCESS;
}

ot_s32 txt_to_cve_image_u16c1(char *filename, ot_avp_cve_img *dst)
{
    ot_s32 ret = 0;
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("txt_to_cve_image_u16c1 fopen fail. file_path:%s\n", filename);
        return ret;
    }
    ot_s32 data = 0;
    ot_s32 width = dst->width;
    ot_s32 height = dst->height;
    ot_u8* pdst = (ot_u8*)dst->virt_addr[0];

    for (ot_s32 i = 0; i < height; i++) {
        for (ot_s32 j = 0; j < width; j++) {
            ret = fscanf(fp, "%d", &data);
            if (ret == EOF) {
                break;
            }

            // 8bit
            *(ot_u16*)(pdst + 2 * j) = (ot_u16)data;
        }
        pdst += dst->stride[1];
    }
    ot_smr_flush_cache((ot_phys_addr)dst->phys_addr[0], (ot_void *)dst->virt_addr[0], dst->capacity);
    fclose(fp);

    return OT_AVP_CVE_SUCCESS;
}

ot_s32 txt_to_cve_image_u8c1(char *filename, ot_avp_cve_img *dst)
{
    ot_s32 ret = 0;
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("txt_to_cve_image_u8c1 fopen fail. file_path:%s\n", filename);
        return ret;
    }
    ot_s32 data = 0;
    ot_s32 width = dst->width;
    ot_s32 height = dst->height;
    ot_u8* pdst = (ot_u8*)dst->virt_addr[0];

    for (ot_s32 i = 0; i < height; i++) {
        for (ot_s32 j = 0; j < width; j++) {
            ret = fscanf(fp, "%d", &data);
            if (ret == EOF) {
                break;
            }

            // 8bit
            *(ot_u8*)(pdst + j) = (ot_u8)data;
        }
        pdst += dst->stride[0];
    }
    ot_smr_flush_cache((ot_phys_addr)dst->phys_addr[0], (ot_void *)dst->virt_addr[0], dst->capacity);
    fclose(fp);

    return OT_AVP_CVE_SUCCESS;
}



#endif


ot_s32 ot_write_test_data_to_file(ot_s8* buff, ot_u32 size, ot_s8 *file_path)
{
    ot_s32 ret = -1;
    FILE* fd = fopen(file_path, "wb");
    if (fd == NULL) {
        printf("fopen fail. file_path:%s\n", file_path);
        return -1;
    }

    if (fwrite(buff, size, 1, fd) != 1) {
        printf("write file fail\n");
        goto EXIT;
    }

    ret = OT_AVP_CVE_SUCCESS;
EXIT:
    fclose(fd);
    return ret;
}
