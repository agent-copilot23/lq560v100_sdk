#include "sample_cve_main.h"
#include <stdio.h>

/* function : show usage */
static ot_void sample_cve_usage(const ot_s8 *name)
{
    printf("  Usage : %s <index> [input/output file_path] \n", name);
    printf("  index :\n");
    printf("          0) dma sub hist.     [dma sub hist ]  (FILE->CVE->FILE).\n");
#ifndef OT_AVP_CVE_V200
    printf("          1) sobel. (FILE->CVE->FILE).\n");
    printf("          2) canny [mag_ang  thresh]. (FILE->CVE->FILE).\n");
    printf("          3) st lk [corner  lk]. (FILE->CVE ).\n");
    printf("          4) resize [resize]. (FILE->CVE->FILE).\n");
#endif
}


static ot_s32 sample_cve_run_case(int argc, char *argv[])
{
    switch (*argv[1]) {
        case '0' :
            sample_cve_memory(argc, argv);
            break;
#ifndef OT_AVP_CVE_V200
        case '1' :
            sample_cve_sobel(argc, argv);
            break;
        case '2' :
            sample_cve_canny(argc, argv);
            break;
        case '3' :
            sample_cve_st_and_lk(argc, argv);
            break;
        case '4' :
            sample_cve_resize(argc, argv);
            break;
#endif
        default :
            break;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        sample_cve_usage(argv[0]);
        return -1;
    }

    int ret = sample_cve_run_case(argc, argv);

    if (ret == -1) {
        sample_cve_usage(argv[0]);
        return -1;
    }

    return 0;
}
