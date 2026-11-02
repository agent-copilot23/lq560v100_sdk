#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 


#include "ot_media_pipe.h"

static void case1()
{
    ot_s32 ret = -1;
    ot_eis_handle pip_hdl;
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port2;
    ot_eis_media_node_port sink_port;
    ot_u32 src_num = 0;
    ot_u32 sink_num = 0;
    ot_eis_media_node_port sinks[OT_MAX_BIND_PORT_NUM] = {0};
    ot_eis_media_node_port srcs[OT_MAX_BIND_PORT_NUM] = {0};

    
    printf("pipe case1 start!\n");

    ret = ot_media_pipe_init();
    if( ret != 0){
        printf("pipe init err! ret[%d]\n", ret);
        return;
    }

    ret = ot_media_pipe_create(&pip_hdl);
    if( ret != 0){
        printf("pipe create err! ret[%d]\n", ret);
        return;
    }

    printf("pip hdl create success! pip[0x%x]\n", (ot_u32)((uintptr_t)pip_hdl));

    src_port.node_type = OT_EIS_MODULE_VI;
    src_port.handle = (ot_eis_handle)0x0;


    sink_port.node_type = OT_EIS_MODULE_VPROC;
    sink_port.handle = (ot_eis_handle)0x0;

    sink_port2.node_type = OT_EIS_MODULE_VPROC;
    sink_port2.handle = (ot_eis_handle)0x1;

    ret = ot_media_pipe_attach(pip_hdl, &src_port, &sink_port);
    if( ret != 0){
        printf("pipe attach err! ret[%d]\n", ret);
        return;
    }

    ret = ot_media_pipe_get_attached_by_src(&src_port, sinks, &sink_num);
    if( ret != 0){
        printf("pipe get attach err! ret[%d]\n", ret);
        return;
    }
    printf("pipe get attached by src success! sink_num[%d] sink0_node[%d] sink0_hdl[0x%x]\n", sink_num, sinks[0].node_type,(ot_u32)((uintptr_t)sinks[0].handle));

    ret = ot_media_pipe_get_attached_by_sink(&sink_port, srcs, &src_num);
    if( ret != 0){
        printf("pipe attach err! ret[%d]\n", ret);
        return;
    }
    printf("pipe get attached by sink success! src_num[%d] src0_node[%d] src0_hdl[0x%x]\n", src_num, srcs[0].node_type, (ot_u32)((uintptr_t)srcs[0].handle));


    ret = ot_media_pipe_attach(pip_hdl, &src_port, &sink_port2);
    if( ret != 0){
        printf("pipe attach err! ret[%d]\n", ret);
        return;
    }

    ret = ot_media_pipe_get_attached_by_src(&src_port, sinks, &sink_num);
    if( ret != 0){
        printf("pipe get attach err! ret[%d]\n", ret);
        return;
    }
    printf("pipe get attached by src success! sink_num[%d] sink0_node[%d] sink0_hdl[0x%x]\n", sink_num, sinks[0].node_type, (ot_u32)((uintptr_t)sinks[0].handle));

    ret = ot_media_pipe_get_attached_by_sink(&sink_port, srcs, &src_num);
    if( ret != 0){
        printf("pipe attach err! ret[%d]\n", ret);
        return;
    }
    printf("pipe get attached by sink success! src_num[%d] src0_node[%d] src0_hdl[0x%x]\n", src_num, srcs[0].node_type, (ot_u32)((uintptr_t)srcs[0].handle));

    ret = ot_media_pipe_detach(pip_hdl, &src_port, &sink_port);
    if( ret != 0){
        printf("pipe detach err! ret[%d]\n", ret);
        return;
    }


    ret = ot_media_pipe_detach(pip_hdl, &src_port, &sink_port2);
    if( ret != 0){
        printf("pipe detach err! ret[%d]\n", ret);
        return;
    }


    ret = ot_media_pipe_attach(pip_hdl, &sink_port2, &src_port);
    if( ret != 0){
        printf("pipe attach err! ret[0x%x]\n", ret);
        return;
    }

    ret = ot_media_pipe_destroy(pip_hdl);
    if( ret != 0){
        printf("pipe create err! ret[%d]\n", ret);
        return;
    }

    ret = ot_media_pipe_deinit();
    if( ret != 0){
        printf("pipe deinit err! ret[%d]\n", ret);
        return;
    }

    printf("pipe case1 end!\n");

}

int main()
{
    case1();

    return 0;
}
