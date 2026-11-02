#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "sample_comm.h"

#define GPIO_GRP_MAX                15
#define GPIO_GRP_PIN_MAX            8
#define GPIO_FILE_NAME_LEN          64
#define GPIO_BUF_LEN                32

#define GPIO_GET_PIN_NUM(grp, pin)          ((8 * grp) + pin)
#define GPIO_BASE_PATH                      "/sys/class/gpio"
#define GPIO_EXPORT_PATH                    "/sys/class/gpio/export"

typedef enum {
    GPIO_TYPE_INIT,
    GPIO_TYPE_EXPORT,
    GPIO_TYPE_DIRECTION,
    GPIO_TYPE_VALUE,
    GPIO_TYPE_INVALID,
} gpio_type;

static ot_s32 gpio_get_file_name(gpio_type type, ot_u32 pin_num, ot_s8 *file_name, ot_u32 len)
{
    memset_s(file_name, len, 0, len);
    switch (type) {
        case GPIO_TYPE_INIT:
            snprintf(file_name, len, "%s/gpio%u", GPIO_BASE_PATH, pin_num);
            break;
        case GPIO_TYPE_EXPORT:
            snprintf(file_name, len, "%s", GPIO_EXPORT_PATH);
            break;
        case GPIO_TYPE_DIRECTION:
            snprintf(file_name, len, "%s/gpio%u/direction", GPIO_BASE_PATH, pin_num);
            break;
        case GPIO_TYPE_VALUE:
            snprintf(file_name, len, "%s/gpio%u/value", GPIO_BASE_PATH, pin_num);
            break;
        default:
            printf("Invalid gpio type\n");
            return OT_FAILURE;
    }
    return OT_SUCCESS;
}

static ot_s32 gpio_write_value(gpio_type type, const ot_s8 *file_name, ot_u32 value)
{
    int fd = -1;
    char buf[GPIO_BUF_LEN] = {0};

    fd = open(file_name, O_WRONLY);
    if (fd == -1) {
        printf("open %s failed\n", file_name);
        return OT_FAILURE;
    }

    if (type == GPIO_TYPE_DIRECTION) {
        if (value == 0) {
            snprintf(buf, GPIO_BUF_LEN, "in");
        } else {
            snprintf(buf, GPIO_BUF_LEN, "out");
        }
    } else {
        snprintf(buf, GPIO_BUF_LEN, "%u", value);
    }
    if (write(fd, buf, strlen(buf)) != strlen(buf)) {
        printf("write %u failed\n", value);
        close(fd);
        return OT_FAILURE;
    }

    close(fd);
    return OT_SUCCESS;
}


static ot_s32 gpio_set_type_value(gpio_type type, ot_u32 pin_num, ot_u32 value)
{
    ot_s32 ret = OT_SUCCESS;
    ot_s8 file_name[GPIO_FILE_NAME_LEN];

    ret = gpio_get_file_name(type, pin_num, file_name, GPIO_FILE_NAME_LEN);
    if (ret != OT_SUCCESS) {
        printf("gpio get path %s failed\n", file_name);
        return ret;
    }

    ret = gpio_write_value(type, file_name, value);
    if (ret != OT_SUCCESS) {
        printf("%s write failed\n", file_name);
        return ret;
    }
    return ret;
}

static ot_s32 gpio_export(ot_u32 pin_num)
{
    ot_s32 ret = OT_SUCCESS;
    ot_s8 file_name[GPIO_FILE_NAME_LEN];

    ret = gpio_get_file_name(GPIO_TYPE_INIT, pin_num, file_name, GPIO_FILE_NAME_LEN);
    if (ret != OT_SUCCESS) {
        printf("gpio get path %s failed\n", file_name);
        return OT_FAILURE;
    }

    if (access(file_name, F_OK) != 0) {
        gpio_set_type_value(GPIO_TYPE_EXPORT, pin_num, pin_num);
    }
    gpio_set_type_value(GPIO_TYPE_DIRECTION, pin_num, OT_TRUE);
    return OT_SUCCESS;
}

ot_s32 sample_comm_gpio_output_ctrl(ot_u8 gpio_grp, ot_u8 gpio_pin, ot_bool pull_high)
{
    ot_s32 ret = OT_SUCCESS;
    ot_u32 pin_num;

    if (gpio_grp >= GPIO_GRP_MAX) {
        sample_print("Invalid gpio group.\n");
        return OT_FAILURE;
    }

    if (gpio_pin > GPIO_GRP_PIN_MAX) {
        sample_print("Invalid gpio group pin.\n");
        return OT_FAILURE;
    }

    pin_num = GPIO_GET_PIN_NUM(gpio_grp, gpio_pin);
    ret = gpio_export(pin_num);
    if (ret != OT_SUCCESS) {
        sample_print("gpio init failed.\n");
        return ret;
    }
    gpio_set_type_value(GPIO_TYPE_VALUE, pin_num, pull_high);

    return ret;
}