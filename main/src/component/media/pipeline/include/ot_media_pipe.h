#ifndef __OT_MEDIA_PIPE_H__
#define __OT_MEDIA_PIPE_H__

#include "ot_media_pipe_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================
 * Name：ot_media_pipe_init
 * Author：
 * Date：2023-09-10
 * Function：media pipe init(manager_info/posix etc)
 * Input：none
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_media_pipe_init(ot_void);

/*==================================================================
 * Name：ot_media_pipe_deinit
 * Author：
 * Date：2023-09-10
 * Function：media pipe deinit(manager_info/posix etc)
 * Input：none
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_media_pipe_deinit(ot_void);

/*==================================================================
 * Name：ot_media_pipe_pipline_create
 * Author：
 * Date：2023-09-10s
 * Function：pipline create
 * Input：callback
 * Output: pipline_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_media_pipe_create(ot_eis_handle *pipline_hdl);

/*==================================================================
 * Name：ot_media_pipe_pipline_destroy
 * Author：
 * Date：2023-09-10
 * Function：pipline detroy
 * Input：pipline_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_media_pipe_destroy(ot_eis_handle pipline_hdl);

/*==================================================================
 * Name：ot_media_pipe_start
 * Author：
 * Date：2023-09-10
 * Function：pipline start
 * Input：pipline_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_media_pipe_start(ot_eis_handle pipline_hdl);

/*==================================================================
 * Name：ot_media_pipe_stop
 * Author：
 * Date：2023-09-10
 * Function：pipline stop
 * Input：pipline_hdl
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_media_pipe_stop(ot_eis_handle pipline_hdl);

/*==================================================================
 * Name: ot_media_pipe_attach
 * Author：
 * Date：2023-09-10
 * Function：attach port src and sink port
 * Input：ot_eis_handle pipline_hdl
 *        ot_eis_media_node_port src_port
 *        ot_eis_media_node_port sink_port
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_media_pipe_attach(ot_eis_handle pipline_hdl, const ot_eis_media_node_port* src_port, const ot_eis_media_node_port* sink_port);

/*==================================================================
 * Name: ot_media_pipe_detach
 * Author：
 * Date：2023-09-10
 * Function：detach port src and dst, link src_out and dst_in and trans data
 * Input：ot_eis_handle pipline_hdl
 *        ot_eis_media_node_port src_port
 *        ot_eis_media_node_port sink_port
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_media_pipe_detach(ot_eis_handle pipline_hdl, const ot_eis_media_node_port* src_port, const ot_eis_media_node_port* sink_port);

/*==================================================================
 * Name: ot_media_pipe_get_attached_by_src
 * Author：
 * Date：2023-09-10
 * Function：get all attached sink_ports by src_port, mainly one src multi sink ports.(1VN)
 * Input：ot_eis_media_node_port src_port
 * Output:ot_eis_media_node_port sink_port[]
 *        ot_u32 sink_num
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_media_pipe_get_attached_by_src(const ot_eis_media_node_port* src_port, 
                                                  ot_eis_media_node_port sink_port[OT_MAX_BIND_PORT_NUM], ot_u32 *sink_num);

/*==================================================================
 * Name: ot_media_pipe_get_attached_by_sink
 * Author：
 * Date：2023-09-10
 * Function：get all attached src_ports by sink_port, mainly one dst multi src ports.(1V1)
 * Input：ot_eis_handle pipline_hdl
 *        ot_eis_media_node_port sink_port
 * Output:ot_eis_media_node_port src_port[]
 *        ot_u32 src_num
 * Ret_val：ot_s32 (0: success | other: fail)
 * Changelog：
==================================================================*/
ot_s32 ot_media_pipe_get_attached_by_sink(const ot_eis_media_node_port* sink_port, 
                                                       ot_eis_media_node_port src_port[OT_MAX_BIND_PORT_NUM], ot_u32 *src_num);
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
