/*
 * Copyright (c) 2026 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

/*
 * Sensor attribute bridge for the Cirque input processor.
 *
 * The zmk,behavior-sensor-attr-cycle behavior calls sensor_attr_set() on a
 * sensor device. The Cirque input processor exposes the input processor API
 * instead, so this small device forwards sensor attributes to the processor's
 * runtime attributes.
 */

#define DT_DRV_COMPAT zmk_input_processor_cirque_setter

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include <zmk/cirque.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct zip_cirque_setter_config {
    const struct device *processor;
};

static int zip_cirque_setter_attr_set(const struct device *dev, enum sensor_channel chan,
                                        enum sensor_attribute attr,
                                        const struct sensor_value *val) {
    const struct zip_cirque_setter_config *config = dev->config;

    if (chan != SENSOR_CHAN_ALL) {
        return -ENOTSUP;
    }

    if (val == NULL) {
        return -EINVAL;
    }

    if (!device_is_ready(config->processor)) {
        return -EBUSY;
    }

    return zip_cirque_set_attr(config->processor, (uint32_t)attr, val->val1);
}

static int zip_cirque_setter_init(const struct device *dev) {
    const struct zip_cirque_setter_config *config = dev->config;

    if (!device_is_ready(config->processor)) {
        LOG_WRN("Cirque input processor not ready at bridge init");
    }

    return 0;
}

static const struct sensor_driver_api zip_cirque_setter_api = {
    .attr_set = zip_cirque_setter_attr_set,
};

#define ZIP_CIRQUE_SETTER_INST(n)                                                     \
    static const struct zip_cirque_setter_config config_##n = {                       \
        .processor = DEVICE_DT_GET(DT_INST_PHANDLE(n, processor)),                      \
    };                                                                                  \
    DEVICE_DT_INST_DEFINE(n, zip_cirque_setter_init, NULL, NULL, &config_##n,         \
                          POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,             \
                          &zip_cirque_setter_api);

DT_INST_FOREACH_STATUS_OKAY(ZIP_CIRQUE_SETTER_INST)

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
