/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */


#ifndef OT_SCENECOMM_H
#define OT_SCENECOMM_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "ot_eis_type.h"
#include "ot_scenecomm_log.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* * Pointer Check */
#define ot_scenecomm_check_pointer_return(p, errcode) do { \
        if ((p) == NULL) {                                 \
            scene_loge("null pointer\n");                  \
            return errcode;                                \
        }                                                  \
    } while (0)

/* * Expression Check */
#define ot_scenecomm_check_expr_return(expr, errcode) do {      \
        if ((expr) == 0) {                                      \
            scene_loge(RED " expr[%s] false" NONE "\n", #expr); \
            return errcode;                                     \
        }                                                       \
    } while (0)

/* * Expression Check */
#define ot_scenecomm_expr_true_return(expr, errcode) do {       \
        if ((expr) == 1) {                                      \
            scene_loge(RED " expr[%s] false" NONE "\n", #expr); \
            return errcode;                                     \
        }                                                       \
    } while (0)

/* * Return Result Check */
#define ot_scenecomm_check_return(ret, errcode) do {     \
        if ((ret) != OT_SUCCESS) {                         \
            scene_loge(RED " ret[%08x]" NONE "\n", ret); \
            return errcode;                              \
        }                                                \
    } while (0)

/* * Goto Result Check */
#define ot_scenecomm_check_goto(ret, ext) do {     \
        if ((ret) != OT_SUCCESS) {                         \
            scene_loge(RED " ret[%08x]" NONE "\n", ret); \
            goto ext;                              \
        }                                                \
    } while (0)

/* * Expression Goto */
#define ot_scenecomm_expr_true_goto(expr, ext) do {     \
        if ((expr) == 1) {                             \
            scene_loge(RED " ret[%08x]" NONE "\n", expr); \
            goto ext;                              \
        }                                                \
    } while (0)

#define ot_scenecomm_check(ret, errcode) do {                                 \
        if ((ret) != OT_SUCCESS) {                                              \
            scene_loge(RED " ret[%08x] errcode[%x]" NONE "\n", ret, errcode); \
        }                                                                     \
    } while (0)


/* * Return Result Check With ErrInformation */
#define ot_scenecomm_check_return_with_errinfo(ret, errcode, errstring) do { \
        if ((ret) != OT_SUCCESS) {                                             \
            scene_loge(RED " [%s] failed[%08x]" NONE "\n", errstring, ret);  \
            return errcode;                                                  \
        }                                                                    \
    } while (0)

/* * Memory Safe Free */
#define ot_scenecomm_safe_free(p) do { \
        if ((p) != NULL) {             \
            free(p);                   \
            (p) = NULL;                \
        }                              \
    } while (0)

/* * Mutex Lock */
#ifndef ot_mutex_init_lock
#define ot_mutex_init_lock(mutex) do {          \
        (void)pthread_mutex_init(&(mutex), NULL); \
    } while (0)
#endif
#ifndef ot_mutex_lock
#define ot_mutex_lock(mutex) do {         \
        (void)pthread_mutex_lock(&(mutex)); \
    } while (0)
#endif
#ifndef ot_mutex_unlock
#define ot_mutex_unlock(mutex) do {         \
        (void)pthread_mutex_unlock(&(mutex)); \
    } while (0)
#endif
#ifndef ot_mutex_destroy
#define ot_mutex_destroy(mutex) do {         \
        (void)pthread_mutex_destroy(&(mutex)); \
    } while (0)
#endif

/* * RWLock */
#ifndef ot_rwlock_init_rwlock
#define ot_rwlock_init_rwlock(mutex) do {          \
        (void)pthread_rwlock_init(&(mutex), NULL); \
    } while (0)
#endif
#ifndef ot_rwlock_rdlock
#define ot_rwlock_rdlock(mutex) do {         \
        (void)pthread_rwlock_rdlock(&(mutex)); \
    } while (0)
#endif
#ifndef ot_rwlock_wrlock
#define ot_rwlock_wrlock(mutex) do {         \
        (void)pthread_rwlock_wrlock(&(mutex)); \
    } while (0)
#endif
#ifndef ot_rwlock_unlock
#define ot_rwlock_unlock(mutex) do {         \
        (void)pthread_rwlock_unlock(&(mutex)); \
    } while (0)
#endif
#ifndef ot_rwlock_destroy
#define ot_rwlock_destroy(mutex) do {         \
        (void)pthread_rwlock_destroy(&(mutex)); \
    } while (0)
#endif

/* * cond */
#ifndef ot_cond_init
#define ot_cond_init(cond, mutex) do {          \
        (void)pthread_cond_init(&(cond), NULL); \
        (void)pthread_mutex_init(&(mutex), NULL); \
    } while (0)
#endif

#ifndef ot_cond_timedwait
#define ot_cond_timedwait(cond, mutex, outtime_ms) do {          \
        struct timespec ts; \
        clock_gettime(CLOCK_REALTIME, &ts); \
        if (outtime_ms < 0) {outtime_ms = 1000*60*60;} \
        ot_u64 nsec = ts.tv_nsec + outtime_ms * 1000000; \
        ot_u64 sec = nsec / 1000000000; \
        ts.tv_sec += sec; \
        ts.tv_nsec = (nsec - sec * 1000000000); \
        (void)pthread_mutex_lock(&(mutex)); \
        (void)pthread_cond_timedwait(&(cond), &(mutex), &(ts)); \
        (void)pthread_mutex_unlock(&(mutex)); \
    } while (0)
#endif

#ifndef ot_cond_signal
#define ot_cond_signal(cond, mutex) do {     \
        (void)pthread_mutex_lock(&(mutex)); \
        (void)pthread_cond_signal(&(cond)); \
        (void)pthread_mutex_unlock(&(mutex)); \
    } while (0)
#endif


#ifndef ot_cond_destroy
#define ot_cond_destroy(cond, mutex) do {          \
        (void)pthread_cond_destroy(&(cond)); \
        (void)pthread_mutex_destroy(&(mutex)); \
    } while (0)
#endif

/* * SCENE Error BaseId : [28bit~31bit] unique */
#define OT_SCENECOMM_ERR_BASEID (0x80000000L)

/* * SCENE Error Code Rule [ --base[4bit]--|--module[8bit]--|--error[20bit]--]
 * module : module enum value [OT_APP_MOD_E]
 * event_code : event code in specified module, unique in module
 */
#define OT_SCENECOMM_ERR_ID(module, err) ((ot_s32)((OT_SCENECOMM_ERR_BASEID) | ((module) << 20) | (err)))

/* * App Module ID */
typedef enum {
    OT_SCENE_MOD_SCENE = 0,
    OT_SCENE_MOD_CONFACCESS,
    OT_SCENE_MOD_BUTT
} ot_scene_mod;

#ifndef scene_array_size
#define scene_array_size(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* End of #ifndef OT_APPCOMM_H */
