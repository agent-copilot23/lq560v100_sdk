/*
 * cyttsp5_devtree.c
 * Cypress TrueTouch(TM) Standard Product V5 Device Tree Support Module.
 * For use with Cypress Txx5xx parts.
 * Supported parts include:
 * TMA5XX
 *
 * Copyright (C) 2013 Cypress Semiconductor
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, and only version 2, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contact Cypress Semiconductor at www.cypress.com <ttdrivers@cypress.com>
 *
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/of_device.h>
#include <linux/slab.h>

/* cyttsp */
#include "cyttsp5_regs.h"
#include "cyttsp5_platform.h"

#ifdef CONFIG_SEC_ATLANTIC_PROJECT
#include <linux/of_gpio.h>
#endif

#ifdef CONFIG_TOUCHSCREEN_CYPRESS_CYTTSP5_BUTTON
#define ENABLE_VIRTUAL_KEYS
#endif

#define MAX_NAME_LENGTH		64

enum cyttsp5_device_type {
	DEVICE_MT,
	DEVICE_BTN,
	DEVICE_PROXIMITY,
	DEVICE_TYPE_MAX,
};

struct cyttsp5_device_pdata_func {
	void *(*create_and_get_pdata)(struct device_node *);
	void (*free_pdata)(void *);
};

struct cyttsp5_pdata_ptr {
	void **pdata;
};

#ifdef ENABLE_VIRTUAL_KEYS

struct cyttsp5_virtual_keys {
	struct kobj_attribute kobj_attr;
	u16 *data;
	int size;
};
#endif

struct cyttsp5_extended_mt_platform_data {
	struct cyttsp5_mt_platform_data pdata;
#ifdef ENABLE_VIRTUAL_KEYS
	struct cyttsp5_virtual_keys vkeys;
#endif
};

static inline int get_inp_dev_name(struct device_node *dev_node,
		const char **inp_dev_name)
{
	return of_property_read_string(dev_node, "cy,inp_dev_name",
			inp_dev_name);
}

#ifdef CONFIG_SEC_ATLANTIC_PROJECT
 int avdd_gpio;
#endif

#ifdef CONFIG_TOUCHSCREEN_CYTTSP5_DEVICETREE_SUPPORT
int cyttsp5_devtree_create_and_get_pdata(struct device *adap_dev)
{
	struct cyttsp5_platform_data *pdata;
	struct device_node *core_node, *dev_node, *dev_node_fail;
	enum cyttsp5_device_type type;
	int count = 0;
	int rc = 0;

	if (!adap_dev->of_node)
		return 0;

	pdata = kzalloc(sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	adap_dev->platform_data = pdata;
	set_pdata_ptr(pdata);

	/* There should be only one core node */
	for_each_child_of_node(adap_dev->of_node, core_node) {
		const char *name;

		rc = of_property_read_string(core_node, "name", &name);
		if (!rc)
			pr_debug("%s: name:%s\n", __func__, name);

		pdata->core_pdata = create_and_get_core_pdata(core_node);
		if (IS_ERR(pdata->core_pdata)) {
			rc = PTR_ERR(pdata->core_pdata);
			break;
		}

		/* Increment reference count */
		of_node_get(core_node);

		for_each_child_of_node(core_node, dev_node) {
			count++;
			rc = get_device_type(dev_node, &type);
			if (rc)
				break;

			*pdata_ptr[type].pdata
				= create_and_get_device_pdata(dev_node, type);
			if (IS_ERR(*pdata_ptr[type].pdata))
				rc = PTR_ERR(*pdata_ptr[type].pdata);
			if (rc)
				break;

			/* Increment reference count */
			of_node_get(dev_node);
		}

		if (rc) {
			free_core_pdata(pdata->core_pdata);
			of_node_put(core_node);
			for_each_child_of_node(core_node, dev_node_fail) {
				if (dev_node == dev_node_fail)
					break;
				rc = get_device_type(dev_node, &type);
				if (rc)
					break;
				free_device_pdata(type);
				of_node_put(dev_node);
			}
			break;
		}

		pdata->loader_pdata = &_cyttsp5_loader_platform_data;
	}

	pr_debug("%s: %d child node(s) found\n", __func__, count);

	return rc;
}
EXPORT_SYMBOL_GPL(cyttsp5_devtree_create_and_get_pdata);

int cyttsp5_devtree_clean_pdata(struct device *adap_dev)
{
	struct cyttsp5_platform_data *pdata;
	struct device_node *core_node, *dev_node;
	enum cyttsp5_device_type type;
	int rc = 0;

	if (!adap_dev->of_node)
		return 0;

	pdata = dev_get_platdata(adap_dev);
	set_pdata_ptr(pdata);
	for_each_child_of_node(adap_dev->of_node, core_node) {
		free_core_pdata(pdata->core_pdata);
		of_node_put(core_node);
		for_each_child_of_node(core_node, dev_node) {
			rc = get_device_type(dev_node, &type);
			if (rc)
				break;
			free_device_pdata(type);
			of_node_put(dev_node);
		}
	}

	return rc;
}
EXPORT_SYMBOL_GPL(cyttsp5_devtree_clean_pdata);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Cypress TrueTouch(R) Standard Product DeviceTree Driver");
MODULE_AUTHOR("Cypress Semiconductor <ttdrivers@cypress.com>");
