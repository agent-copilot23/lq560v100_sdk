#include "sample_cve_main.h"
#include "ot_avp_cve.h"
#include "ot_avp_cve_comm.h"
#include "ot_avp_cve_errno.h"
#include "ot_smr.h"
#ifndef OT_AVP_CVE_V200
#include "ot_eis_media_sys.h"
#endif

ot_s32 sample_cve_init()
{
    ot_avp_cve_status_e status = OT_AVP_CVE_SUCCESS;
    // init cve
#ifndef OT_AVP_CVE_V200
    status = ot_eis_media_deinit();
    status = ot_eis_media_init();
    if (status != OT_AVP_CVE_SUCCESS) {
        printf("failed to init eis_media, status[%d]", status);
        return -1;
    }
#else
    ot_smr_attr smr_attr = {0};
    const char *region_name = (const char *)"anony";

    strcpy((char *)smr_attr.regions[0].region_name, region_name);
    smr_attr.regions[0].addr = (ot_phys_addr)0x80000000;
    smr_attr.regions[0].len = 0x40000000;  // 256M * 4 = 1G
    smr_attr.allocator = OT_SMR_ALLOCATOR_BBAA;
    smr_attr.num = 1;

    if (ot_smr_config(&smr_attr) != 0) {
        printf("SMR confg already!\n");
    }
#endif
    status = ot_avp_cve_init();
    if (status != OT_AVP_CVE_SUCCESS) {
        printf("failed to init cve, status[%d]", status);
        return -1;
    }

    // init smr
    if (ot_smr_init() != 0) {
        printf("SMR init error!\n");
        return -1;
    }
    printf("Sample_cve_init Success!\n");
    return status;
}

ot_void sample_cve_deinit()
{
    // deinit cve
    ot_avp_cve_status_e status = OT_AVP_CVE_SUCCESS;

    // deinit smr
    if (ot_smr_deinit() != 0) {
        printf("ot_smr_deinit fail.\n");
        return;
    }

    status = ot_avp_cve_deinit();
    if (status != OT_AVP_CVE_SUCCESS) {
        printf("failed to deinit cve, status[%d]", status);
        return;
    }

    printf("Sample_cve_deinit Success.\n");
}
