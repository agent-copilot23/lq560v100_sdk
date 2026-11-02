/*
  Copyright (c), 2021-2022, Oritek Tech. Co., Ltd.
 */
#ifndef PARTITIONS_PRIV_H
#define PARTITIONS_PRIV_H

#include <stdint.h>
#include <stdio.h>

#ifdef DEBUG
#define pt_trace(format,arg...) printf("[trace] %s:%d" "  " format"",__func__, __LINE__,##arg);fflush(0)
#define pt_info(format,arg...) printf("[info] %s:%d" "  " format"",__func__, __LINE__,##arg);fflush(0)
#else
#define pt_trace(format,arg...)
#define pt_info(format,arg...)
#endif

#define pt_error(format,arg...) printf("[error] %s:%d" "  " format"",__func__, __LINE__,##arg);fflush(0)

#define uswap_16(x) \
	((((x) & 0xff00) >> 8) | \
	 (((x) & 0x00ff) << 8))
#define uswap_32(x) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24))
#define _uswap_64(x, sfx) \
	((((x) & 0xff00000000000000##sfx) >> 56) | \
	 (((x) & 0x00ff000000000000##sfx) >> 40) | \
	 (((x) & 0x0000ff0000000000##sfx) >> 24) | \
	 (((x) & 0x000000ff00000000##sfx) >>  8) | \
	 (((x) & 0x00000000ff000000##sfx) <<  8) | \
	 (((x) & 0x0000000000ff0000##sfx) << 24) | \
	 (((x) & 0x000000000000ff00##sfx) << 40) | \
	 (((x) & 0x00000000000000ff##sfx) << 56))
#if defined(__GNUC__)
# define uswap_64(x) _uswap_64(x, ull)
#else
# define uswap_64(x) _uswap_64(x, )
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
# define cpu_to_le16(x)		(x)
# define cpu_to_le32(x)		(x)
# define cpu_to_le64(x)		(x)
# define le16_to_cpu(x)		(x)
# define le32_to_cpu(x)		(x)
# define le64_to_cpu(x)		(x)
# define cpu_to_be16(x)		uswap_16(x)
# define cpu_to_be32(x)		uswap_32(x)
# define cpu_to_be64(x)		uswap_64(x)
# define be16_to_cpu(x)		uswap_16(x)
# define be32_to_cpu(x)		uswap_32(x)
# define be64_to_cpu(x)		uswap_64(x)
#else
# define cpu_to_le16(x)		uswap_16(x)
# define cpu_to_le32(x)		uswap_32(x)
# define cpu_to_le64(x)		uswap_64(x)
# define le16_to_cpu(x)		uswap_16(x)
# define le32_to_cpu(x)		uswap_32(x)
# define le64_to_cpu(x)		uswap_64(x)
# define cpu_to_be16(x)		(x)
# define cpu_to_be32(x)		(x)
# define cpu_to_be64(x)		(x)
# define be16_to_cpu(x)		(x)
# define be32_to_cpu(x)		(x)
# define be64_to_cpu(x)		(x)
#endif


unsigned long long memparse(const char *ptr, char **retptr);
char* read_file(const char *filename);
void save_bin(const char *path, uint8_t *content, uint32_t size);
void save_env(const char *path, const char *content);



#endif  /*PARTITIONS_PRIV_H*/