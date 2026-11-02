#include <stdio.h>
#include "ot_eis_media_sys.h"


static void case1()
{
    int ret = -1;
    ot_eis_sdk_version ver;
    ot_u64 cur_pts;
    ot_eis_media_log_level log_level;
    ret =ot_eis_media_init();
    if(ret != 0){
        printf("media init fail! ret_code[0x%x]\n", ret);
        return;
    }

    ret = ot_eis_media_get_version(&ver);
    if(ret != 0){
        printf("media get_version fail! ret_code[0x%x]\n", ret);
        return;
    }
    printf("get_version [%s]\n", ver.version);

    ret = ot_eis_media_get_cur_pts(&cur_pts);
    if(ret != 0){
        printf("media get_pts fail! ret_code[0x%x]\n", ret);
        return;
    }
    printf("get_pts [0x%llx]\n", cur_pts);

    ret =ot_eis_media_deinit();
    if(ret != 0){
        printf("media init fail! ret_code[0x%x]\n", ret);
        return;
    }

    log_level.mod_id = OT_EIS_MODULE_VI;
    ret = ot_eis_media_log_level_get(&log_level);
    if(ret != 0){
        printf("media get log level fail! ret_code[0x%x]\n", ret);
        return;
    }
    printf("start media get log level success! level[%d]\n", log_level.level);

    log_level.mod_id = OT_EIS_MODULE_VI;
    log_level.level = OT_EIS_MEDIA_LOG_DEBUG;
    ret = ot_eis_media_log_level_set(&log_level);
    if(ret != 0){
        printf("media set log level fail! ret_code[0x%x]\n", ret);
        return;
    }
    printf("set 7 media set log level success! level[%d]\n", log_level.level);

    log_level.mod_id = OT_EIS_MODULE_VI;
    ret = ot_eis_media_log_level_get(&log_level);
    if(ret != 0){
        printf("media get log level fail! ret_code[0x%x]\n", ret);
        return;
    }
    printf("after set media get log level success! level[%d]\n", log_level.level);

    ret =ot_eis_media_deinit();
    if(ret != 0){
        printf("media deinit fail! ret_code[0x%x]\n", ret);
        return;
    }

    printf("case 1 end!\n");
}

static void case2()
{
    ot_s32 ret = -1;
    ot_s32 time_zone = 0;
    ot_eis_vi_vproc_mode mode = {0};
    ot_s32 connect = 0;

    printf("case2 start!\n");

	ret =ot_eis_media_init();
    if(ret != 0){
        printf("media init fail! ret_code[0x%x]\n", ret);
        return;
    }

    ret = ot_eis_media_get_time_zone(&time_zone);
    if(ret != 0 ){
        printf("media get time zone fail! ret_code[0x%x]\n", ret);
        return;
    }
    printf("default time_zone[%d]\n", time_zone);

    time_zone = 111;
    ret = ot_eis_media_set_time_zone(time_zone);
    if(ret != 0 ){
        printf("media set time zone fail! ret_code[0x%x]\n", ret);
        return;
    }
    printf("set time_zone[%d]\n", time_zone);

    ret = ot_eis_media_get_time_zone(&time_zone);
    if(ret != 0 ){
        printf("after set 111 media get time zone fail! ret_code[0x%x]\n", ret);
        return;
    }
    printf("set 111 time_zone[%d]\n", time_zone);

    ret = ot_eis_media_get_vi_vproc_mode(&mode);
    if(ret != 0 ){
        printf("media get vi_vproc_mode fail! ret_code[0x%x]\n", ret);
        return;
    }
    printf("default vi_vproc_mode[%d]\n", mode.mode[0]);

    mode.mode[0] = OT_EIS_VI_ONLINE_VPROC_OFFLINE;
    ret = ot_eis_media_set_vi_vproc_mode(&mode);
    if(ret != 0 ){
        printf("media set vi_vproc_mode fail! ret_code[0x%x] mode[%d]\n", ret, OT_EIS_VI_ONLINE_VPROC_OFFLINE);
        //return;
    }
    printf("set mode 2\n");

    ret = ot_eis_media_get_vi_vproc_mode(&mode);
    if(ret != 0 ){
        printf("media get vi_vproc_mode fail! ret_code[0x%x]\n", ret);
        return;
    }
    printf("set 2 vi_vproc_mode[%d]\n", mode.mode[0]);

    ret =ot_eis_media_deinit();
    if(ret != 0){
        printf("media deinit fail! ret_code[0x%x]\n", ret);
        return;
    }


//    ret = ot_eis_media_get_tuning_connect(&connect);
//    if(ret != 0 ){
//        printf("media get tuning_connect fail! ret_code[0x%x]\n", ret);
//        return;
//    }
//    printf("default tuning_connect[%d]\n", connect);
//
//    connect = 1;
//    ret = ot_eis_media_set_tuning_connect(connect);
//    if(ret != 0 ){
//        printf("media set tuning_connect fail! ret_code[0x%x]\n", ret);
//        return;
//    }
//    printf("set tuning_connect 1\n");
//
//    ret = ot_eis_media_get_tuning_connect(&connect);
//    if(ret != 0 ){
//        printf("media get tuning_connect fail! ret_code[0x%x]\n", ret);
//        return;
//    }
//    printf("set 2 tuning_connect[%d]\n", connect);

}


int main()
{
    case1();
    case2();
    return 0;
}


