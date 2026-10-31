/*
 * Copyright (c) 2026 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Apply a runtime attribute to a Cirque input processor.
 *
 * Attribute IDs are defined in <dt-bindings/zmk/zip_cirque_attr.h>. Values are
 * clamped to the supported range. Out-of-range values are adjusted rather
 * than rejected.
 *
 * @param dev   The Cirque input processor device.
 * @param attr_id One of the ZIP_CIRQUE_ATTR_* identifiers.
 * @param value New value for the attribute.
 *
 * @retval 0        Attribute applied.
 * @retval -EINVAL  Unknown attribute.
 * @retval -ENOTSUP The device is not a Cirque input processor.
 */
int zip_cirque_set_attr(const struct device *dev, uint32_t attr_id, int32_t value);

#ifdef __cplusplus
}
#endif
