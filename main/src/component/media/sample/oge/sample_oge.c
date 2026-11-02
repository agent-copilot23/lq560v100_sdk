#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <linux/fb.h>

#include "ot_oge.h"
#include "ot_gfb.h"
#include "ot_smr.h"
#include "ot_vrb.h"
#include "ot_buffer_pool.h"
#include "sample_comm.h"
#include "load_bmp.h"
#define SAMPLE_BMP_PATH        "./source_file/humen_red.bmp"
#define SAMPLE_BMP_565_PATH    "./source_file/rgb565.bmp"
#define SAMPLE_BMP_1555_PATH   "./source_file/argb1555.bmp"

#define SAMPLE_OGE_WIDTH_1920  1920
#define SAMPLE_OGE_HEIGHT_1080 1080

#define SAMPLE_OGE_CMAP_LENGTH_MAX 256

#define SAMPLE_OGE_RED_1555           0xFC00
#define SAMPLE_OGE_GREEN_1555         0x83E0
#define SAMPLE_OGE_BLUE_1555          0x801F
#define SAMPLE_OGE_RED_8888           0xFFff0000
#define SAMPLE_OGE_WHITE_8888         0xFFFFFFFF
#define SAMPLE_OGE_BLACK_8888         0x00000000
#define SAMPLE_OGE_GREEN_8888         0xFF00FF00
#define SAMPLE_OGE_BLUE_8888          0xFF0000FF
#define SAMPLE_OGE_YELLOW_8888        0xFF00FFFF
#define SAMPLE_OGE_RED_565            0xF800
#define SAMPLE_OGE_GREEN_565          0x7E0
#define SAMPLE_OGE_BLUE_565           0x1F

#define SAMPLE_OGE_IMAGE_WIDTH     160
#define SAMPLE_OGE_IMAGE_HEIGHT    160

#define SAMPLE_OGE_IMAGE_565_WIDTH     640
#define SAMPLE_OGE_IMAGE_565_HEIGHT    480

#define SAMPLE_OGE_IMAGE_1555_WIDTH     640
#define SAMPLE_OGE_IMAGE_1555_HEIGHT    480

#define RECT_NUMS 7
#define LINE_NUMS 2
#define LINE_STRIDE 100

#define USE_THREAD      0

pthread_t g_oge_thread0 = 0;

static ot_char g_exit_flag = 0;

//static ot_eis_handle pool_hdl;

static ot_s32 fd = -1;

// clut4
static struct fb_bitfield g_a4 = {0, 0, 0};
static struct fb_bitfield g_r4 = {0, 4, 0};
static struct fb_bitfield g_g4 = {0, 4, 0};
static struct fb_bitfield g_b4 = {0, 4, 0};

// argb1555
static struct fb_bitfield g_r16 = {10, 5, 0};
static struct fb_bitfield g_g16 = {5, 5, 0};
static struct fb_bitfield g_b16 = {0, 5, 0};
static struct fb_bitfield g_a16 = {15, 1, 0};

// argb888
static struct fb_bitfield g_a32 = {24, 8, 0};
static struct fb_bitfield g_r32 = {16, 8, 0};
static struct fb_bitfield g_g32 = {8,  8, 0};
static struct fb_bitfield g_b32 = {0,  8, 0};

// rgb565
static struct fb_bitfield g_a_16 = {0, 0, 0};
static struct fb_bitfield g_r_16 = {11, 5, 0};
static struct fb_bitfield g_g_16 = {5,  6, 0};
static struct fb_bitfield g_b_16 = {0,  5, 0};


ot_u16 g_cmap_red[SAMPLE_OGE_CMAP_LENGTH_MAX] = {0xff, 0, 0, 0xff};
ot_u16 g_cmap_green[SAMPLE_OGE_CMAP_LENGTH_MAX] = {0, 0xff, 0, 0xff};
ot_u16 g_cmap_blue[SAMPLE_OGE_CMAP_LENGTH_MAX] = {0, 0, 0xff, 0xff};
ot_u16 g_cmap_alpha[SAMPLE_OGE_CMAP_LENGTH_MAX] = {0xff, 0xff, 0xff, 0xff};

/* the color format OSD supported */
typedef enum {
    OSD_COLOR_FORMAT_RGB444 = 0,
    OSD_COLOR_FORMAT_RGB4444 = 1,
    OSD_COLOR_FORMAT_RGB555 = 2,
    OSD_COLOR_FORMAT_RGB565 = 3,
    OSD_COLOR_FORMAT_RGB1555 = 4,
    OSD_COLOR_FORMAT_RGB888 = 6,
    OSD_COLOR_FORMAT_RGB8888 = 7,
    OSD_COLOR_FORMAT_CLUT2 = 8,
    OSD_COLOR_FORMAT_CLUT4 = 9,
    OSD_COLOR_FORMAT_BUTT
} oge_osd_color_format;

/* all buffer mode GFB supported*/
typedef enum {
    GFB_MODE_EXTEND_ZERO_REFRESH = 0, // extend mode: 0buffer refresh
    GFB_MODE_EXTEND_ONE_BUFFER = 1,  // extend mode: 1 buffer
    GFB_MODE_EXTEND_DOUBLE_BUFFER = 2, // extend mode: 2 buffer
    GFB_MODE_EXTEND_DOUBLE_IMMDEDIATE = 3, // extend mode: 2 buffer immdediate
    GFB_MODE_STAND_ZERO_BUFFER = 4, // stand mode: pan display
    GFB_MODE_INVALID
} sample_oge_mode_info;

typedef struct {
    ot_s32 fd; /* fb's file describe */
    ot_s32 layer; /* which graphic layer */
    sample_oge_mode_info mode_info; /* {0,1,2,3,4}={0buffer refresh, 1buffer, 2buffer, 2buffer immdediate, 0buffer pan display}*/
    ot_bool compress; /* image compressed or not */
    ot_eis_fb_color_format color_format; /* color format. */
} oge_gfb_sample_info;

typedef struct {
    ot_eis_fb_color_format clr_fmt;
    ot_oge_color_format oge_clr_fmt;
    ot_u32 color;
    ot_u32 color_diff;
    ot_u32 byte_per_pixel;
    ot_u32 image_width;
    ot_u32 image_height;
    struct fb_bitfield red;		/* bitfield in fb mem if true color, */
    struct fb_bitfield green;	/* else only length is significant */
    struct fb_bitfield blue;
    struct fb_bitfield transp;
    ot_u32 bits_per_pixel;
} sample_oge_attr;

ot_phys_addr g_phyaddr = 0;
ot_phys_addr buf_phyaddr = 0;
oge_osd_color_format g_osd_color_fmt = OSD_COLOR_FORMAT_RGB1555;

static ot_s32 sample_oge_vrb_init()
{
    ot_video_buffer_attr vrb_cfg = {0};

    sample_common_get_vrb_cfg(2, 2048, 2, &vrb_cfg);
    if (sample_comm_sys_init(&vrb_cfg) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    return 0;
}

static ot_s32 sample_clear_screen(void* buf, ot_u32 byte_per_pixel, ot_eis_fb_buf *canvas_buf)
{
    if (memset_s(buf, SAMPLE_OGE_WIDTH_1920 * SAMPLE_OGE_HEIGHT_1080 * byte_per_pixel, 0x00, canvas_buf->canvas.pitch * canvas_buf->canvas.height) != OT_SUCCESS) {
        printf("ERROR: memset failed \n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}


static ot_s32 sample_oge_var_init_corner_box(ot_eis_fb_color_format color_format, struct fb_var_screeninfo *var)
{
    if (color_format != OT_EIS_FB_FORMAT_4BPP) {
        printf("Error: the color format is not OT_EIS_FB_FORMAT_4BPP!\n");
        return OT_FAILURE;
    }
    /* get the variable screen information */
    if (ot_gfb_get_var_screen_attr(fd, var) != OT_SUCCESS) {
        printf("get variable screen info failed!\n");
        return OT_FAILURE;
    }

    var->transp = g_a4;
    var->red    = g_r4;
    var->green  = g_g4;
    var->blue   = g_b4;
    var->bits_per_pixel = 4; /* 4 bits per pixel */
    var->xres_virtual = SAMPLE_OGE_WIDTH_1920;
    var->yres_virtual = SAMPLE_OGE_HEIGHT_1080 * 2; /* alloc 2 buf */
    var->xres = SAMPLE_OGE_WIDTH_1920;
    var->yres = SAMPLE_OGE_HEIGHT_1080;
    var->activate = 0;
    var->xoffset = 0;
    var->yoffset = 0;

    if (ot_gfb_set_var_screen_attr(fd, var) != OT_SUCCESS) {
        printf("put variable screen info failed!\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_cmap_init()
{
    struct fb_cmap cmap;

    cmap.start = 0;
    cmap.len = SAMPLE_OGE_CMAP_LENGTH_MAX;
    cmap.red = g_cmap_red;
    cmap.green = g_cmap_green;
    cmap.blue = g_cmap_blue;
    cmap.transp = g_cmap_alpha;

    if (ot_gfb_set_color_map_attr(fd, &cmap) != OT_SUCCESS) {
        printf("put cmap info failed!\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_fix_and_mmap(struct fb_fix_screeninfo *fix,
                                      ot_void **viraddr)
{
    ot_s32 ret = OT_FAILURE;

    if (fix == OT_NULL || viraddr == OT_NULL) {
        return OT_FAILURE;
    }
    if (ot_gfb_get_fix_screenf_info(fd, fix) != OT_SUCCESS) {
        printf("get fix screen info failed!\n");
        return OT_FAILURE;
    }

    ret = ot_gfb_mmap(fd, SAMPLE_OGE_WIDTH_1920 * SAMPLE_OGE_HEIGHT_1080, viraddr);
    if (ret != OT_SUCCESS) {
        printf("mmap fd[%d] failed!\n", fd);
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_oge_draw_rect_nums(ot_void *viraddr, ot_u32 index,
                             const struct fb_var_screeninfo *var, struct fb_fix_screeninfo *fix)
{
    ot_oge_handle handle;
    ot_u32 i;

    ot_oge_corner_rect corner[RECT_NUMS];
    ot_oge_surface dst_surface = {0};
    ot_oge_rect dst_rect[RECT_NUMS];
    ot_oge_none_src none_src[RECT_NUMS];
    ot_oge_corner_rect_info conner_rect[RECT_NUMS];

    if (viraddr == OT_NULL) {
        return OT_FAILURE;
    }
    /* clear buffer,fill white */
    memset(viraddr, 0x33, SAMPLE_OGE_WIDTH_1920 * SAMPLE_OGE_HEIGHT_1080);

    handle = ot_oge_begin_job();
    if (handle == OT_ERR_OGE_INVALID_HANDLE || handle == OT_ERR_OGE_DEV_NOT_OPEN) {
        printf("begin job failed \n");
        return OT_FAILURE;
    }
    dst_surface.color_format = OT_OGE_COLOR_FORMAT_CLUT4;
    dst_surface.width = SAMPLE_OGE_WIDTH_1920;
    dst_surface.height = SAMPLE_OGE_HEIGHT_1080;
    dst_surface.stride = fix->line_length;
    dst_surface.phys_addr = (index % 2) ? (fix->smem_start + fix->line_length * var->yres) : /* 2 alg data */
                            (fix->smem_start);

    for ( i = 0; i < RECT_NUMS; i++ ) {
        dst_rect[i].pos_x = i * 200; // User-changeable
        dst_rect[i].pos_y = 200;  // User-changeable
        dst_rect[i].height = 100; /* 100 alg data */
        dst_rect[i].width = 100; /* 200 alg data */
        none_src[i].dst_surface = &dst_surface;
        none_src[i].dst_rect = &dst_rect[i];

        conner_rect[i].width = 10 + i * 10; /* 10 alg data */
        conner_rect[i].height = 20 + i * 10; /* 20 alg data */
        conner_rect[i].inner_color = 0x1;
        conner_rect[i].outer_color = (index % 2) ? 0x0 : 0x2;

        corner[i].corner_rect_region = none_src[i].dst_rect;
        corner[i].corner_rect_info = &conner_rect[i];
    }

    if (ot_oge_draw_corner_box(handle, none_src->dst_surface, corner, RECT_NUMS) < 0) {
        ot_oge_cancel_job(handle);
        return OT_FAILURE;
    }

    if (ot_oge_end_job(handle, OT_FALSE, OT_TRUE, 10) < 0) { /* 10 timeout */
        ot_oge_cancel_job(handle);
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_start_draw_rect(ot_oge_none_src *none_src, ot_oge_corner_rect_info *conner_rect)
{
    ot_oge_handle handle;
    ot_u32 i;
    ot_oge_corner_rect corner;

    handle = ot_oge_begin_job();
    if (handle == OT_ERR_OGE_INVALID_HANDLE || handle == OT_ERR_OGE_DEV_NOT_OPEN) {
        printf("begin job failed \n");
        return OT_FAILURE;
    }

    corner.corner_rect_region = none_src->dst_rect;
    corner.corner_rect_info = conner_rect;

    /* times draw rect,once commit */
    for ( i = 0; i < 6; i++ ) { /* 6 draw 6 rect */
        if (ot_oge_draw_corner_box(handle, none_src->dst_surface, &corner, 1) < 0) {
            ot_oge_cancel_job(handle);
            return OT_FAILURE;
        }
        none_src->dst_rect->pos_x += 200; /* 200 alg data */
        none_src->dst_rect->pos_y += 200; /* 200 alg data */
    }

    if (ot_oge_end_job(handle, OT_FALSE, OT_TRUE, 10) < 0) { /* 10 timeout */
        ot_oge_cancel_job(handle);
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_draw_rect(ot_void *viraddr, ot_u32 index,
                             const struct fb_var_screeninfo *var, struct fb_fix_screeninfo *fix)
{
    ot_oge_surface dst_surface = {0};
    ot_oge_rect dst_rect = {0};
    ot_oge_none_src none_src = {0};
    ot_oge_corner_rect_info conner_rect = {0};

    if (viraddr == OT_NULL) {
        return OT_FAILURE;
    }
    /* clear buffer,fill white */
    memset(viraddr, 0x33, SAMPLE_OGE_WIDTH_1920 * SAMPLE_OGE_HEIGHT_1080);

    dst_surface.color_format = OT_OGE_COLOR_FORMAT_CLUT4;
    dst_surface.width = SAMPLE_OGE_WIDTH_1920;
    dst_surface.height = SAMPLE_OGE_HEIGHT_1080;
    dst_surface.stride = fix->line_length;
    dst_surface.phys_addr = (index % 2) ? (fix->smem_start + fix->line_length * var->yres) : /* 2 alg data */
                          (fix->smem_start);
    dst_rect.pos_x = 0;
    dst_rect.pos_y = 0;
    dst_rect.height = 100; /* 100 alg data */
    dst_rect.width = 100; /* 100 alg data */
    none_src.dst_surface = &dst_surface;
    none_src.dst_rect = &dst_rect;
    /* conner rect */
    conner_rect.width = 10; /* 10 alg data */
    conner_rect.height = 20; /* 20 alg data */
    conner_rect.inner_color = 0x1;
    conner_rect.outer_color = (index % 2) ? 0x0 : 0x2; /* 2 alg data,0x2 for cmap index */

    if (sample_oge_start_draw_rect(&none_src, &conner_rect) != OT_SUCCESS) {
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_pandisplay(ot_u32 index, struct fb_var_screeninfo *var)
{
    if (var == OT_NULL || fd < 0) {
        return OT_FAILURE;
    }
    if ((index % 2) == 0) { /* 2 alg data */
        var->yoffset = 0;
    } else {
        var->yoffset = var->yres;
    }

    if (ot_gfb_set_display_offset(fd, var) != OT_SUCCESS) {
         printf("ot_gfb_set_display_offset failed!\n");
         return OT_FAILURE;
    }

    printf("expected: buffer%d!\n", ((index % 2 == 0) ? 0 : 1)); /* 2 alg data */
    printf("wait 1 seconds.\n");
    sleep(1);
    return OT_SUCCESS;
}

static ot_s32 sample_oge_init_frame_buffer(oge_gfb_sample_info *info)
{
    ot_bool show;
    ot_eis_fb_point point = {0, 0};

    show = OT_FALSE;
    if (ot_gfb_set_display_state(info->fd, show) != OT_SUCCESS) {
        printf("ot_gfb_set_display_state failed!\n");
        info->fd = -1;
        return OT_FAILURE;
    }

    /* step 2. set the screen original position */
    point.x_pos = 0;
    point.y_pos = 0;

    if (ot_gfb_set_screen_origin(info->fd, &point) != OT_SUCCESS) {
        printf("set screen original show position failed!\n");
        info->fd = -1;
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_oge_gfb_init_var(oge_gfb_sample_info *info, sample_oge_attr *oge_attr)
{
    struct fb_var_screeninfo var;

    if (ot_gfb_get_var_screen_attr(info->fd, &var) != OT_SUCCESS) {
        printf("get variable screen info failed!\n");
        return OT_FAILURE;
    }

    var.transp = oge_attr->transp;
    var.red = oge_attr->red;
    var.green = oge_attr->green;
    var.blue = oge_attr->blue;
    var.bits_per_pixel = oge_attr->bits_per_pixel;
    var.xres_virtual = SAMPLE_OGE_WIDTH_1920;
    var.yres_virtual = SAMPLE_OGE_HEIGHT_1080 * 2; /* 2 for 2buf */
    var.xres = SAMPLE_OGE_WIDTH_1920;
    var.yres = SAMPLE_OGE_HEIGHT_1080;
    var.activate = FB_ACTIVATE_NOW;

    if (ot_gfb_set_var_screen_attr(info->fd, &var) != OT_SUCCESS) {
        printf("put variable screen info failed!\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_gfb_init_var_mode(oge_gfb_sample_info *info, sample_oge_attr *oge_attr)
{
    struct fb_var_screeninfo var;
    ot_eis_fb_colorkey color_key = {0};
    ot_eis_fb_alpha alpha = {0};

    if (ot_gfb_get_var_screen_attr(info->fd, &var) != OT_SUCCESS) {
        printf("get variable screen info failed!\n");
        return OT_FAILURE;
    }

    var.transp = oge_attr->transp;
    var.red = oge_attr->red;
    var.green = oge_attr->green;
    var.blue = oge_attr->blue;
    var.bits_per_pixel = oge_attr->bits_per_pixel;
    var.xres_virtual = SAMPLE_OGE_WIDTH_1920;
    var.yres_virtual = SAMPLE_OGE_HEIGHT_1080; /* 1 for 1buf */
    var.xres = SAMPLE_OGE_WIDTH_1920;
    var.yres = SAMPLE_OGE_HEIGHT_1080;
    var.activate = FB_ACTIVATE_NOW;

    if (ot_gfb_set_var_screen_attr(info->fd, &var) != OT_SUCCESS) {
        printf("put variable screen info failed!\n");
        return OT_FAILURE;
    }

    alpha.pixel_alpha = OT_FALSE;
    alpha.global_alpha_en = OT_FALSE;
    alpha.global_alpha = 0;
    if (ot_gfb_set_alpha_cfg(info->fd, &alpha) != OT_SUCCESS) {
        printf("set alpha failed!\n");
        return OT_FAILURE;
    }

    color_key.enable = OT_FALSE;
    if (ot_gfb_set_colorkey_vaule(info->fd, &color_key)!= OT_SUCCESS) {
        printf("ot_gfb_set_colorkey_vaule failed!\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_void sample_oge_draw_rect_by_cpu(ot_void *ptemp, struct fb_var_screeninfo *var, sample_oge_attr *oge_attr)
{
    ot_s32 x, y;
    for ( y = 100; y < 300; y++ ) { /* 100 300 for y */
        for ( x = 0; x < 300; x++ ) { /* 300 for x */
            if (oge_attr->clr_fmt == OT_EIS_FB_FORMAT_ARGB8888) {
                *((ot_u32*)ptemp + y * var->xres + x) = oge_attr->color;
            } else {
                *((ot_u16*)ptemp + y * var->xres + x) = oge_attr->color;
            }
        }
    }
    return;
}

static ot_s32 sample_oge_put_alpha_and_colorkey(oge_gfb_sample_info *info, sample_oge_attr *oge_attr)
{
    ot_s32 ret = OT_FAILURE;
    ot_eis_fb_alpha alpha = {0};
    ot_eis_fb_colorkey color_key;
    printf("expected: the red box will appear!\n");
    sleep(2); /* 2 second */
    if (oge_attr->clr_fmt == OT_EIS_FB_FORMAT_ARGB1555) {
        alpha.pixel_alpha = OT_TRUE;
        alpha.alpha0 = 0x0;
        alpha.alpha1 = 0x0;
        if (ot_gfb_set_alpha_cfg(info->fd, &alpha) != OT_SUCCESS) {
            printf("set alpha failed!\n");
            return OT_FAILURE;
        }
        printf("expected: after set alpha = 0, the red box will disappear!\n");
        sleep(2); /* 2 second */

        alpha.alpha0 = 0;
        alpha.alpha1 = 0xFF;
        if (ot_gfb_set_alpha_cfg(info->fd, &alpha) != OT_SUCCESS) {
            printf("set alpha failed!\n");
            return OT_FAILURE;
        }
        printf("expected: after set set alpha = 0xFF, the red box will appear again!\n");
        sleep(2); /* 2 second */
    }

    alpha.pixel_alpha = OT_FALSE;
    alpha.global_alpha_en = OT_TRUE;
    alpha.global_alpha = 0;
    if (ot_gfb_set_alpha_cfg(info->fd, &alpha) != OT_SUCCESS) {
        printf("set alpha failed!\n");
        return OT_FAILURE;
    }
    printf("expected: set global alpha=0, the red box will disappear!\n");
    sleep(2); /* 2 second */

    alpha.global_alpha = 0x78;
    if (ot_gfb_set_alpha_cfg(info->fd, &alpha) != OT_SUCCESS) {
        printf("set alpha failed!\n");
        return OT_FAILURE;
    }
    printf("expected: after set global alpha = 120, the red box will appear again!\n");
    sleep(2); /* 2 second */
    
    printf("expected: the red box will erased by colorkey!\n");
    color_key.enable = OT_TRUE;
    color_key.value = oge_attr->color;
    ret = ot_gfb_set_colorkey_vaule(info->fd, &color_key);
    if (ret != OT_SUCCESS) {
        printf("ot_gfb_set_colorkey_vaule failed!\n");
        return OT_FAILURE;
    }
    sleep(2); /* 2 second */
    printf("expected: the red box will appear again!\n");
    color_key.enable = OT_FALSE;
    ret = ot_gfb_set_colorkey_vaule(info->fd, &color_key);
    if (ret != OT_SUCCESS) {
        printf("ot_gfb_set_colorkey_vaule failed!\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

ot_void sample_oge_get_param(oge_gfb_sample_info *info, sample_oge_attr *oge_attr)
{
    switch (info->color_format) {
        case OT_EIS_FB_FORMAT_ARGB8888:
            oge_attr->clr_fmt = OT_EIS_FB_FORMAT_ARGB8888;
            oge_attr->color = SAMPLE_OGE_RED_8888;
            oge_attr->color_diff = SAMPLE_OGE_BLUE_8888;
            oge_attr->oge_clr_fmt = OT_OGE_COLOR_FORMAT_ARGB8888;
            oge_attr->byte_per_pixel = 4;
            oge_attr->image_width = SAMPLE_OGE_IMAGE_WIDTH;
            oge_attr->image_height = SAMPLE_OGE_IMAGE_HEIGHT;
            oge_attr->transp = g_a32;
            oge_attr->red = g_r32;
            oge_attr->green = g_g32;
            oge_attr->blue = g_b32;
            oge_attr->bits_per_pixel = 32; /* 32 for 4 byte */
            break;
        case OT_EIS_FB_FORMAT_RGB565:
            oge_attr->clr_fmt = OT_EIS_FB_FORMAT_RGB565;
            oge_attr->color = SAMPLE_OGE_RED_565;
            oge_attr->color_diff = SAMPLE_OGE_BLUE_565;
            oge_attr->oge_clr_fmt = OT_OGE_COLOR_FORMAT_RGB565;
            oge_attr->byte_per_pixel = 2;
            oge_attr->image_width = SAMPLE_OGE_IMAGE_565_WIDTH;
            oge_attr->image_height = SAMPLE_OGE_IMAGE_565_HEIGHT;
            oge_attr->transp = g_a_16;
            oge_attr->red = g_r_16;
            oge_attr->green = g_g_16;
            oge_attr->blue = g_b_16;
            oge_attr->bits_per_pixel = 16; /* 32 for 4 byte */
            break;
        default:
            oge_attr->clr_fmt = OT_EIS_FB_FORMAT_ARGB1555;
            oge_attr->color = SAMPLE_OGE_RED_1555;
            oge_attr->color_diff = SAMPLE_OGE_BLUE_1555;
            oge_attr->oge_clr_fmt = OT_OGE_COLOR_FORMAT_ARGB1555;
            oge_attr->byte_per_pixel = 2;
            oge_attr->image_width = SAMPLE_OGE_IMAGE_1555_WIDTH;
            oge_attr->image_height = SAMPLE_OGE_IMAGE_1555_HEIGHT;
            oge_attr->transp = g_a16;
            oge_attr->red = g_r16;
            oge_attr->green = g_g16;
            oge_attr->blue = g_b16;
            oge_attr->bits_per_pixel = 16; /* 16 for 2 byte */
            break;
    }
    return;
}

static ot_s32 sample_oge_draw_line_by_cpu_pandisplay(oge_gfb_sample_info *info, ot_u8 *show_screen,
    ot_u64 *hide_screen_phy, ot_u32 i, sample_oge_attr *oge_attr, struct fb_var_screeninfo *var, struct fb_fix_screeninfo *fix)
{
    ot_s32 x, y;
    ot_u8 *hide_screen = OT_NULL;
    ot_void *show_line = OT_NULL;

    /* OGE step1: draw two red line */
    var->yoffset = (i % 2 == 1) ? var->yres : 0; /* 2 for 0 1 */
    
    hide_screen = show_screen + (fix->line_length * var->yres) * (i % 2); /* 2 for 0 1 */
    if (memset_s(hide_screen, fix->line_length * var->yres, 0x00, fix->line_length * var->yres) != OT_SUCCESS) {
        return OT_FAILURE;
    }

    *hide_screen_phy = fix->smem_start + (i % 2) * fix->line_length * var->yres; /* 2 for 0 1 */
    show_line = hide_screen;
    for (y = (SAMPLE_OGE_HEIGHT_1080 / 2 - 2); y < (SAMPLE_OGE_HEIGHT_1080 / 2 + 2); y++) { /* 2 for alg data */
        for (x = 0; x < SAMPLE_OGE_WIDTH_1920; x++) {
            if (oge_attr->clr_fmt == OT_EIS_FB_FORMAT_ARGB8888) {
                *((ot_u32*)show_line + y * var->xres + x) = oge_attr->color;
            } else {
                *((ot_u16*)show_line + y * var->xres + x) = oge_attr->color;
            }
        }
    }
    for (y = 0; y < SAMPLE_OGE_HEIGHT_1080; y++) {
        for (x = (SAMPLE_OGE_WIDTH_1920 / 2 - 2); x < (SAMPLE_OGE_WIDTH_1920 / 2 + 2); x++) { /* 2 for alg data */
            if (oge_attr->clr_fmt == OT_EIS_FB_FORMAT_ARGB8888) {
                *((ot_u32*)show_line + y * var->xres + x) = oge_attr->color;
            } else {
                *((ot_u16*)show_line + y * var->xres + x) = oge_attr->color;
            }
        }
    }

    if (ot_gfb_set_display_offset(info->fd, var) != OT_SUCCESS) {
            printf("ot_gfb_set_display_offset failed!\n");
            return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_quick_copy_by_oge(oge_gfb_sample_info *info, ot_eis_fb_buf *canvas_buf, ot_void *viraddr, sample_oge_attr *oge_attr)
{
    ot_oge_handle handle;
    ot_oge_rect src_rect = {0};
    ot_oge_rect dst_rect = {0};
    ot_oge_surface src_surface = {0};
    ot_oge_surface dst_surface = {0};
    ot_oge_single_src single_src = {0};

    dst_rect.width = oge_attr->image_width;
    dst_rect.height = oge_attr->image_height;
    dst_surface.color_format = oge_attr->oge_clr_fmt;
    dst_surface.width = SAMPLE_OGE_WIDTH_1920;
    dst_surface.height = SAMPLE_OGE_HEIGHT_1080;
    dst_surface.stride = SAMPLE_OGE_WIDTH_1920 * (oge_attr->byte_per_pixel);
    dst_surface.phys_addr = g_phyaddr;

    src_rect.pos_x   = 0;
    src_rect.pos_y   = 0;
    src_rect.width  = oge_attr->image_width;
    src_rect.height = oge_attr->image_height;
    src_surface.color_format = oge_attr->oge_clr_fmt;
    src_surface.width = oge_attr->image_width;
    src_surface.height = oge_attr->image_height;
    src_surface.stride = (oge_attr->byte_per_pixel) * (oge_attr->image_width);
    src_surface.phys_addr = buf_phyaddr;
    src_surface.support_alpha_ex_1555 = OT_FALSE;
    src_surface.alpha_max_is_255 = OT_TRUE;
    src_surface.alpha0 = 0XFF;
    src_surface.alpha1 = 0XFF;
    single_src.src_surface = &src_surface;
    single_src.dst_surface = &dst_surface;
    single_src.src_rect = &src_rect;
    single_src.dst_rect = &dst_rect;
    handle = ot_oge_begin_job();
    if (handle == OT_ERR_OGE_INVALID_HANDLE || handle == OT_ERR_OGE_DEV_NOT_OPEN) {
        printf("begin job failed \n");
        return OT_FAILURE;
    }
    dst_rect.pos_x = 0;
    dst_rect.pos_y = 0;
    if (ot_oge_quick_copy(handle, &single_src) < 0) {
        printf("oge_quick_copy:%d failed!\n", __LINE__);
        ot_oge_cancel_job(handle);
        return OT_FAILURE;
    }

    dst_rect.pos_x = 650;
    dst_rect.pos_y = 0;
    if (ot_oge_quick_copy(handle, &single_src) < 0) {
        printf("oge_quick_copy:%d failed!\n", __LINE__);
        ot_oge_cancel_job(handle);
        return OT_FAILURE;
    }

    /* OGE job step 2. submit job */
    if (ot_oge_end_job(handle, OT_FALSE, OT_TRUE, 10) < 0) { /* 10 timeout */
        printf("line:%d,oge_end_job failed!\n", __LINE__);
        ot_oge_cancel_job(handle);
        return OT_FAILURE;
    }
    ot_oge_wait_the_task_done(handle);
    if (ot_gfb_refresh_display_info(info->fd, canvas_buf) != OT_SUCCESS) {
        printf("Error: call ot_gfb_refresh_display_info failed \n");
        return OT_FAILURE;
    }
    sleep(2);
    if (sample_clear_screen(viraddr, oge_attr->byte_per_pixel, canvas_buf) != OT_SUCCESS) {
        printf("ERROR: call clear screen failed \n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_quick_resize_by_oge(oge_gfb_sample_info *info, ot_eis_fb_buf *canvas_buf, ot_void *viraddr, sample_oge_attr *oge_attr)
{
    ot_oge_handle handle;
    ot_oge_rect src_rect = {0};
    ot_oge_rect dst_rect = {0};
    ot_oge_surface src_surface = {0};
    ot_oge_surface dst_surface = {0};
    ot_oge_single_src single_src = {0};

    dst_rect.pos_x = 0; // User-changeable
    dst_rect.pos_y = 480; // User-changeable
    // shrink by a quarter
    dst_rect.width = oge_attr->image_width / 2; // User-changeable
    dst_rect.height = oge_attr->image_height / 2; // User-changeable
    src_rect.pos_x   = 0;
    src_rect.pos_y   = 0;
    src_rect.height = oge_attr->image_height;
    src_rect.width  = oge_attr->image_width;
    dst_surface.color_format = oge_attr->oge_clr_fmt;
    dst_surface.width = SAMPLE_OGE_WIDTH_1920;
    dst_surface.height = SAMPLE_OGE_HEIGHT_1080;
    dst_surface.stride = SAMPLE_OGE_WIDTH_1920 * (oge_attr->byte_per_pixel);
    dst_surface.phys_addr = g_phyaddr;

    src_surface.color_format = oge_attr->oge_clr_fmt;
    src_surface.width = oge_attr->image_width;
    src_surface.height = oge_attr->image_height;
    src_surface.stride = (oge_attr->byte_per_pixel) * (oge_attr->image_width);
    src_surface.phys_addr = buf_phyaddr;
    src_surface.support_alpha_ex_1555 = OT_TRUE;
    src_surface.alpha_max_is_255 = OT_TRUE;
    src_surface.alpha0 = 0X00;
    src_surface.alpha1 = 0XFF;
    single_src.src_surface = &src_surface;
    single_src.dst_surface = &dst_surface;
    single_src.src_rect = &src_rect;
    single_src.dst_rect = &dst_rect;
    handle = ot_oge_begin_job();
    if (handle == OT_ERR_OGE_INVALID_HANDLE || handle == OT_ERR_OGE_DEV_NOT_OPEN) {
        printf("begin job failed \n");
        return OT_FAILURE;
    }
    if (ot_oge_quick_resize(handle, &single_src) < 0) {
        printf("ot_oge_quick_resize:%d failed!\n", __LINE__);
        ot_oge_cancel_job(handle);
        return OT_FAILURE;
    }

    /* OGE job step 2. submit job */
    if (ot_oge_end_job(handle, OT_FALSE, OT_TRUE, 10) < 0) { /* 10 timeout */
        printf("line:%d,oge_end_job failed!\n", __LINE__);
        ot_oge_cancel_job(handle);
        return OT_FAILURE;
    }
    ot_oge_wait_the_task_done(handle);
    if (ot_gfb_refresh_display_info(info->fd, canvas_buf) != OT_SUCCESS) {
        printf("Error: call ot_gfb_refresh_display_info failed \n");
        return OT_FAILURE;
    }
    sleep(2);
    if (sample_clear_screen(viraddr, oge_attr->byte_per_pixel, canvas_buf) != OT_SUCCESS) {
        printf("ERROR: call clear screen failed \n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_draw_line_smr(oge_gfb_sample_info *info, ot_void *viraddr, ot_u8 *show_screen, sample_oge_attr *oge_attr, struct fb_var_screeninfo *var, struct fb_fix_screeninfo *fix)
{
    ot_u32 i;
    ot_u64 hide_screen_phy = 0;

    printf("expected: two red line!\n");
    for (i = 0; i < 2; i++) {
        printf("draw line!\n");
        if (sample_oge_draw_line_by_cpu_pandisplay(info, show_screen, &hide_screen_phy, i, oge_attr, var, fix) != OT_SUCCESS) {
            return OT_FAILURE;
        }

        var->yoffset = (i % 2 == 1) ? var->yres : 0; /* 2 for 0 1 */

        if (ot_gfb_set_display_offset(info->fd, var) != OT_SUCCESS) {
            printf("ot_gfb_set_display_offset failed!\n");
            return OT_FAILURE;
        }
        sleep(2);
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_cpu_draw_show(oge_gfb_sample_info *info, ot_u8 *show_screen, sample_oge_attr *oge_attr, struct fb_var_screeninfo *var, struct fb_fix_screeninfo *fix)
{
    ot_s32 ret = OT_FAILURE;
    ot_void *viraddr = OT_NULL;
    ot_smr_alloc_attr smr_alloc_attr = {0};

    if (info->mode_info != GFB_MODE_STAND_ZERO_BUFFER) {
        printf("Error: buffer mode should be 0buffer pan display \n");
        return OT_FAILURE;
    }

    smr_alloc_attr.len = SAMPLE_OGE_IMAGE_WIDTH * SAMPLE_OGE_IMAGE_HEIGHT * (oge_attr->byte_per_pixel);
    strcpy(smr_alloc_attr.region_name , "");
    if (ot_smr_alloc(&smr_alloc_attr, &g_phyaddr, &viraddr) != OT_SUCCESS) {
        printf("allocate memory (max_w*max_h*%u bytes) failed\n", oge_attr->byte_per_pixel);
        return OT_FAILURE;
    }
    ret = ot_oge_open();
    if (ret < 0) {
        printf("oge_open failed :%d!\n", ret);
        goto free_smr;
    }
    if (sample_oge_draw_line_smr(info, viraddr, show_screen, oge_attr, var, fix) != OT_SUCCESS) {
        printf("cpu draw line with smr failed \n");
        goto close_oge;
    }

    ot_oge_close();
    ot_smr_free(g_phyaddr);
    g_phyaddr = 0;
    return OT_SUCCESS;

close_oge:
    ot_oge_close();

free_smr:
    ot_smr_free(g_phyaddr);
    g_phyaddr = 0;

    return OT_FAILURE;
}

static int sample_oge_draw_line(oge_gfb_sample_info *info, sample_oge_attr *oge_attr)
{
    ot_u8 *show_screen = OT_NULL;
    struct fb_var_screeninfo var;
    struct fb_fix_screeninfo fix;
    ot_bool show;
    ot_s32 ret = OT_FAILURE;
    ot_u32 i;
    ot_void *ptemp = OT_NULL;

    show = OT_TRUE;
    if (ot_gfb_set_display_state(info->fd, show) != OT_SUCCESS) {
        printf("ot_gfb_set_display_state failed!\n");
        return OT_FAILURE;
    }

    if (ot_gfb_get_var_screen_attr(info->fd, &var) != OT_SUCCESS) {
        printf("get variable screen info failed!\n");
        goto EXIT;
    }

    if (ot_gfb_get_fix_screenf_info(info->fd, &fix) != OT_SUCCESS) {
        printf("get fix screen info failed!\n");
        goto EXIT;
    }

    ret = ot_gfb_mmap(info->fd, fix.smem_len, (ot_void**)&show_screen);
    //show_screen = mmap(OT_NULL, fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, info->fd, 0);
    if (ret != OT_SUCCESS) {
        printf("mmap framebuffer failed!\n");
        goto EXIT;
    }

    if (memset_s(show_screen, fix.smem_len, 0x0, fix.smem_len) != OT_SUCCESS) {
        goto exit_unmmap;
    }

    /* only for G0 or G1 */
    if ((info->layer != OT_SUCCESS) && (info->layer != 1)) {
        return OT_SUCCESS;
    } else {
        // draw by cpu in stand mode 
        for (i = 0; i < 1; i++) {
            ptemp = (show_screen + var.yres * fix.line_length * (i % 2)); /* 2 for 0 or 1 */
            sample_oge_draw_rect_by_cpu(ptemp, &var, oge_attr);
            sleep(2);
        }
        
        // oge set alpha and colorkey test
        /*
        * note : not acting on ARGB8888, for ARGB8888 format image's alpha, you can change ptemp[x][y]'s value
        * GFB_RED_8888 = 0xffff00000 means alpha=255(show),red.0x00ff0000 means alpha=0,red(hide).
        */
        if (sample_oge_put_alpha_and_colorkey(info, oge_attr) != OT_SUCCESS) {
            goto exit_unmmap;
        }
    }

    // draw by cpu pan display with smr
    if (sample_oge_cpu_draw_show(info, show_screen, oge_attr, &var, &fix) != OT_SUCCESS) {
        printf("sample oge show bitmap failed \n");
        goto exit_unmmap;
    }

    ot_gfb_munmap(show_screen, fix.smem_len);
    show = OT_FALSE;
    if (ot_gfb_set_display_state(info->fd, show) != OT_SUCCESS) {
        printf("ot_gfb_set_display_state failed!\n");
        goto EXIT;
    }

    return OT_SUCCESS;

exit_unmmap:
    ot_gfb_munmap(show_screen, fix.smem_len);
    show_screen = OT_NULL;
EXIT:

    return OT_FAILURE;
}

static ot_s32 sample_init_layer_info(oge_gfb_sample_info *info) {
    ot_eis_fb_layer_info layer_info = {0};
    // {0,1,2,3,4}={0buffer refresh, 1buffer, 2buffer, 2buffer immdediate, 0buffer pan display}
    switch(info->mode_info) {
        case GFB_MODE_EXTEND_ZERO_REFRESH:
            layer_info.buf_mode = OT_EIS_FB_LAYER_BUF_NONE;
            layer_info.mask = OT_EIS_FB_LAYER_MASK_BUF_MODE;
            break;
        case GFB_MODE_EXTEND_ONE_BUFFER:
            layer_info.buf_mode = OT_EIS_FB_LAYER_BUF_ONE;
            layer_info.mask = OT_EIS_FB_LAYER_MASK_BUF_MODE;
            break;
        case GFB_MODE_EXTEND_DOUBLE_BUFFER:
            layer_info.buf_mode = OT_EIS_FB_LAYER_BUF_DOUBLE;
            layer_info.mask = OT_EIS_FB_LAYER_MASK_BUF_MODE;
            break;
        case GFB_MODE_EXTEND_DOUBLE_IMMDEDIATE:
            layer_info.buf_mode = OT_EIS_FB_LAYER_BUF_DOUBLE_IMMEDIATE;
            layer_info.mask = OT_EIS_FB_LAYER_MASK_BUF_MODE;
            break;
        case GFB_MODE_STAND_ZERO_BUFFER:
            layer_info.buf_mode = OT_EIS_FB_LAYER_BUF_NONE;
            layer_info.mask = 0;
            break;
        default:
            layer_info.buf_mode = OT_EIS_FB_LAYER_BUF_NONE;
            layer_info.mask = 0;
    }

    if (ot_gfb_set_layer_info(info->fd, &layer_info) != OT_SUCCESS) {
        printf("Error: init layer info failed \n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_quick_fill(ot_s32 handle, const ot_oge_none_src *none_src, ot_u32 fill_color, ot_u32 pos_x, ot_u32 pos_y, ot_u32 width, ot_u32 height)
{
    none_src->dst_rect->pos_x = pos_x; // User-changeable
    none_src->dst_rect->pos_y = pos_y; // User-changeable
    none_src->dst_rect->width = width; // User-changeable
    none_src->dst_rect->height = height; // User-changeable
    if (ot_oge_quick_fill(handle, none_src, fill_color)) {
        printf("Error ot_oge_quick_fill failed \n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_draw_rect_corner_by_line(oge_gfb_sample_info *info, ot_eis_fb_buf *canvas_buf, ot_void *viraddr, sample_oge_attr *oge_attr)
{
    ot_oge_handle handle;
    ot_oge_none_src none_src = {0};
    ot_oge_surface dst_surface = {0};
    ot_oge_rect dst_rect = {0};

    none_src.dst_rect = &dst_rect;
    none_src.dst_surface = &dst_surface;
    dst_surface.color_format = oge_attr->oge_clr_fmt;
    dst_surface.width = SAMPLE_OGE_WIDTH_1920;
    dst_surface.height = SAMPLE_OGE_HEIGHT_1080;
    dst_surface.stride = SAMPLE_OGE_WIDTH_1920 * (oge_attr->byte_per_pixel);
    dst_surface.phys_addr = g_phyaddr;

    handle = ot_oge_begin_job();
    if (handle == OT_ERR_OGE_INVALID_HANDLE || handle == OT_ERR_OGE_DEV_NOT_OPEN) {
        printf("begin job failed \n");
        return OT_FAILURE;
    }

    // left of rectangle
    if (sample_quick_fill(handle, &none_src, oge_attr->color, 100, 100, 20, 540)) {
        printf("Error call sample quick fill failed \n");
        return OT_FAILURE;
    }

    // up of rectangle
    if (sample_quick_fill(handle, &none_src, oge_attr->color, 100, 100, 540, 20)) {
        printf("Error call sample quick fill failed \n");
        return OT_FAILURE;
    }

    // right of rectangle
    if (sample_quick_fill(handle, &none_src, oge_attr->color, 620, 100, 20, 540)) {
        printf("Error call sample quick fill failed \n");
        return OT_FAILURE;
    }

    // down of rectangle
    if (sample_quick_fill(handle, &none_src, oge_attr->color, 100, 620, 540, 20)) {
        printf("Error call sample quick fill failed \n");
        return OT_FAILURE;
    }

    ot_oge_end_job(handle, OT_FALSE, OT_TRUE, 60);
    if (ot_gfb_refresh_display_info(info->fd, canvas_buf) != OT_SUCCESS) {
        printf("Error: call ot_gfb_refresh_display_info failed \n");
        return OT_FAILURE;
    }
    sleep(2);
    if (sample_clear_screen(viraddr, oge_attr->byte_per_pixel, canvas_buf) != OT_SUCCESS) {
        printf("ERROR: call memset screen failed \n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

ot_s32 sample_init_canvas(oge_gfb_sample_info *info, ot_eis_fb_buf *canvas_buf, ot_phys_addr *g_phyaddr, void* buf, sample_oge_attr *oge_attr)
{
    canvas_buf->canvas.phys_addr = *g_phyaddr;
    canvas_buf->canvas.width = SAMPLE_OGE_WIDTH_1920;
    canvas_buf->canvas.height = SAMPLE_OGE_HEIGHT_1080;
    canvas_buf->canvas.pitch = SAMPLE_OGE_WIDTH_1920 * (oge_attr->byte_per_pixel);
    canvas_buf->canvas.format = oge_attr->clr_fmt;
    canvas_buf->update_rect.x = 0;
    canvas_buf->update_rect.y = 0;
    canvas_buf->update_rect.width = SAMPLE_OGE_WIDTH_1920;
    canvas_buf->update_rect.height = SAMPLE_OGE_HEIGHT_1080;

    if (sample_clear_screen(buf, oge_attr->byte_per_pixel, canvas_buf) != OT_SUCCESS) {
        printf("ERROR: call memset screen failed \n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static void sample_excute_draw_line(ot_void *buf, ot_eis_fb_color_format clr_fmt, ot_u32 x, ot_u32 y)
{
    if (clr_fmt == OT_EIS_FB_FORMAT_ARGB8888) {
        *((ot_u32*)buf + y * SAMPLE_OGE_WIDTH_1920 + x) = SAMPLE_OGE_RED_8888;
    } else if (clr_fmt == OT_EIS_FB_FORMAT_RGB565) {
        *((ot_u16*)buf + y * SAMPLE_OGE_WIDTH_1920 + x) = SAMPLE_OGE_RED_565;
    } else {
        *((ot_u16*)buf + y * SAMPLE_OGE_WIDTH_1920 + x) = SAMPLE_OGE_RED_1555;
    }
}

static ot_s32 sample_oge_draw_line_by_cpu_mode(oge_gfb_sample_info *info, ot_eis_fb_buf *canvas_buf, ot_void *buf, sample_oge_attr *oge_attr)
{
    ot_u32 x, y;
    // draw row line
    for (y = (SAMPLE_OGE_HEIGHT_1080 / 2 - 2); y < (SAMPLE_OGE_HEIGHT_1080 / 2 + 2); y++) {
        for (x = 0; x < SAMPLE_OGE_WIDTH_1920; x++) {
            sample_excute_draw_line(buf, oge_attr->clr_fmt, x, y);
        }
    }

    // draw col line
    for (y = 0; y < SAMPLE_OGE_HEIGHT_1080; y++) {
        for (x = (SAMPLE_OGE_WIDTH_1920 / 2 - 2); x < (SAMPLE_OGE_WIDTH_1920 / 2 + 2); x++) { /* 2 alg data */
            sample_excute_draw_line(buf, oge_attr->clr_fmt, x, y);
        }
    }

    if (ot_gfb_refresh_display_info(info->fd, canvas_buf) != OT_SUCCESS) {
        printf("Error: call ot_gfb_refresh_display_info failed \n");
        return OT_FAILURE;
    }
    sleep(2);
    if (sample_clear_screen(buf, oge_attr->byte_per_pixel, canvas_buf) != OT_SUCCESS) {
        printf("ERROR: call memset screen failed \n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 sample_oge_draw_line_nums(oge_gfb_sample_info *info,  ot_eis_fb_buf *canvas_buf, ot_void *viraddr, sample_oge_attr *oge_attr)
{
    ot_oge_handle handle;
    ot_oge_surface dst_surface = {0};
    ot_u32 i;
    ot_oge_line line[LINE_NUMS];

    for ( i = 0; i < LINE_NUMS; i++) {
        line[i].start_x = i * LINE_STRIDE + LINE_STRIDE;
        line[i].start_y = i * LINE_STRIDE + LINE_STRIDE;
        line[i].end_x = i * LINE_STRIDE + LINE_STRIDE * 2;
        line[i].end_y = i * LINE_STRIDE + LINE_STRIDE * 2;
        line[i].thick = 20;
        line[i].color = (i%2 == 0 ? oge_attr->color : oge_attr->color_diff);
    }

    dst_surface.color_format = oge_attr->oge_clr_fmt;
    dst_surface.width = SAMPLE_OGE_WIDTH_1920;
    dst_surface.height = SAMPLE_OGE_HEIGHT_1080;
    dst_surface.stride = SAMPLE_OGE_WIDTH_1920 * (oge_attr->byte_per_pixel);
    dst_surface.phys_addr = g_phyaddr;

    handle = ot_oge_begin_job();
    if (handle == OT_ERR_OGE_INVALID_HANDLE || handle == OT_ERR_OGE_DEV_NOT_OPEN) {
        printf("begin job failed \n");
        return OT_FAILURE;
    }

    if (ot_oge_draw_line(handle, &dst_surface, line, LINE_NUMS) != OT_SUCCESS) {
        printf("ot_oge_draw_line failed\n");
        return OT_FAILURE;
    }

    if (ot_oge_end_job(handle, OT_FALSE, OT_TRUE, 10) < 0) { /* 10 timeout */
        ot_oge_cancel_job(handle);
        return OT_FAILURE;
    }
    if (ot_gfb_refresh_display_info(info->fd, canvas_buf) != OT_SUCCESS) {
        printf("Error: call ot_gfb_refresh_display_info failed \n");
        return OT_FAILURE;
    }
    sleep(2);
    if (sample_clear_screen(viraddr, oge_attr->byte_per_pixel, canvas_buf) != OT_SUCCESS) {
        printf("ERROR: call memset screen failed \n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

ot_s32 smr_alloc_common(ot_phys_addr *alloc_phyaddr, ot_void **alloc_viraddr, ot_s32 byte_per_pixel, ot_u32 width, ot_u32 height)
{
    ot_smr_alloc_attr smr_alloc_attr = {0};
    smr_alloc_attr.len = width * height * byte_per_pixel;
    smr_alloc_attr.cached = OT_FALSE;
    strcpy(smr_alloc_attr.region_name, "");
    if (ot_smr_alloc(&smr_alloc_attr, alloc_phyaddr, alloc_viraddr) != OT_SUCCESS) {
        printf("Error:allocate memory failed\n");
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

ot_void smr_load_alpha_bmp_common(ot_char* file_bmp, ot_void *bmp_buf, ot_u32 width, ot_u32 height, ot_u32 byte_per_pixel)
{
    ot_u32 i, j;

    ot_load_bmp_area(file_bmp, bmp_buf, width, height, width * byte_per_pixel);

    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            *((ot_u32 *)bmp_buf + i * width + j) |= 0xFF000000;
        }
    }
}

static ot_s32 sample_oge_draw_line_all(oge_gfb_sample_info *gfb_info, ot_eis_fb_buf *canvas_buf, ot_void *viraddr, sample_oge_attr *oge_attr)
{
    ot_s32 ret = OT_FAILURE;

    for (int i = 0; i < 1; i++) {
        printf("begin to draw line by cpu\n");
        if (sample_oge_draw_line_by_cpu_mode(gfb_info, canvas_buf, viraddr, oge_attr) != OT_SUCCESS) {
            printf("call oge draw line cpu mode failed \n");
            goto EXIT;
        }

        printf("begin to draw line nums by oge\n");
        if (sample_oge_draw_line_nums(gfb_info, canvas_buf, viraddr, oge_attr) != OT_SUCCESS) {
            printf("call oge draw line nums failed \n");
            goto EXIT;
        }

        printf("begin to draw rectangle by line\n");
        if (sample_oge_draw_rect_corner_by_line(gfb_info, canvas_buf, viraddr, oge_attr) != OT_SUCCESS) {
            printf("call oge draw rectangle by line failed \n");
            goto EXIT;
        }
    }
    ret = OT_SUCCESS;

EXIT:

    return ret;
}

static ot_s32 sample_oge_images_all(oge_gfb_sample_info *gfb_info, ot_eis_fb_buf *canvas_buf, ot_void *viraddr, sample_oge_attr *oge_attr
)
{
    ot_s32 ret = OT_FAILURE;
    ot_void *buf = OT_NULL;

    if (smr_alloc_common(&buf_phyaddr, &buf, oge_attr->byte_per_pixel, oge_attr->image_width, oge_attr->image_height) != OT_SUCCESS) {
        printf("Error: smr alloc memory buf_phyaddr failed \n");
        goto EXIT;
    }
    
    switch (oge_attr->clr_fmt) {
        case OT_EIS_FB_FORMAT_ARGB8888:
            printf("begin to load bmp for ARGB8888 \n");
            smr_load_alpha_bmp_common(SAMPLE_BMP_PATH, buf, SAMPLE_OGE_IMAGE_WIDTH, SAMPLE_OGE_IMAGE_HEIGHT, oge_attr->byte_per_pixel);
            break;
        case OT_EIS_FB_FORMAT_RGB565:
            printf("begin to load bmp for RGB565 \n");
            ot_load_bmp_area(SAMPLE_BMP_565_PATH, buf, SAMPLE_OGE_IMAGE_565_WIDTH, SAMPLE_OGE_IMAGE_565_HEIGHT, SAMPLE_OGE_IMAGE_565_WIDTH * (oge_attr->byte_per_pixel));
            break;
        case OT_EIS_FB_FORMAT_ARGB1555:
            printf("begin to load bmp for ARGB1555 \n");
            ot_load_bmp_area(SAMPLE_BMP_1555_PATH, buf, SAMPLE_OGE_IMAGE_1555_WIDTH, SAMPLE_OGE_IMAGE_1555_HEIGHT, SAMPLE_OGE_IMAGE_1555_WIDTH * (oge_attr->byte_per_pixel));
            break;
        default:
            printf("Warning: the input ot_eis_fb_color_format has not case \n");
            goto EXIT;
    }

    for (int i = 0; i < 1; i++) {
        printf("begin to quick copy \n");
        if (sample_oge_quick_copy_by_oge(gfb_info, canvas_buf, viraddr, oge_attr) != OT_SUCCESS) {
            printf("call oge quick copy failed \n");
            goto free_smr_alloc_bmp;
        }

        printf("begin to quick resize \n");
        if (sample_oge_quick_resize_by_oge(gfb_info, canvas_buf, viraddr, oge_attr) != OT_SUCCESS) {
            printf("call oge quick resize failed \n");
            goto free_smr_alloc_bmp;
        }
    }

    ret = OT_SUCCESS;

free_smr_alloc_bmp:
    ot_smr_free(buf_phyaddr);
    buf_phyaddr = 0;
EXIT:

    return ret;
}

static ot_s32 sample_init_vi_vo_smr(sample_vo_attr **vo_attr)
{
    ot_s32 ret = OT_FAILURE;
    ot_eis_img_size img_size = {0};

    sample_comm_vi_get_size_by_sns_type(SENSOR0_TYPE, &img_size);
    *vo_attr = sample_comm_get_vo_attr(ONE_IN_PORT_MIPI_768);
    if (*vo_attr == OT_NULL) {
        printf("Error:Invalid vo attr.\n");
        return OT_FAILURE;
    }

    if (sample_oge_vrb_init() != OT_SUCCESS) {
        printf("Error:init vrb failed.\n");
        return OT_FAILURE;
    }

    if (ot_smr_init() != OT_SUCCESS) {
        printf("Error:init smr error! \n");
        goto deinit_vrb;
    }

    if (sample_comm_start_vo(*vo_attr) != OT_SUCCESS) {
        goto deinit_smr;
    }
    return OT_SUCCESS;

deinit_smr:
    ot_smr_deinit();
deinit_vrb:
    ot_eis_media_deinit();
    ot_vrb_deinit();

    return ret;
}

static ot_void sample_deinit_vi_vo_smr(sample_vo_attr *vo_attr)
{
    if (vo_attr != OT_NULL) {
        sample_comm_stop_vo(vo_attr);
        sample_comm_sys_exit();
    }
}

static ot_s32 sample_init_oge_gfb(oge_gfb_sample_info *gfb_info, ot_eis_fb_buf *canvas_buf, ot_void **viraddr, sample_oge_attr *oge_attr)
{
    ot_s32 ret = OT_FAILURE;

    if (ot_oge_open() < OT_SUCCESS) {
        printf("oge_open failed :%d!\n", ret);
        goto EXIT;
    }

    if (ot_gfb_open(gfb_info->layer, &(gfb_info->fd)) != OT_SUCCESS) {
        printf("gfb open error! ret[0x%x], gfb_info->fd=%d\n", ret, gfb_info->fd);
        goto exit_oge;
    }

    if (sample_oge_init_frame_buffer(gfb_info) != OT_SUCCESS) {
        printf("Error: call sample mul_init_framebuffer failed\n");
        goto exit_gfb;
    }

    if (sample_oge_gfb_init_var_mode(gfb_info, oge_attr) != OT_SUCCESS) {
        printf("Error: call sample gfb_init_var failed\n");
        goto exit_gfb;
    }

    if (sample_init_layer_info(gfb_info) != OT_SUCCESS) {
        printf("Error: call sample init layer info failed\n");
        goto exit_gfb;
    }

    ot_gfb_set_display_state(gfb_info->fd, OT_TRUE);

    if (smr_alloc_common(&g_phyaddr, viraddr, oge_attr->byte_per_pixel, SAMPLE_OGE_WIDTH_1920, SAMPLE_OGE_HEIGHT_1080) != OT_SUCCESS) {
        printf("Error: smr alloc memory g_phyaddr failed \n");
        goto exit_gfb;
    }

    if (sample_init_canvas(gfb_info, canvas_buf, &g_phyaddr, *viraddr, oge_attr) != OT_SUCCESS) {
        printf("call oge init canvas failed \n");
        goto free_smr;
    }
    return OT_SUCCESS;

free_smr:
    ot_smr_free(g_phyaddr);
    g_phyaddr = 0;
exit_gfb:
    ot_gfb_close(gfb_info->fd);
exit_oge:
    ot_oge_close();
EXIT:

    return ret;
}

static void* sample_extend_mode_display(ot_void* info) {
    oge_gfb_sample_info *gfb_info = OT_NULL;
    ot_eis_fb_buf canvas_buf;
    ot_void *viraddr = OT_NULL;

    if (info == OT_NULL) {
        printf("Error: input args of sample extend_mode_display is null\n");
        goto exit_null;
    }

    gfb_info = (oge_gfb_sample_info *) info;

    sample_oge_attr *oge_attr = (sample_oge_attr *)malloc(sizeof(sample_oge_attr));
    if (oge_attr == OT_NULL) {
        printf("Error: sample oge attr malloc failed\n");
        goto exit_null;
    }
    sample_oge_get_param(gfb_info, oge_attr);

    if (sample_init_oge_gfb(gfb_info, &canvas_buf, &viraddr, oge_attr) != OT_SUCCESS) {
        printf("call init oge or gfb failed \n");
        goto free_oge_attr;
    }

    if (sample_oge_draw_line_all(gfb_info, &canvas_buf, viraddr, oge_attr) != OT_SUCCESS) {
        printf("call oge draw lines all failed \n");
        goto free_smr;
    }

    if (sample_oge_images_all(gfb_info, &canvas_buf, viraddr, oge_attr) != OT_SUCCESS) {
        printf("call oge draw images all failed \n");
        goto free_smr;
    }
    printf("show extend_mode_display success\n");

free_smr:
    ot_smr_free(g_phyaddr);
    g_phyaddr = 0;
    ot_gfb_close(gfb_info->fd);
    ot_oge_close();
free_oge_attr:
    if (oge_attr != OT_NULL) {
        free(oge_attr);
        oge_attr = OT_NULL;
    }
exit_null:

    return OT_NULL;
}

static void* sample_layer_pan_display(ot_void* info) {
    ot_s32 ret = OT_FAILURE;
    oge_gfb_sample_info *gfb_info = OT_NULL;

    if (info == OT_NULL) {
        printf("Error: sample layer_pan_display failed, info is null\n");
        return OT_NULL;
    }

    sample_oge_attr *oge_attr = (sample_oge_attr *)malloc(sizeof(sample_oge_attr));
    if (oge_attr == OT_NULL) {
        printf("Error: sample oge attr malloc failed\n");
        goto exit_null;
    }
    gfb_info = (oge_gfb_sample_info *) info;
    sample_oge_get_param(gfb_info, oge_attr);

    ret = ot_gfb_open(gfb_info->layer, &gfb_info->fd);
    if (ret != OT_SUCCESS) {
        printf("open error! ret[0x%x], gfb_info->fd=%d\n", ret, gfb_info->fd);
        goto free_oge_attr;
    }

    ret = sample_oge_init_frame_buffer(gfb_info);
    if (ret != OT_SUCCESS) {
        printf("Error: sample mul_init_framebuffer failed\n");
        goto close_gfb;
    }

    ret = sample_oge_gfb_init_var(gfb_info, oge_attr);
    if (ret != OT_SUCCESS) {
        printf("Error: call sample oge_gfb_init_var failed\n");
        goto close_gfb;
    }

    ret = sample_oge_draw_line(gfb_info, oge_attr);
    if (ret != OT_SUCCESS) {
        printf("Error: call sample__oge_draw_line failed\n");
        goto close_gfb;
    }

close_gfb:
    ot_gfb_close(gfb_info->fd);
free_oge_attr:
    if (oge_attr != OT_NULL) {
        free(oge_attr);
        oge_attr = OT_NULL;
    }
exit_null:

    return OT_NULL;
}

static ot_void sample_oge_main_thread_exit(ot_void) {
    ot_char ch;
    while (1)
    {  
        int c = getchar();
        printf("\npress 'q' to exit sample in mul thread\n");
        ch = (char)c;
        if (ch == 'q') {
            g_exit_flag = ch;
            break;
        } else {
            printf("input invalid! please try again.\n");
        }
    }
    if (g_oge_thread0 != 0) {
        pthread_join(g_oge_thread0, 0);
        g_oge_thread0 = 0;
    }

    return;
}

static int oge_draw_line_pandisplay()
{
    oge_gfb_sample_info gfb_info = {0};
    sample_vo_attr *vo_attr = OT_NULL;

    if (sample_init_vi_vo_smr(&vo_attr) != OT_SUCCESS) {
        printf("Error: init vi vo failed \n");
        return OT_FAILURE;
    }

    //start gfb
    gfb_info.layer = 0;
    gfb_info.fd = -1;
    gfb_info.mode_info = GFB_MODE_STAND_ZERO_BUFFER; /* 0buffer pandisplay */
    gfb_info.compress = OT_FALSE; /* compress opened or not */
    gfb_info.color_format = OT_EIS_FB_FORMAT_ARGB8888;
    if (USE_THREAD == 1) {
        printf("use thread \n");
        if (pthread_create(&g_oge_thread0, 0, sample_layer_pan_display, (ot_void *)(&gfb_info)) != OT_SUCCESS) {
            printf("Error: start oge thread0 failed\n");
            goto deinit_vi_vo_smr;
        }
        sample_oge_main_thread_exit();
    } else {
        sample_layer_pan_display((ot_void *)(&gfb_info));
    }

deinit_vi_vo_smr:
    sample_deinit_vi_vo_smr(vo_attr);

    return 0;

}

static int oge_draw_rect_cornerbox()
{
    ot_s32 ret = OT_FAILURE;
    int j = 0;
    struct fb_var_screeninfo var;
    struct fb_fix_screeninfo fix;
    ot_eis_fb_color_format color_format = OT_EIS_FB_FORMAT_4BPP;
    ot_void *viraddr = OT_NULL;
    ot_eis_img_size img_size = {0};
    sample_vo_attr *vo_attr = OT_NULL;

    sample_comm_vi_get_size_by_sns_type(SENSOR0_TYPE, &img_size);
    vo_attr = sample_comm_get_vo_attr(ONE_IN_PORT_MIPI_768);
    if (vo_attr == OT_NULL) {
        printf("Error:Invalid vo attr.\n");
        return OT_FAILURE;
    }

    ret = sample_oge_vrb_init();
    if (ret != OT_SUCCESS) {
        printf("Error:init vrb failed.\n");
        return ret;
    }

    ret = sample_comm_start_vo(vo_attr);
    if (ret != OT_SUCCESS) {
        printf("Error:start vo failed.\n");
        goto start_vo_failed;
    }

    ret = ot_gfb_open(0, &fd);
    if (ret != OT_SUCCESS) {
        printf("Error: sample oge gfb init error! ret[0x%x]\n", ret);
        goto var_init_fail;
    }

    ret = sample_oge_var_init_corner_box(color_format, &var);
    if (ret != OT_SUCCESS) {
        goto close_gfb;
    }

    ret = sample_oge_cmap_init();
    if (ret != OT_SUCCESS) {
        printf("Error: init cmap failed.\n");
        goto close_gfb;
    }

    ret = sample_oge_fix_and_mmap(&fix, &viraddr);
    if (ret != OT_SUCCESS) {
        goto mmap_fail;
    }

    ret = ot_oge_open();
    if (ret < 0) {
        printf("oge_open failed :%d!\n", ret);
        goto close_gfb;
    }

    while (j < 10) { /* 10 times */
        if (sample_oge_draw_rect(viraddr, j, &var, &fix) != OT_SUCCESS) {
            goto close_oge;
        }
        if (sample_oge_pandisplay(j, &var) != OT_SUCCESS) {
            goto close_oge;
        }

        if (sample_oge_draw_rect_nums(viraddr, j, &var, &fix) != OT_SUCCESS) {
            goto close_oge;
        }
        if (sample_oge_pandisplay(j, &var) != OT_SUCCESS) {
            goto close_oge;
        }
        j++;
    }
    
close_oge:
    ot_oge_close();
mmap_fail:
    ot_gfb_munmap(viraddr, SAMPLE_OGE_WIDTH_1920 * SAMPLE_OGE_HEIGHT_1080);
close_gfb:
    ot_gfb_close(fd);
var_init_fail:
    sample_comm_stop_vo(vo_attr);
start_vo_failed:
    sample_comm_sys_exit();
    printf("[end]\n");

    return 0;
}

static ot_void oge_help_info()
{
    printf("usage : OGE <index> \n");
    printf("index:\n");
    printf("    (0) oge extend mode none buffer\n");
    printf("    (1) oge extend mode one buffer\n");
    printf("    (2) oge extend mode double buffer\n");
    printf("    (3) oge extend mode double buffer immediate \n");
    printf("    (4) oge stand mode pandisplay\n");
    printf("    (5) oge draw rect with corner box\n");
}

static ot_s32 sample_oge_init_buf_mode_info(ot_u32 buf_mode, oge_gfb_sample_info *gfb_info)
{
    switch (buf_mode) {
        case 0: // none buffer mode
            gfb_info->layer = 0;
            gfb_info->fd = -1;
            gfb_info->mode_info = GFB_MODE_EXTEND_ZERO_REFRESH; /* 0: none buffer */
            gfb_info->compress = OT_FALSE; /* compress opened or not */
            gfb_info->color_format = OT_EIS_FB_FORMAT_ARGB8888;
            break;
        case 1: // one buffer mode
            gfb_info->layer = 0;
            gfb_info->fd = -1;
            gfb_info->mode_info = GFB_MODE_EXTEND_ONE_BUFFER; /* 1: 1 buffer */
            gfb_info->compress = OT_FALSE; /* compress opened or not */
            gfb_info->color_format = OT_EIS_FB_FORMAT_RGB565;
            break;
        case 2: // double buffer mode
            gfb_info->layer = 0;
            gfb_info->fd = -1;
            gfb_info->mode_info = GFB_MODE_EXTEND_DOUBLE_BUFFER; /* 2: double buffer */
            gfb_info->compress = OT_FALSE; /* compress opened or not */
            gfb_info->color_format = OT_EIS_FB_FORMAT_ARGB1555;
            break;
        case 3: // double buffer mode immediate
            gfb_info->layer = 0;
            gfb_info->fd = -1;
            gfb_info->mode_info = GFB_MODE_EXTEND_DOUBLE_IMMDEDIATE; /* 2: double buffer immediate*/
            gfb_info->compress = OT_FALSE; /* compress opened or not */
            gfb_info->color_format = OT_EIS_FB_FORMAT_ARGB1555;
            break;
        default :
            printf("Error: buf mode is invalid\n");
            return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static int oge_draw_buf_mode(ot_u32 buf_mode) {
    oge_gfb_sample_info gfb_info = {0};
    sample_vo_attr *vo_attr = OT_NULL;

    if (sample_init_vi_vo_smr(&vo_attr) != OT_SUCCESS) {
        printf("Error: init vi vo failed \n");
        return OT_FAILURE;
    }

    if (sample_oge_init_buf_mode_info(buf_mode, &gfb_info) != OT_SUCCESS) {
        printf("Error: init buf mode info failed \n");
        return OT_FAILURE;
    }

    if (USE_THREAD == 1) {
        printf("use thread \n");
        if (pthread_create(&g_oge_thread0, 0, sample_extend_mode_display, (ot_void *)(&gfb_info)) != OT_SUCCESS) {
            printf("Error: start oge thread0 failed\n");
            goto deinit_vi_vo_smr;
        }
        sample_oge_main_thread_exit();
    } else {
        sample_extend_mode_display((ot_void *)(&gfb_info));
    }    

deinit_vi_vo_smr:
    sample_deinit_vi_vo_smr(vo_attr);

    return OT_SUCCESS;
}

static ot_s32 sample_oge_execute_case(ot_u32 case_index)
{
    ot_s32 ret;

    switch (case_index) {
        case 0:  // none buffer mode
            ret = oge_draw_buf_mode(0);
            break;
        case 1:  // one buffer mode
            ret = oge_draw_buf_mode(1);
            break;
        case 2:  // double buffer mode
            ret = oge_draw_buf_mode(2);
            break;
        case 3: // double buffer mode immediate
            ret = oge_draw_buf_mode(3);
            break;
        case 4: /* draw line with 0buffer pandisplay*/
            ret = oge_draw_line_pandisplay();
            break;
        case 5: /* draw rect corner box */
            ret = oge_draw_rect_cornerbox();
            break;
        default:
            ret = OT_FAILURE;
            break;
    }

    return ret;
}

ot_s32 main(ot_s32 argc, ot_char *argv[])
{
    ot_s32 ret = OT_FAILURE;
    ot_u32 index = 0;

    if (argc != 2) { /* 2:arg num */
        oge_help_info();
        return OT_FAILURE;
    }

    if (!strncmp(argv[1], "-h", 2)) { /* 2:arg num */
        oge_help_info();
        return OT_FAILURE;
    }

    if (strlen(argv[1]) >= 2 || strlen(argv[1]) <= 0 || !check_digit(argv[1][0])) { /* 2:arg len */
        oge_help_info();
        return OT_FAILURE;
    }

    index = strtol(argv[1], NULL, 10); /* base 10 */
    ret = sample_oge_execute_case(index);
    if (ret == OT_SUCCESS) {
        printf("\033[0;32mprogram exit normally!\033[0;39m\n");
    } else {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    return ret;
}
