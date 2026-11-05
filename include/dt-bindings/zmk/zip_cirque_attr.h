/*
 * Copyright (c) 2026 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

/*
 * Runtime attributes for the Cirque input processor.
 *
 * These values are passed through zmk,behavior-sensor-attr-cycle to the
 * zmk,input-processor-cirque-setter bridge, which forwards them to the
 * input processor. Values are clamped in zip_cirque_set_attr() to the
 * ranges shown in tailed comments.
 */
#define ZIP_CIRQUE_ATTR_BASE 0x1000

/* Short-hand: ZCA(TAP_MAX_MS) expands to ZIP_CIRQUE_ATTR_TAP_MAX_MS */
#define ZCA(n) ZIP_CIRQUE_ATTR_##n

/* Tap and tap-drag */
#define ZIP_CIRQUE_ATTR_PRIMARY_TAP_ENABLE (ZIP_CIRQUE_ATTR_BASE + 0)
#define ZIP_CIRQUE_ATTR_SECONDARY_TAP_ENABLE (ZIP_CIRQUE_ATTR_BASE + 1)
#define ZIP_CIRQUE_ATTR_TAP_MAX_MS (ZIP_CIRQUE_ATTR_BASE + 2)    /* [1, 1000] */
#define ZIP_CIRQUE_ATTR_TAP_MAX_DRIFT (ZIP_CIRQUE_ATTR_BASE + 3) /* [0, 50] */
#define ZIP_CIRQUE_ATTR_TAP_DRAG_ENABLE (ZIP_CIRQUE_ATTR_BASE + 4)
#define ZIP_CIRQUE_ATTR_TAP_DRAG_WINDOW_MS (ZIP_CIRQUE_ATTR_BASE + 5)     /* [1, 1000] */
#define ZIP_CIRQUE_ATTR_TAP_DRAG_DRIFT (ZIP_CIRQUE_ATTR_BASE + 6)         /* [0, 50] */
#define ZIP_CIRQUE_ATTR_TAP_DRAG_HOLD_START_MS (ZIP_CIRQUE_ATTR_BASE + 7) /* [0, 1000] */

/* Edge scroll */
#define ZIP_CIRQUE_ATTR_EDGE_SCROLL_MARGIN_H_PCT (ZIP_CIRQUE_ATTR_BASE + 8)         /* [0, 100] */
#define ZIP_CIRQUE_ATTR_EDGE_SCROLL_MARGIN_V_PCT (ZIP_CIRQUE_ATTR_BASE + 9)         /* [0, 100] */
#define ZIP_CIRQUE_ATTR_EDGE_SCROLL_SEGMENT (ZIP_CIRQUE_ATTR_BASE + 10)             /* [0, 360] */
#define ZIP_CIRQUE_ATTR_EDGE_SCROLL_GESTURE_DELAY_MS (ZIP_CIRQUE_ATTR_BASE + 11)    /* [0, 1000] */
#define ZIP_CIRQUE_ATTR_EDGE_SCROLL_GESTURE_DELAY_DRIFT (ZIP_CIRQUE_ATTR_BASE + 12) /* [0, 50] */

/* Session and filtering */
#define ZIP_CIRQUE_ATTR_SESSION_END_IDLE_MS (ZIP_CIRQUE_ATTR_BASE + 13) /* [1, 1000] */
#define ZIP_CIRQUE_ATTR_SYNC_REL_XY_MS (ZIP_CIRQUE_ATTR_BASE + 14)      /* [0, 1000] */
#define ZIP_CIRQUE_ATTR_SYNC_REL_V_MS (ZIP_CIRQUE_ATTR_BASE + 15)       /* [0, 1000] */
#define ZIP_CIRQUE_ATTR_LIFT_FILTER_PCT (ZIP_CIRQUE_ATTR_BASE + 16)     /* [0, 100] */

/* Inertial glide */
#define ZIP_CIRQUE_ATTR_INERTIAL_ENABLE (ZIP_CIRQUE_ATTR_BASE + 17)
#define ZIP_CIRQUE_ATTR_INERTIAL_TICK_MS (ZIP_CIRQUE_ATTR_BASE + 18)        /* [0, 1000] */
#define ZIP_CIRQUE_ATTR_INERTIAL_TICK_MIN_MS (ZIP_CIRQUE_ATTR_BASE + 19)    /* [1, 1000] */
#define ZIP_CIRQUE_ATTR_INERTIAL_TICK_MAX_MS (ZIP_CIRQUE_ATTR_BASE + 20)    /* [1, 1000] */
#define ZIP_CIRQUE_ATTR_INERTIAL_DECAY_PCT (ZIP_CIRQUE_ATTR_BASE + 21)      /* [0, 100] */
#define ZIP_CIRQUE_ATTR_INERTIAL_GLIDE_MAX_MS (ZIP_CIRQUE_ATTR_BASE + 22)   /* [1, 10000] */
#define ZIP_CIRQUE_ATTR_INERTIAL_VELOCITY_CAP (ZIP_CIRQUE_ATTR_BASE + 23)   /* [0, 1000000] */
#define ZIP_CIRQUE_ATTR_INERTIAL_START_VELOCITY (ZIP_CIRQUE_ATTR_BASE + 24) /* [0, 4096] */

/* Haptic feedback channels */
#define ZIP_CIRQUE_ATTR_FEEDBACK_TAP_DRAG_ENABLE (ZIP_CIRQUE_ATTR_BASE + 25)
#define ZIP_CIRQUE_ATTR_FEEDBACK_EDGE_SCROLL_START_ENABLE (ZIP_CIRQUE_ATTR_BASE + 26)
#define ZIP_CIRQUE_ATTR_FEEDBACK_EDGE_SCROLL_TRIGGER_ENABLE (ZIP_CIRQUE_ATTR_BASE + 27)
#define ZIP_CIRQUE_ATTR_FEEDBACK_GLIDE_ENABLE (ZIP_CIRQUE_ATTR_BASE + 28)
#define ZIP_CIRQUE_ATTR_FEEDBACK_TAP_DRAG_MS (ZIP_CIRQUE_ATTR_BASE + 29)          /* [1, 1000] */
#define ZIP_CIRQUE_ATTR_FEEDBACK_EDGE_SCROLL_START_MS (ZIP_CIRQUE_ATTR_BASE + 30) /* [1, 1000] */
#define ZIP_CIRQUE_ATTR_FEEDBACK_EDGE_SCROLL_TRIGGER_MS                                            \
    (ZIP_CIRQUE_ATTR_BASE + 31)                                                /* [1, 1000]        \
                                                                                */
#define ZIP_CIRQUE_ATTR_FEEDBACK_GLIDE_MS (ZIP_CIRQUE_ATTR_BASE + 32)          /* [1, 1000] */
#define ZIP_CIRQUE_ATTR_FEEDBACK_GLIDE_INTERVAL_MS (ZIP_CIRQUE_ATTR_BASE + 33) /* [0, 1000] */
