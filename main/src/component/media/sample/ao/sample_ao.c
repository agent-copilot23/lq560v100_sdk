/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "ot_smr.h"
#include "ot_vrb.h"
#include "ot_buffer_pool.h"
#include "ot_eis_media_ao.h"
#include "sample_comm.h"
#include "ot_tlv320aic3101_ioctl.h"

typedef enum {
    OT_EIS_PT_AAC           = 37,
    OT_EIS_PT_WMA9STD       = 38,
    OT_EIS_PT_HEAAC         = 39,
    OT_EIS_PT_PCM_VOICE     = 40,
    OT_EIS_PT_PCM_AUDIO     = 41,
    OT_EIS_PT_MP3           = 43,
    OT_EIS_PT_OPUS          = 1007,
    OT_EIS_PT_BUTT
} ot_eis_payload_type;

typedef struct {
    ot_buffer_pool_blk pool_blk;
    ot_void* virt_addr;
}ao_pool_blk;

#define SAMPLE_AUDIO_AI_USER_FRAME_DEPTH 5
#define AO_PCM_PER_FRAME_SIZE (2048)
#define AO_POOL_BLK_RECYCLE_THREOLD (200)
#define AO_FILE_NAME "/pcm/ocean.pcm"

static ot_eis_payload_type g_payload_type = OT_EIS_PT_AAC;
static ot_eis_handle pool_hdl;
static ao_pool_blk pool_blk[AO_POOL_BLK_RECYCLE_THREOLD] = {0};
static int blk_cnt = 0;
static int exit_flag = 0;

static ot_void sample_ao_init_param(ot_eis_ao_attr *ao_attr, ot_s32 *ao_dev)
{
    ao_attr->sample_rate  = OT_EIS_AUDIO_SAMPLE_RATE_44100;
    ao_attr->bit_width    = OT_EIS_AUDIO_BIT_WIDTH_16;
    ao_attr->work_mode    = OT_EIS_AO_MODE_I2S_MASTER;
    ao_attr->snd_mode     = OT_EIS_AUDIO_SOUND_MODE_STEREO;
    ao_attr->frame_num    = 30;
    if (g_payload_type == OT_EIS_PT_AAC) {
        ao_attr->point_num_per_frame = 1024;
    } else if (g_payload_type == OT_EIS_PT_MP3) {
        ao_attr->point_num_per_frame = 1152;
    } else if (g_payload_type == OT_EIS_PT_OPUS) {
        ao_attr->point_num_per_frame = ao_attr->sample_rate / 100;
    } else {
        ao_attr->point_num_per_frame = 480;
    }
    ao_attr->chn_cnt      = 2;

    *ao_dev = 0;
    ao_attr->i2s_type   = OT_EIS_AO_I2STYPE_INNERCODEC;
}

static ot_s32 sample_ao_smr_init()
{
    ot_s32 ret;
    ot_video_buffer_attr vrb_cfg = {0};
    ot_video_buffer_attr attr = {0};

    sample_common_get_vrb_cfg(2, 2048, 2, &vrb_cfg);
    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    ret = ot_smr_init();
    if(ret != OT_SUCCESS){
        printf("sample ao smr init error! ret[0x%x]\n", ret);
        return ret;
    }

    ret = ot_buffer_pool_init();
    if(ret != OT_SUCCESS){
        printf("sample ao buf pool init error! ret[0x%x]\n", ret);
        return ret;
    }

    attr.cnt = 1;
    attr.buf_blks[0].cnt = 300;
    attr.buf_blks[0].size = AO_PCM_PER_FRAME_SIZE * 2;
    ret = ot_buffer_pool_create(&pool_hdl, &attr);
    if(ret != OT_SUCCESS){
        printf("sample ao create fail! ret[0x%x]\n", ret);
        return ret;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_ao_smr_deinit()
{
    ot_s32 ret;

    ret = ot_buffer_pool_destroy(pool_hdl);
    if(ret != OT_SUCCESS){
        printf("sample ao destroy fail!\n");
        return ret;
    }

    ret = ot_buffer_pool_deinit();
    if(ret != OT_SUCCESS){
        printf("sample ao buf pool deinit error! ret[0x%x]\n", ret);
        return ret;
    }

    ret = ot_smr_deinit();
    if(ret != OT_SUCCESS){
        printf("sample ao smr deinit error! ret[0x%x]\n", ret);
        return ret;
    }

    return OT_SUCCESS;
}


static ot_s32 sample_ao_init(ot_void)
{
    ot_s32 ret;

    ret = sample_ao_smr_init();
    if(ret != OT_SUCCESS){
        return ret;
    }

    ot_eis_ao_deinit();

    ret = ot_eis_ao_init();
    if(ret != OT_SUCCESS){
        printf("sample ao init fail! ret[0x%x]\n", ret);
        return ret;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_ao_deinit(ot_void)
{

    ot_eis_ao_deinit();

    sample_comm_sys_exit();

    return OT_SUCCESS;
}

/* stop ao */
ot_s32 sample_ao_stop(ot_s32 ao_dev_id, ot_s32 ao_chn_cnt)
{
   ot_s32 i;
   ot_s32 ret;

   for (i = 0; i < (ot_s32)ao_chn_cnt; i++) {
       ret = ot_eis_ao_disable_chn(ao_dev_id, i);
       if (ret != OT_SUCCESS) {
           printf("%s: sample ao_disable_chn failed with %#x!\n", __FUNCTION__, ret);
           return ret;
       }
   }

   ret = ot_eis_ao_disable_chn(ao_dev_id, 8);
   if (ret != OT_SUCCESS) {
       printf("%s: sample ao_disable_chn(%d) failed with %#x!\n", __FUNCTION__, i, ret);
       return OT_FAILURE;
   }

   ret = ot_eis_ao_disable(ao_dev_id);
   if (ret != OT_SUCCESS) {
       printf("%s: ao_disable failed with %#x!\n", __FUNCTION__, ret);
       return ret;
   }

   return OT_SUCCESS;
}

static ot_s32 sample_ao_start(ot_s32 ao_dev_id, ot_s32 ao_chn_cnt, ot_eis_ao_attr *ao_attr,
                           ot_eis_audio_sample_rate in_sample_rate)
{
    ot_s32 i;
    ot_s32 ret;
    ot_u32 chn_cnt;
    ot_s32 volume_db;

    ret = ot_eis_ao_set_pub_attr(ao_dev_id, ao_attr);
    if (ret != OT_SUCCESS) {
        printf("%s: ao_set_pub_attr(%d) failed with %#x!\n", __FUNCTION__, ao_dev_id, ret);
        return OT_FAILURE;
    }

    ret = ot_eis_ao_enable(ao_dev_id);
    if (ret != OT_SUCCESS) {
        printf("%s: ao_enable(%d) failed with %#x!\n", __FUNCTION__, ao_dev_id, ret);
        return OT_FAILURE;
    }

    ret = ot_eis_ao_get_volume(ao_dev_id, &volume_db);
    if(ret != OT_SUCCESS){
        printf("sample ao get volume fail!\n");
        return ret;
    }
    printf("sample ao get volume[%d]\n", volume_db);

    ret = ot_eis_ao_set_volume(ao_dev_id, volume_db/2);
    if(ret != OT_SUCCESS){
        printf("sample ao set volume fail!\n");
        return ret;
    }
    printf("sample ao set volume[%d]\n", volume_db/2);

    chn_cnt = ao_chn_cnt >> ((ot_u32)ao_attr->snd_mode);
    for (i = 0; i < (ot_s32)chn_cnt; i++) {
        ret = ot_eis_ao_enable_chn(ao_dev_id, i);
        if (ret != OT_SUCCESS) {
            printf("%s: ao_enable_chn(%d) failed with %#x!\n", __FUNCTION__, i, ret);
            return OT_FAILURE;
        }
    }

    ret = ot_eis_ao_enable_chn(ao_dev_id, 0);
    if (ret != OT_SUCCESS) {
        printf("%s: ao_enable_chn(%d) failed with %#x!\n", __FUNCTION__, i, ret);
        return OT_FAILURE;
    }


    ret = ot_eis_ao_enable_chn(ao_dev_id, 8);
    if (ret != OT_SUCCESS) {
        printf("%s: ao_enable_chn(%d) failed with %#x!\n", __FUNCTION__, i, ret);
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

static ot_void ao_interleave_16bit(const ot_s16 *src, ot_s16 *dest_left, ot_s16 *dest_right, ot_u32 samples)
{
    ot_u32 i;

    if ((src == NULL) || (dest_left == NULL) || (dest_left == NULL)) {
        return;
    }

    for (i = 0; i < samples; i++) {
        *dest_left = src[2 * i]; /* 2: 2chn */
        *dest_right = src[2 * i + 1]; /* 2: 2chn */
        dest_left++;
        dest_right++;
    }
}

static ot_void sample_ao_release_pool_blk()
{
    ot_s32 ret;
    int i = 0;

    for(i =0; i< blk_cnt;i++){

        ret = ot_smr_munmap(pool_blk[i].virt_addr, AO_PCM_PER_FRAME_SIZE * 2);
        if (ret != OT_SUCCESS){
            printf("sample smr munmap fail!\n");
            return;
        }

        ret = ot_buffer_pool_blk_release(pool_hdl, &pool_blk[i].pool_blk);
        if(ret != OT_SUCCESS){
            printf("sample ao release blk fail!\n");
            return;
        }

    }

    blk_cnt = 0;
    memset(&pool_blk, 0, sizeof(ao_pool_blk)*AO_POOL_BLK_RECYCLE_THREOLD);
}

static ot_s32 sample_ao_get_set_volume(ot_s32 fd_acodec)
{
    ot_s32 ret;
    ot_acodec_volume_ctrl volume;

    if (fd_acodec < 0) {
        printf("%s: audio codec fd invalid,%s\n", __FUNCTION__, EIS_ACODEC_FILE);
        return OT_FAILURE;
    }

    ret = ioctl(fd_acodec, OT_ACODEC_VOLUME_GET, &volume);
    if (ret) {
        printf("%s: get volume failed ret = %d\n", __FUNCTION__, ret);
        return OT_FAILURE;
    }

    printf("get volume ok: volume = %d mute = %d.\n", volume.volume, volume.volume_ctrl_mute);
    volume.volume += 20;
    ret = ioctl(fd_acodec, OT_ACODEC_VOLUME_SET, &volume);
    if (ret) {
        printf("%s: set volume failed ret = %d\n", __FUNCTION__, ret);
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_ao_get_and_send_frame(ot_s32 ao_dev_id, ot_s32 ao_chn_id)
{
    ot_eis_audio_frame frame = {0};
    FILE *file;
    ot_s32 ret;
    ot_void* virt_addr;
    ot_s16* virt_tmp;
    ot_u64 timestamp = 548636895208;
    ot_u32 frame_seq = 0;
    ot_buffer_pool_blk blk = {0};
    ot_char path[256];
    if (getcwd(path, sizeof(path)) == NULL) {
        printf("get current working directory error!\n");
        return OT_FAILURE;
    }
    strcat(path, AO_FILE_NAME);

    printf("start send frame\n");

    file = fopen(path, "rb");
    if (!file) {
        printf("pcm cannot open ,sample send frame fail!\n");
        return OT_FAILURE;
    }

    virt_tmp = (ot_s16 *)malloc(AO_PCM_PER_FRAME_SIZE * 2);
    if(virt_tmp == NULL){
        printf("malloc err!\n");
        ret = OT_FAILURE;
        goto file_close;
    }

    ret = ot_buffer_pool_blk_acquire(pool_hdl, AO_PCM_PER_FRAME_SIZE * 2, &blk);
    if(ret != OT_SUCCESS){
        printf("sample ao get blk ,alloc fail!\n");
        goto mem_free;
    }

    ret = ot_smr_mmap(blk.phys_addr, AO_PCM_PER_FRAME_SIZE * 2, OT_FALSE, &virt_addr);
    if(ret != OT_SUCCESS){
        printf("sample ao mmap fail!\n");
        (void)ot_buffer_pool_blk_release(pool_hdl, &blk);
        goto mem_free;
    }

    pool_blk[blk_cnt].pool_blk.phys_addr = blk.phys_addr;
    pool_blk[blk_cnt].virt_addr = virt_addr;
    blk_cnt++;

    while (fread(virt_tmp, sizeof(ot_s16), AO_PCM_PER_FRAME_SIZE, file) == AO_PCM_PER_FRAME_SIZE) {
        frame.bit_width = OT_EIS_AUDIO_BIT_WIDTH_16;
        frame.snd_mode = OT_EIS_AUDIO_SOUND_MODE_STEREO;

        frame.time_stamp = timestamp;
        frame.seq = frame_seq;
        frame.len = AO_PCM_PER_FRAME_SIZE;

        frame.virt_addr[0] = (ot_u8 *)virt_addr;
        frame.pool_hdl[0] = pool_hdl;

        frame.virt_addr[1] = ((ot_u8 *)virt_addr + AO_PCM_PER_FRAME_SIZE);
        frame.pool_hdl[1] = pool_hdl;

        ao_interleave_16bit((ot_s16 *)virt_tmp, (ot_s16 *)(frame.virt_addr[0]),
            (ot_s16 *)(frame.virt_addr[1]), frame.len / sizeof(ot_s16));

        frame_seq++;

        ret = ot_eis_ao_send_frame(ao_dev_id, ao_chn_id, &frame, 1000);
        if (ret != OT_SUCCESS){
            printf("sample ao send frame fail!\n");
            goto mem_free;
        }


        if(blk_cnt >= AO_POOL_BLK_RECYCLE_THREOLD){
            sample_ao_release_pool_blk();
        }

        if(exit_flag == 1) {
            break;
        }

        ret = ot_buffer_pool_blk_acquire(pool_hdl, AO_PCM_PER_FRAME_SIZE * 2, &blk);
        if(ret != OT_SUCCESS){
            printf("sample ao get blk ,alloc fail!\n");
            goto mem_free;
        }

        ret = ot_smr_mmap(blk.phys_addr, AO_PCM_PER_FRAME_SIZE * 2, OT_FALSE, &virt_addr);
        if(ret != OT_SUCCESS){
            printf("sample ao mmap fail!\n");
            (void)ot_buffer_pool_blk_release(pool_hdl, &blk);
            goto mem_free;
        }

        pool_blk[blk_cnt].pool_blk.phys_addr = blk.phys_addr;
        pool_blk[blk_cnt].virt_addr = virt_addr;
        blk_cnt++;
    }

mem_free:
    free(virt_tmp);
file_close:
    fclose(file);
    return ret;
}

ot_s32 sample_ao_codec_cfg_audio(const ot_eis_ao_attr *ao_attr)
{
    ot_s32 fd_acodec = -1;
    ot_ao_codec_attr config;
    ot_s32 ret;

    fd_acodec = open(EIS_ACODEC_FILE, O_RDWR);
    if (fd_acodec < 0) {
        printf("%s: can't open audio codec,%s\n", __FUNCTION__, EIS_ACODEC_FILE);
        return OT_FAILURE;
    }

    if (ioctl(fd_acodec, OT_ACODEC_SOFT_RESET_CTRL)) {
        printf("reset audio codec error\n");
    }

    config.volume_ctrl.volume = 0x0;
    config.volume_ctrl.volume_ctrl_mute = 0x0;
    config.ao_attr = *ao_attr;
    ret = ioctl(fd_acodec, OT_ACODEC_AO_CONFIG, &config);
    if (ret) {
        printf("%s: set acodec sample rate failed ret = %d\n", __FUNCTION__, ret);
        return OT_FAILURE;
    }

    sample_ao_get_set_volume(fd_acodec);

    printf("set inner audio codec ok: sample_rate = %d.\n", ao_attr->sample_rate);

    close(fd_acodec);
    return OT_SUCCESS;
}


/* config codec */
static ot_s32 sample_ao_audio_cfg_acodec(const ot_eis_ao_attr *ao_attr)
{
    ot_bool codec_cfg = OT_FALSE;

    ot_s32 ret = sample_ao_codec_cfg_audio(ao_attr);
    if (ret != OT_SUCCESS) {
        printf("%s:sample ao _inner_codec_cfg_audio failed\n", __FUNCTION__);
        return ret;
    }
    codec_cfg = OT_TRUE;

    if (codec_cfg == OT_FALSE) {
        printf("can not find the right codec.\n");
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

static ot_void sample_ao_handle_sig(ot_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        exit_flag = 1;
    }
}

static ot_void sample_register_sig_handler(ot_void (*sig_handle)(ot_s32))
{
    struct sigaction sa;

    (ot_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handle;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, OT_NULL);
    sigaction(SIGTERM, &sa, OT_NULL);
}

ot_s32 main()
{
    ot_s32 ret;
    const ot_s32 ao_chn = 0;
    ot_s32 ao_dev;
    ot_eis_ao_attr ao_attr;

    ret = sample_ao_init();
    if(ret != 0){
        return ret;
    }

    sample_ao_init_param(&ao_attr, &ao_dev);

    ret = sample_ao_start(ao_dev, ao_attr.chn_cnt, &ao_attr, OT_EIS_AUDIO_SAMPLE_RATE_44100);
    if(ret != OT_SUCCESS){
        printf("sample ao start fail!\n");
        goto exit2;
    }

    ret = sample_ao_audio_cfg_acodec(&ao_attr);
    if(ret != OT_SUCCESS){
        printf("sample ao codec cfg fail! ret[0x%x]\n", ret);
        goto exit1;
    }

    sample_register_sig_handler(sample_ao_handle_sig);

    ret = sample_ao_get_and_send_frame(ao_dev, ao_chn);
    if(ret != OT_SUCCESS){
        printf("sample send frame fail!\n");
        goto exit1;
    }

exit1:
    sample_ao_stop(ao_dev, ao_attr.chn_cnt);
exit2:
    sample_ao_smr_deinit();
    sample_ao_deinit();

    if(exit_flag == 0) {
        printf("\033[0;32msample ao exit normally!\033[0;39m\n");
    } else {
        printf("\033[0;31msample ao exit abnormally!\033[0;39m\n\n");
    }
}
