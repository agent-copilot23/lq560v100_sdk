/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <sys/prctl.h>
#include <unistd.h>
#include "sample_comm.h"

#define VDEC_SECOND 1000000

ot_eis_vdec_chn_handle g_chnhdl = (ot_eis_vdec_chn_handle)(-1);

ot_void sample_comm_vdec_send_stream_proc(vdec_thread_param *thread_param,
    ot_eis_vdec_stream *stream, ot_bool end_of_stream)
{
    ot_s32 ret;
    do {
        ret = ot_eis_vdec_chn_send_stream(thread_param->chn_hdl, stream, -1);
        if (ret != OT_SUCCESS)
            usleep(thread_param->interval_time);
        else
            break;
    }while(thread_param->e_thread_ctrl == THREAD_CTRL_START);
}

ot_void sample_comm_vdec_handle_send_stream(vdec_thread_param *thread_param, ot_eis_vdec_stream *stream,
    ot_bool *end_of_stream, ot_u64 *pts)
{
    ot_u64 cur_time;

    while (1) {
        if (thread_param->e_thread_ctrl != THREAD_CTRL_START) {
            break;
        }
        //get current pts
        ot_eis_media_get_cur_pts(&cur_time);

        //ensure that the stream is sent accurately according to fps. (1ms)
        if ((thread_param->last_time == 0) ||
            ((cur_time - thread_param->last_time) >= (VDEC_SECOND / thread_param->fps - thread_param->time_gap))) {
            sample_comm_vdec_send_stream_proc(thread_param, stream, *end_of_stream);
            *end_of_stream = OT_FALSE;
            *pts += thread_param->pts_increase;
            if (thread_param->last_time != 0) {
                thread_param->time_gap =
                    ((cur_time - thread_param->last_time) >= (VDEC_SECOND / thread_param->fps)) ?
                    (cur_time - thread_param->last_time - (VDEC_SECOND / thread_param->fps)) : 0;
                thread_param->time_gap = (thread_param->time_gap > (VDEC_SECOND / thread_param->fps)) ?
                    (VDEC_SECOND / thread_param->fps) : thread_param->time_gap;
            }

            thread_param->last_time = cur_time;
            break;
        } else {
            usleep(thread_param->interval_time);    //休眠1ms
        }
    }
    return;
}
ot_void sample_comm_vdec_send_EOS(vdec_thread_param *thread_param)
{
    ot_eis_vdec_stream ot_stream;
    (ot_void)memset_s(&ot_stream, sizeof(ot_eis_vdec_stream), 0, sizeof(ot_eis_vdec_stream));
    ot_stream.end_of_stream = OT_TRUE;
    ot_eis_vdec_chn_send_stream(thread_param->chn_hdl, &ot_stream, -1);  
}
ot_void sample_comm_vdec_send_stream_process(vdec_thread_param *thread_param, FILE *fp_strm, ot_u8 *buf, ot_u64 *frame_lens, ot_u64 frame_cnt)
{
    ot_bool end_of_stream;
    ot_s32 read_len;
    ot_u64 pts = thread_param->pts_init;
    ot_u64 read_cnt = 0;
    ot_eis_vdec_stream stream;

    thread_param->last_time = 0;
    thread_param->time_gap = 0;
    while (1) {
        if (thread_param->e_thread_ctrl == THREAD_CTRL_STOP) {
            break;
        } else if (thread_param->e_thread_ctrl == THREAD_CTRL_PAUSE) {
            sleep(1);
            continue;
        }
        // read a frame of stream data from stream file
        end_of_stream = OT_FALSE;
        read_len = fread(buf, 1, frame_lens[read_cnt], fp_strm);
        read_cnt++;

        // send to vdec
        stream.pts = pts;
        stream.stream_virt_addr = buf;
        stream.stream_len = read_len;
        stream.end_of_stream = OT_FALSE;
        sample_comm_vdec_handle_send_stream(thread_param, &stream, &end_of_stream, &pts);

        // updata read_cnt
        if (read_cnt == frame_cnt) {
            if (thread_param->circle_send == OT_TRUE) {
                sample_comm_vdec_send_EOS(thread_param);
                fseek(fp_strm, 0, SEEK_SET);
                read_cnt = 0;
            } else {
                break;
            }
        }
    }
    return;
}

ot_s32 sample_comm_vdec_check_send_stream_param(vdec_thread_param *thread_param,
    ot_char *c_stream_file, ot_char *c_bin_file,ot_u32 arr_len)
{
    if (thread_param->c_file_path == OT_NULL || thread_param->c_file_name == OT_NULL) {
        sample_print("chn %d stream file path or stream file name is NULL\n", (ot_s32)((intptr_t)thread_param->chn_hdl));
        return OT_FAILURE;
    }

    if (arr_len <= 1) {
        sample_print("chn %d arr length might be overflow\n", (ot_s32)((intptr_t)thread_param->chn_hdl));
        return OT_FAILURE;
    }

    if (snprintf_s(c_stream_file, arr_len, arr_len - 1, "%s/%s", thread_param->c_file_path,
        thread_param->c_file_name) < 0) {
        sample_print("chn %d config stream file failed!\n", (ot_s32)((intptr_t)thread_param->chn_hdl));
        return OT_FAILURE;
    }

    if (snprintf_s(c_bin_file, arr_len, arr_len - 1, "%s/%s.bin", thread_param->c_file_path,
        thread_param->c_file_name) < 0) {
        sample_print("chn %d config stream file failed!\n", (ot_s32)((intptr_t)thread_param->chn_hdl));
        return OT_FAILURE;
    }

    if (thread_param->min_buf_size <= 0) {
        sample_print("chn %d min_buf_size should greater than zero!\n", (ot_s32)((intptr_t)thread_param->chn_hdl));
        return OT_FAILURE;
    }

    if (thread_param->fps <= 0 || thread_param->fps > 300) { /* 0~300:frame rate limit */
        sample_print("chn %d fps should be [1, 300]!\n", (ot_s32)((intptr_t)thread_param->chn_hdl));
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

//bin文件中，每一帧码流的长度信息使用8byte来表示。
ot_void *sample_comm_vdec_send_stream(ot_void *args)
{
    vdec_thread_param *thread_param = (vdec_thread_param *)args;
    ot_u8 *buf = OT_NULL;
    FILE *fp_strm = OT_NULL;
    FILE *bin_fileP = OT_NULL;
    ot_char c_stream_file[FILE_NAME_LEN];
    ot_char c_bin_file[FILE_NAME_LEN];
    ot_char *path = OT_NULL;
    ot_char *bin_path = OT_NULL;
    ot_u64 *frame_lens = OT_NULL;
    ot_u64 frame_cnt = 0;

    //set thread name
    prctl(PR_SET_NAME, "video_send_stream", 0, 0, 0);

    //check param
    if (sample_comm_vdec_check_send_stream_param(thread_param, c_stream_file, c_bin_file, FILE_NAME_LEN) != OT_SUCCESS) {
        return (ot_void *)(OT_FAILURE);
    }

    //open stream file & bin file
    path = realpath(c_stream_file, OT_NULL);
    if (path == OT_NULL) {
        sample_print("chn %d Invalid stream path. Please check!\n", (ot_s32)((intptr_t)thread_param->chn_hdl));
        return (ot_void *)(OT_FAILURE);
    }
    fp_strm = fopen(path, "rb");
    if (fp_strm == OT_NULL) {
        sample_print("chn %d can't open file %s in send stream thread!\n", (ot_s32)((intptr_t)thread_param->chn_hdl), c_stream_file);
        goto stream_file_open_fail;
    }
    bin_path = realpath(c_bin_file, OT_NULL);
    if (bin_path == OT_NULL) {
        sample_print("chn %d Invalid stream bin path. Please check!\n", (ot_s32)((intptr_t)thread_param->chn_hdl));
        goto bin_path_is_invalid;
    }
    bin_fileP = fopen(bin_path, "rb");
    if (bin_fileP == OT_NULL) {
        sample_print("chn %d can't open file %s.bin in send stream thread!\n", (ot_s32)((intptr_t)thread_param->chn_hdl), c_stream_file);
        goto bin_file_open_fail;
    }
    
    //parse frame lens from bin file
    fseek(bin_fileP, 0, SEEK_END);
    frame_cnt = ftell(bin_fileP) / sizeof(ot_u64);
    fseek(bin_fileP, 0, SEEK_SET);
    frame_lens = (ot_u64 *)malloc(sizeof(ot_u64) * frame_cnt);
    if (frame_lens == OT_NULL) {
        sample_print("chn %d can't alloc %d for frame_lens in send stream thread!\n", (ot_s32)((intptr_t)thread_param->chn_hdl), (sizeof(ot_u64) * frame_cnt));
        goto lens_buf_malloc_fail;
    }
    fread(frame_lens, sizeof(ot_u64), frame_cnt, bin_fileP);

    printf("\n \033[0;36m chn %d, stream file:%s, stream file frame_cnt:%d, userbufsize: %d \033[0;39m\n", (ot_s32)((intptr_t)thread_param->chn_hdl),
        thread_param->c_file_name, frame_cnt, thread_param->min_buf_size);
        
    //malloc buf for stream
    buf = malloc(thread_param->min_buf_size);
    if (buf == OT_NULL) {
        sample_print("chn %d can't alloc %d in send stream thread!\n", (ot_s32)((intptr_t)thread_param->chn_hdl), thread_param->min_buf_size);
        goto stream_buf_malloc_fail;
    }
    fflush(stdout);

    //read stream from stream file & send to vdec
    sample_comm_vdec_send_stream_process(thread_param, fp_strm, buf, frame_lens, frame_cnt);
    sample_comm_vdec_send_EOS(thread_param);

    printf("\033[0;35m chn %d send steam thread return !!!...  \033[0;39m\n", (ot_s32)((intptr_t)thread_param->chn_hdl));
    fflush(stdout);

    free(buf);
    buf = OT_NULL;
stream_buf_malloc_fail:
    free(frame_lens);
    frame_lens = OT_NULL;
lens_buf_malloc_fail:
    fclose(bin_fileP);
    bin_fileP = OT_NULL;    
bin_file_open_fail:
    free(bin_path);
    bin_path = OT_NULL;
bin_path_is_invalid:
    fclose(fp_strm);
    fp_strm = OT_NULL;
stream_file_open_fail:
    free(path);
    path = OT_NULL;
    return (ot_void *)OT_SUCCESS;
}

ot_void sample_comm_vdec_start_send_stream(vdec_thread_param *vdec_send, pthread_t *vdec_thread)
{
    if ((vdec_send == OT_NULL) || (vdec_thread == OT_NULL)) {
        printf("vdec_send or vdec_thread can't be NULL!\n");
        return;
    }
    *vdec_thread = 0;
    pthread_create(vdec_thread, 0, sample_comm_vdec_send_stream, (ot_void*)vdec_send);
}

ot_s32 sample_comm_vdec_start(sample_vdec_attr *vdec_attr)
{
    check_return(ot_eis_vdec_chn_create(&vdec_attr->vdec_hdl, &vdec_attr->chn_attr), "vdec create chn");
    check_chn_return(ot_eis_vdec_chn_start(vdec_attr->vdec_hdl), ((ot_s32)((intptr_t)vdec_attr->vdec_hdl)), "vdec start recv stream");
    return OT_SUCCESS;
}
ot_s32 sample_comm_vdec_stop(ot_eis_vdec_chn_handle vdec_hdl)
{
    check_chn_return(ot_eis_vdec_chn_stop(vdec_hdl),  ((ot_s32)((intptr_t)vdec_hdl)), "vdec stop recv stream");
    check_chn_return(ot_eis_vdec_chn_destroy(vdec_hdl), ((ot_s32)((intptr_t)vdec_hdl)), "vdec destroy chn");
    return OT_SUCCESS;
}
