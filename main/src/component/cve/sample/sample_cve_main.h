#ifndef SAMPLE_CVE_MAIN_H
#define SAMPLE_CVE_MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "sample_cve_common.h"

#ifndef OT_AVP_CVE_V200
#include "ot_eis_type.h"
#else
#include "ot_type.h"
#endif
#include "ot_avp_cve_alg_lib.h"
#include "ot_avp_cve.h"
#include "ot_avp_cve_comm.h"
#include "ot_avp_cve_errno.h"

ot_void sample_cve_memory(int argc, char *argv[]);
ot_void sample_cve_sobel(int argc, char *argv[]);
ot_void sample_cve_resize(int argc, char *argv[]);
ot_void sample_cve_st_and_lk(int argc, char *argv[]);
ot_void sample_cve_canny(int argc, char *argv[]);
#endif
