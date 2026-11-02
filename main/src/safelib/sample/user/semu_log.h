#ifndef SEMU_LOG_H
#define SEMU_LOG_H

#include <stdio.h>

#define SEMU_LOG_LEVEL_NONE         0U
#define SEMU_LOG_LEVEL_ERROR        3U
#define SEMU_LOG_LEVEL_WARNING      4U
#define SEMU_LOG_LEVEL_INFO         6U
#define SEMU_LOG_LEVEL_DEBUG        7U

/* 后续在编译时指定 */
#define SEMU_LOG_LEVEL SEMU_LOG_LEVEL_INFO

#define semu_level_fmt(level, fmt) "SEMU [Func: %s] " level ": " fmt, __FUNCTION__

# define semu_error(fmt, ...)\
    if (SEMU_LOG_LEVEL >= SEMU_LOG_LEVEL_ERROR) printf(semu_level_fmt("Error", fmt), ##__VA_ARGS__)

# define semu_warning(fmt, ...)\
    if (SEMU_LOG_LEVEL >= SEMU_LOG_LEVEL_WARNING) printf(semu_level_fmt("Warning", fmt), ##__VA_ARGS__)

# define semu_info(fmt, ...)\
    if (SEMU_LOG_LEVEL >= SEMU_LOG_LEVEL_INFO) printf(semu_level_fmt("Info", fmt), ##__VA_ARGS__)

# define semu_debug(fmt, ...)\
    if (SEMU_LOG_LEVEL >= SEMU_LOG_LEVEL_DEBUG) printf(semu_level_fmt("Debug", fmt), ##__VA_ARGS__)

#endif