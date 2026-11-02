
#ifndef OT_AVP_CVE_ALG_LIB_H
#define OT_AVP_CVE_ALG_LIB_H

#include "ot_avp_cve_errno.h"
#include "ot_avp_cve_comm.h"
#include <stdint.h>
#ifndef CVE_ALIGN_UP
#define CVE_ALIGN_UP(x, a)           ((((x) + ((a) - 1)) / a ) * a)
#endif

#ifdef __cplusplus
extern "C"{
#endif
#define cve_convert_addr_to_ptr(type, addr) ((type *)(uintptr_t)(addr))

#define cve_convert_ptr_to_addr(type, addr) ((type)(uintptr_t)(addr))

ot_void *cve_zalloc(ot_u32 size);
ot_void cve_free(ot_void *virt_addr);
ot_u64 cve_virt_to_phys(ot_void *virt_addr);

ot_s32 ot_avp_cve_create_mem_info(ot_mem_info *buf, ot_u32 size);
ot_void ot_avp_cve_destory_mem_info(ot_mem_info *buf);
ot_void ot_avp_cve_set_mem_info(ot_mem_info *buf, ot_u8 val, ot_u32 size);

ot_s32 ot_avp_cve_create_data_info(ot_data_info *buf, ot_u32 width, ot_u32 height);
ot_void ot_avp_cve_destory_data_info(ot_data_info *buf);

ot_s32 ot_avp_cve_create_data_info(ot_data_info *buf, ot_u32 width, ot_u32 height);
ot_void ot_avp_cve_destory_data_info(ot_data_info *buf);


ot_s32 ot_mem_info_read_file(ot_mem_info* mem_info, const ot_s8 *file_path);
ot_s32 ot_mem_info_write_file(ot_mem_info* mem_info, const ot_s8 *file_path);

ot_s32 ot_data_info_write_file(ot_data_info* data_info, const ot_s8 *file_path);
ot_s32 ot_data_info_read_file(ot_data_info* data_info, const ot_s8 *file_path);

ot_s32 ot_avp_cve_create_img(ot_avp_cve_img *img, ot_image_pixel_format img_type, ot_u32 width, ot_u32 height);
ot_void ot_avp_cve_destory_img(ot_avp_cve_img *img);

ot_s32 ot_avp_cve_img_read_file(ot_avp_cve_img *img, const ot_s8 *file_path);
ot_s32 ot_avp_cve_img_write_file(ot_avp_cve_img *img, const ot_s8 *file_path);


ot_s32 txt_to_cve_mem_info(ot_s8 *filename, ot_mem_info *dst, ot_u32 byte_width, ot_u32 size);

ot_s32 txt_to_cve_image_u8c1(char *filename, ot_avp_cve_img *dst);
ot_s32 txt_to_cve_image_u16c1(char *filename, ot_avp_cve_img *dst);

ot_s32 ot_write_test_data_to_file(ot_s8* buff, ot_u32 size, ot_s8 *file_path);

#ifdef __cplusplus
}
#endif

#endif