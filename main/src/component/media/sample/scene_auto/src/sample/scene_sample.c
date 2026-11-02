/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <limits.h>
#include <securec.h>

#include "ot_scenecomm.h"
#include "ot_scene.h"
#include "scene_media.h"
#include "scene_auto.h"

static ot_scene_param g_scene_param = {0};

static ot_bool g_scene_sample_exit = OT_FALSE;
static ot_bool g_scene_auto_running = OT_FALSE;

static ot_void sample_scene_handle_signal(ot_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        g_scene_sample_exit = OT_TRUE;
    }
}

static ot_void sample_register_handler(ot_void)
{
    struct sigaction action = { 0 };
    action.sa_flags = 0;
    action.sa_handler = sample_scene_handle_signal;
    sigaction(SIGINT, &action, OT_NULL);
    sigaction(SIGTERM, &action, OT_NULL);
}

static ot_s32 sample_prompt_and_input_para(ot_char *input, ot_u32 input_len, ot_s32 *choice)
{
    printf("1.scene auto pause\n");
    printf("2.scene auto resume\n");
    printf("3.scene auto refresh ini param\n");
    printf("4.scene auto exit\n");

    *choice = -1;
    (ot_void)fgets(input, input_len, stdin);
    ot_s32 ret = sscanf_s(input, "%d", choice);
    if (ret != 1) {
        printf("choice error\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_create_scene_auto(const ot_char *dir_name, ot_scene_param *scene_param, 
                    ot_scene_mediapipe *scene_mediapipe)
{
    if (g_scene_auto_running) {
        printf("Scene auto is already running.\n");
        return OT_SUCCESS;
    }

    if (dir_name == OT_NULL || scene_mediapipe == OT_NULL || scene_param == OT_NULL) {
        printf("create param and set mode failed, Invalid argument\n");
        return OT_FAILURE;
    }

    ot_s32 ret = ot_scene_create_param(dir_name, scene_param, scene_mediapipe);
    if (ret != OT_SUCCESS) {
        printf("ot_scene_create_param failed\n");
        return OT_FAILURE;
    }

    ret = sample_scene_init(scene_param, scene_mediapipe);
    if (ret != OT_SUCCESS) {
        printf("sample_scene_init failed\n");
        goto exit;
    }

    ret = sample_scene_start_mediapipe();
    if (ret != OT_SUCCESS) {
        printf("sample_scene_start_mediapipe failed\n");
        goto exit;
    }
    
    printf("start mediapipe success\n");

    ret = sample_scene_start_thread();
    if (ret != OT_SUCCESS) {
        printf("sample_scene_start_thread failed\n");
        goto exit;
    }

    printf("The Scene auto is started already.\n");
    g_scene_auto_running = OT_TRUE;
    return OT_SUCCESS;
    
exit:
    printf("Scene auto exit.\n");
    sample_scene_deinit();
    return OT_FAILURE;
}

static ot_s32 sample_refresh_ini(const ot_char *dir_name, ot_scene_param *scene_param, ot_scene_mediapipe *scene_mediapipe)
{
    if (!g_scene_auto_running) {
        printf("Scene auto must be running first.\n");
        return OT_SUCCESS;
    }

    if (dir_name == OT_NULL || scene_mediapipe == OT_NULL) {
        printf("refresh param and set mode failed, Invalid argument\n");
        return OT_FAILURE;
    }

    ot_s32 ret = ot_scene_create_param(dir_name, scene_param, NULL);
    if (ret != OT_SUCCESS) {
        printf("ot_scene_create_param failed\n");
        return OT_FAILURE;
    }

    ret = sample_scene_refresh_param(scene_param);
    if (ret != OT_SUCCESS) {
        printf("sample_scene_refresh_param failed\n");
        goto exit;
    }
    
    printf("Refresh ini param successful.\n");
    return OT_SUCCESS;
    
exit:
    sample_scene_deinit();
    return OT_FAILURE;
}

static ot_s32 sample_scene_pause_auto(ot_void)
{
    if (!g_scene_auto_running) {
        printf("Scene auto must be running first.\n");
        return OT_SUCCESS;
    }

    ot_s32 ret = sample_scene_pause(OT_TRUE);
    if (ret != OT_SUCCESS) {
        printf("sample_scene_pause failed\n");
        return OT_FAILURE;
    }
    printf("The sceneauto is pause.\n");
    return OT_SUCCESS;
}

static ot_s32 sample_scene_resume_auto(ot_void)
{
    if (!g_scene_auto_running) {
        printf("Scene auto must be running first.\n");
        return OT_SUCCESS;
    }

    ot_s32 ret = sample_scene_pause(OT_FALSE);
    if (ret != OT_SUCCESS) {
        printf("sample_scene_pause failed\n");
        return OT_FAILURE;
    }
    printf("The sceneauto is resume.\n");
    return OT_SUCCESS;
}

ot_s32 sample_scene_exit(ot_void)
{
    ot_s32 ret;
    printf("sample_scene_exit.\n");
    if (!g_scene_auto_running) {  
        g_scene_sample_exit = OT_TRUE;     
        goto exit;
    }
    printf("sample_scene_deinit.\n");
    ret = sample_scene_deinit();
    if (ret != OT_SUCCESS) {
        printf("sample_scene_deinit failed\n");
        return OT_FAILURE;
    }

exit:    
    g_scene_sample_exit = OT_TRUE;

    printf("The scene sample is end.\n");
    return OT_SUCCESS;
}

ot_s32 main(ot_s32 argc, ot_char *argv[])
{
    ot_s32 ret, choice;
    ot_char input[10]; /* 10 max input char */
    ot_scene_mediapipe scene_mediapipe = {0};

    ot_scenecomm_check_pointer_return(argv, OT_FAILURE);
    if (argc < 2 || strncmp(argv[1], "-h", 3) == 0) {
        printf("Usage : %s <inidir>\n\t\tfor example :./sample_scene ./param/sensor_os04a10\n", argv[0]);
        return OT_SUCCESS;
    }

    set_dir_name(argv[1]);

    sample_register_handler();
    printf("init success\n");

    ret = sample_create_scene_auto(argv[1], &g_scene_param, &scene_mediapipe);
    ot_scenecomm_check_goto(ret, exit);

    sleep(2);

    while (g_scene_sample_exit == OT_FALSE) {
        ret = sample_prompt_and_input_para(input, sizeof(input), &choice);
        if (ret != OT_SUCCESS) {
            continue;
        }

        switch (choice) {
            case 1: /* user input 1 */
                ret = sample_scene_pause_auto();
                break;
            case 2: /* user input 2 */
                ret = sample_scene_resume_auto();
                break;
            case 3: /* user input 3 */
                ret = sample_refresh_ini(argv[1], &g_scene_param, &scene_mediapipe);
                break;
            case 4: /* user input 4 */
                goto exit;                
            default:
                ret = OT_SUCCESS;
                printf("unknown input\n");
                break;
        }
        ot_scenecomm_check_goto(ret, exit);

        sleep(1);
    }

exit:
    sample_scene_exit();
    g_scene_sample_exit = OT_FALSE;

    return OT_SUCCESS;
}
