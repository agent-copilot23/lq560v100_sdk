#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "sample_comm.h"

#define PWM_MAX_NUM         PWM_ID_BUTT
#define PWM_FILE_NAME_LEN   64
#define PWM_BUF_LEN         32
#define PWM_BASE_PATH       "/sys/class/pwm/pwmchip0"
#define PWM_PERIOD_50KHZ    20000U
#define PWM_PERIOD_25KHZ    40000U
#define PWM_PERIOD_20KHZ    50000U
#define PWM_PERIOD          PWM_PERIOD_25KHZ
#define PWM_MAX_DUTY_CYCLE  10000
#define PWM_PER_STAGE       (PWM_PERIOD / PWM_MAX_DUTY_CYCLE)

typedef enum {
    PWM_TYPE_INIT,
    PWM_TYPE_EXPORT,
    PWM_TYPE_PERIOD,
    PWM_TYPE_DUTY_CYCLE,
    PWM_TYPE_ENABLE,
    PWM_TYPE_BUTT,
} pwm_type;

typedef struct {
    ot_bool init;
    ot_bool enable;
} pwm_attr;
static pwm_attr g_pwm_attr[PWM_MAX_NUM];

static ot_s32 pwm_get_file_name(pwm_type type, ot_u8 pwm_id, ot_s8 *file_name, ot_u32 len)
{
    memset_s(file_name, len, 0, len);
    switch (type) {
        case PWM_TYPE_INIT:
            snprintf(file_name, len, "%s/pwm%d", PWM_BASE_PATH, pwm_id);
            break;
        case PWM_TYPE_EXPORT:
            snprintf(file_name, len, "%s/export", PWM_BASE_PATH);
            break;
        case PWM_TYPE_PERIOD:
            snprintf(file_name, len, "%s/pwm%d/period", PWM_BASE_PATH, pwm_id);
            break;
        case PWM_TYPE_DUTY_CYCLE:
            snprintf(file_name, len, "%s/pwm%d/duty_cycle", PWM_BASE_PATH, pwm_id);
            break;
        case PWM_TYPE_ENABLE:
            snprintf(file_name, len, "%s/pwm%d/enable", PWM_BASE_PATH, pwm_id);
            break;
        default:
            printf("Invalid pwm type\n");
            return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 pwm_write_value(const ot_s8 *file_name, ot_u32 value)
{
    int fd = -1;
    ot_s8 buf[PWM_BUF_LEN] = {0};

    fd = open(file_name, O_WRONLY);
    if (fd == -1) {
        printf("open %s failed\n", file_name);
        return OT_FAILURE;
    }

    snprintf(buf, PWM_BUF_LEN, "%u", value);
    if (write(fd, buf, strlen(buf)) != strlen(buf)) {
        printf("write %u failed\n", value);
        close(fd);
        return OT_FAILURE;
    }

    close(fd);
    return OT_SUCCESS;
}

static ot_s32 pwm_set_type_value(pwm_type type, ot_u8 pwm_id, ot_u32 value)
{
    ot_s32 ret;
    ot_s8 file_name[PWM_FILE_NAME_LEN];

    ret = pwm_get_file_name(type, pwm_id, file_name, PWM_FILE_NAME_LEN);
    if (ret != OT_SUCCESS) {
        printf("pwm get path %s failed\n", file_name);
        return ret;
    }

    ret = pwm_write_value(file_name, value);
    if (ret != OT_SUCCESS) {
        printf("%s write failed\n", file_name);
        return ret;
    }
    return ret;
}

static ot_s32 pwm_init(ot_u8 pwm_id)
{
    ot_s32 ret;
    ot_s8 file_name[PWM_FILE_NAME_LEN];

    ret = pwm_get_file_name(PWM_TYPE_INIT, pwm_id, file_name, PWM_FILE_NAME_LEN);
    if (ret != OT_SUCCESS) {
        printf("pwm get path %s failed\n", file_name);
        return OT_FAILURE;
    }

    if (access(file_name, F_OK) != 0) {
        pwm_set_type_value(PWM_TYPE_EXPORT, pwm_id, pwm_id);
        pwm_set_type_value(PWM_TYPE_PERIOD, pwm_id, PWM_PERIOD);
    }
    return OT_SUCCESS;
}

// duty_cycle, 占空比, 取值 0 - 100
ot_s32 sample_comm_start_pwm(sample_pwm_id pwm_id, ot_u32 duty_cycle)
{
    ot_s32 ret;

    if (pwm_id >= PWM_MAX_NUM) {
        sample_print("Invalid pwm id.\n");
        return OT_FAILURE;
    }

    if (duty_cycle > PWM_MAX_DUTY_CYCLE) {
        sample_print("Invalid duty cycle.\n");
        return OT_FAILURE;
    }

    if (g_pwm_attr[pwm_id].init != OT_TRUE) {
        ret = pwm_init(pwm_id);
        if (ret != OT_SUCCESS) {
            sample_print("pwm %hhu init failed\n", pwm_id);
            return OT_FAILURE;
        }
        g_pwm_attr[pwm_id].init = OT_TRUE;
    }

    if (g_pwm_attr[pwm_id].enable != OT_TRUE) {
        ret = pwm_set_type_value(PWM_TYPE_ENABLE, pwm_id, OT_TRUE);
        if (ret != OT_SUCCESS) {
            sample_print("pwm %hhu enable failed\n", pwm_id);
            return OT_FAILURE;
        }
        g_pwm_attr[pwm_id].enable = OT_TRUE;
    }

    ret = pwm_set_type_value(PWM_TYPE_DUTY_CYCLE, pwm_id, duty_cycle * PWM_PER_STAGE);
    if (ret != OT_SUCCESS) {
        sample_print("pwm %hhu enable failed\n", pwm_id);
    }
    return ret;
}

ot_s32 sample_comm_stop_pwm(sample_pwm_id pwm_id)
{
    if (pwm_id >= PWM_MAX_NUM) {
        sample_print("Invalid pwm id.\n");
        return OT_FAILURE;
    }

    if (g_pwm_attr[pwm_id].enable == OT_TRUE) {
        if (pwm_set_type_value(PWM_TYPE_ENABLE, pwm_id, OT_FALSE) != OT_SUCCESS) {
            sample_print("pwm %hhu enable failed\n", pwm_id);
            return OT_FAILURE;
        }
        g_pwm_attr[pwm_id].enable = OT_FALSE;
    }
    return OT_SUCCESS;
}
