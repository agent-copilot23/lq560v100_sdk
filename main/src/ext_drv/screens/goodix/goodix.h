#ifndef GOODIX_H
#define GOODIX_H

#include <linux/kernel.h>

#define GOODIX_MAX_HEIGHT		4096
#define GOODIX_MAX_WIDTH		4096
#define GOODIX_GT911_HEIGHT		1280
#define GOODIX_GT911_WIDTH		800
#define GOODIX_GA6571_HEIGHT 	720
#define GOODIX_GA6571_WIDTH 	1280

#define GOODIX_ID_MAX_LEN	            4
#define GOODIX_INT_TRIGGER		        1
#define GOODIX_CONTACT_SIZE		        8
#define GOODIX_MAX_CONTACT_SIZE		    9
#define GOODIX_MAX_CONTACTS		        10
#define GOODIX_MAX_KEYS			        7
#define GOODIX_GT911_MAX_TOUCH_NUMBER   5
#define GOODIX_GA6571_MAX_TOUCH_NUMBER  10

#define GOODIX_CONFIG_MIN_LENGTH	186
#define GOODIX_CONFIG_911_LENGTH	186
#define GOODIX_CONFIG_967_LENGTH	228
#define GOODIX_CONFIG_GT9X_LENGTH	240
#define GOODIX_CONFIG_GA6571_LENGTH	240
#define GOODIX_CONFIG_MAX_LENGTH	240

/* Register defines */
#define GOODIX_REG_COMMAND		    0x8040
#define GOODIX_CMD_SCREEN_OFF		0x05

#define GOODIX_READ_COOR_ADDR		    0x814E
#define GOODIX_GT1X_REG_CONFIG_DATA	    0x8050
#define GOODIX_GT9X_REG_CONFIG_DATA	    0x8047
#define GOODIX_GA6571_REG_CONFIG_DATA	0x8047
#define GOODIX_REG_ID			        0x8140

#define GOODIX_BUFFER_STATUS_READY	BIT(7)
#define GOODIX_HAVE_KEY			    BIT(4)
#define GOODIX_BUFFER_STATUS_TIMEOUT	20

#define RESOLUTION_LOC			1
#define RESOLUTION_GA6571_LOC	37	// 0x8047 + 37 = 0x806C
#define MAX_CONTACTS_LOC		5
#define MAX_CONTACTS_GA6571_LOC	1	// 0x8047 + 1 = 0x8048
#define TRIGGER_LOC				6


struct touchscreen_properties {
	unsigned int max_x;
	unsigned int max_y;
	bool invert_x;
	bool invert_y;
	bool swap_x_y;
};

enum goodix_irq_pin_access_method {
	IRQ_PIN_ACCESS_NONE,
	IRQ_PIN_ACCESS_GPIO,
	IRQ_PIN_ACCESS_ACPI_GPIO,
	IRQ_PIN_ACCESS_ACPI_METHOD,
};


struct goodix_chip_id {
	const char *id;
	const struct goodix_chip_data *data;
};

struct goodix_ts_data {
	struct i2c_client *client;
	struct input_dev *input_dev;
	const struct goodix_chip_data *chip;
	struct touchscreen_properties prop;
	unsigned int max_touch_num;
	unsigned int int_trigger_type;
	struct regulator *avdd28;
	struct regulator *vddio;
	struct gpio_desc *gpiod_int;
	struct gpio_desc *gpiod_rst;
	int gpio_count;
	int gpio_int_idx;
	char id[GOODIX_ID_MAX_LEN + 1];
	u16 version;
	const char *cfg_name;
	bool reset_controller_at_probe;
	bool load_cfg_from_disk;
	struct completion firmware_loading_complete;
	unsigned long irq_flags;
	enum goodix_irq_pin_access_method irq_pin_access_method;
	unsigned int contact_size;
	u8 config[GOODIX_CONFIG_MAX_LENGTH];
	unsigned short keymap[GOODIX_MAX_KEYS];
};

const unsigned long goodix_irq_flags[] = {
	IRQ_TYPE_EDGE_RISING,
	IRQ_TYPE_EDGE_FALLING,
	IRQ_TYPE_LEVEL_LOW,
	IRQ_TYPE_LEVEL_HIGH,
};


void touchscreen_parse_properties(struct input_dev *input, bool multitouch,
				  struct touchscreen_properties *prop);

void touchscreen_report_pos(struct input_dev *input,
			    const struct touchscreen_properties *prop,
			    unsigned int x, unsigned int y,
			    bool multitouch);



#endif