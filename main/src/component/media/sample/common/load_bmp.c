/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include "load_bmp.h"

ot_ulong ot_file_read(
    ot_void * ptr,
    ot_ulong size,
    ot_void *p
    )
{
    return fread(ptr, 1, size, (FILE*)p);
}

ot_s32 ot_file_seek(
    ot_void *p,
    ot_ulong offset,
    ot_bool end
    )
{
    if (end)
    {
        return fseek((FILE*)p, offset, SEEK_END);
    }
    return fseek((FILE*)p, offset, SEEK_SET);
}

ot_u8 ot_file_read_byte(
    ot_void *p
    )
{
    FILE *fp = (FILE *) p;
    ot_u8 b0;

    b0 = getc(fp);

    return b0;
}

ot_u16 ot_file_read_word(
    ot_void *p
    )
{
    FILE *fp = (FILE *) p;
    ot_u8 b0, b1;

    b0 = getc(fp);
    b1 = getc(fp);

    return ((b1 << 8) | b0);
}

ot_u32 ot_file_read_dword(
    ot_void *p
    )
{
    FILE *fp = (FILE *) p;
    ot_u8 b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

ot_s32 ot_file_read_long(
    ot_void *p
    )
{
    FILE *fp = (FILE *) p;
    ot_u8 b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((ot_s32)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

ot_u32 ot_file_tell(
    ot_void *p
    )
{
    return ftell((FILE *)p);
}

ot_s32 ot_file_write(
    ot_void *ptr,
    ot_ulong size,
    ot_void *p
    )
{
    return fwrite(ptr, 1, size, (FILE *)p);
}

ot_s32 ot_file_write_word(ot_void *p, ot_u16 w)
{
    FILE *fp = (FILE *)p;
    putc(w, fp);
    return (putc(w >> 8, fp));
}

ot_s32 ot_file_write_dword(ot_void *p, ot_u32 dw)
{
    FILE *fp = (FILE *)p;
    putc(dw, fp);
    putc(dw >> 8, fp);
    putc(dw >> 16, fp);
    return (putc(dw >> 24, fp));
}

ot_s32 ot_file_write_long(ot_void *p, ot_s32  l)
{
    FILE *fp = (FILE *)p;
    putc(l, fp);
    putc(l >> 8, fp);
    putc(l >> 16, fp);
    return (putc(l >> 24, fp));
}

ot_s32 ot_file_close(ot_void *p)
{
    return fclose((FILE*)p);
}


ot_s32 ot_load_bmp_area(
    ot_char* file_name,
    ot_u8* buf,
    ot_u32 buf_width,
    ot_u32 buf_height,
    ot_u32 buf_stride
    )
{
    ot_s32 ret = -1;
    ot_u32 i, j;
    ot_u32 offset, bpp;
    ot_u32 img_width, img_height, img_stride;
    ot_u32 width, height, stride;
    ot_s32 h;
    ot_void *fp = OT_NULL;

    ot_u8 *img_buf = OT_NULL;

    if (!file_name|| !buf)
    {
        return ret;
    }

    fp = fopen((ot_char *)file_name, "rb");
    if (!fp)
    {
        printf("Can not find %s\n", file_name);
        goto exit;
    }

    ot_file_read_word(fp);
    ot_file_read_dword(fp);
    ot_file_read_word(fp);
    ot_file_read_word(fp);
    offset = ot_file_read_dword(fp);
    ot_file_read_dword(fp);
    img_width = ot_file_read_long(fp);
    h = ot_file_read_long(fp);
    img_height = OT_ABS(h);
    ot_file_read_word(fp);
    bpp = ot_file_read_word(fp);
    img_stride = (img_width*bpp/8+3)&(~3);

    width  = OT_MIN(buf_width, img_width);
    height = OT_MIN(buf_height, img_height);
    stride = OT_MIN(buf_stride, img_stride);
    printf("bmp width:%d\n", width);
    printf("bmp height:%d\n", height);
    printf("bmp stride:%d\n", stride);
    printf("bmp bpp:%d\n", bpp);
    printf("bmp h:%d\n", h);

    /* biCompression. */
    ot_file_read_dword(fp);
    /* biSizeImage. */
    ot_file_read_dword(fp);
    /* biXPelsPerMeter*/
    ot_file_read_long(fp);
    /* biYPelsPerMeter*/
    ot_file_read_long(fp);

    /* biClrUsed. */
    ot_file_read_dword(fp);
    /* biClrImportant. */
    ot_file_read_dword(fp);

    if (ot_file_seek(fp, offset, OT_FALSE) == 0)
    {
        ot_u8 *bits = OT_NULL;
        ot_u32 pos;
        ot_u8 *img_bits = OT_NULL;

        for (i = 0; i < height; i ++)
        {
            /* h < 0 || tile_height == 1 */
            bits = (h >= 0)
                ? (ot_u8 *)buf + (height - i -1) * buf_stride
                : (ot_u8 *)buf + i * buf_stride;
            pos = ot_file_tell(fp);

            if (bpp == 24)
            {
                for (j = 0; j < width; j++)
                {
                    if (ot_file_read(bits + j * 4, 3, fp) < 3)
                    {
                        ret = -1;
                        goto exit;
                    }
                }
            }
            else
            {
                if (ot_file_read(bits, stride, fp) < stride)
                {
                    printf("stride = %d\n", stride);
                    ret = -1;
                    goto exit;
                }
            }

            /* Move to the next position. */
            ot_file_seek(fp, pos + img_stride, OT_FALSE);
        }
    }
exit:
    if (fp)
    {
        ot_file_close(fp);
    }

    return ret;
}