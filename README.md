# Input Processor for Cirque GlidePoint (Pinnacle) Touchpad

This module is a special brain upgrade for a round mini touchpad on a custom keyboard. It takes your finger touches and turns them into decent mouse.

## What it does

It processes the input reports from Zephyr's `cirque,pinnacle` touchpad driver. Converts absolute
touch coordinates to relative cursor output, with an edge-scroll ring emitting
wheel output, primary/secondary tap detection, inertial glide, and haptic
feedback pulses driven on gpio pins (e.g. the [drv2605 gpio](https://github.com/badjeff/zmk-drv2605-driver) wrapper).

### Feature Highlights

*   **Precise Pointer:** It detects finger-lift trailing frames, swallows phantom inputs caused by wireless peripheral without wired grounding.
*   **Circular Scroll:** The very outer edge of the circle acts like a scrolling rim. If you trace your finger around the rim, it scrolls like a mouse wheel. Once scroll gesture mode on, you can spin your finger all over the pad, adjust the scroll speed by circling closer or further to the center.
*   **Tap to Click:** If you tap quickly with your finger, it clicks the left mouse button. If you tap on the outer edge, it performs a right-click.
*   **Tap-Tap and Drag:** If you tap quickly and then put your finger right back down to hold it, it "grabs" whatever is on the screen so you can highlight text.
*   **Inertial Glide:** If you flick your finger and let go, the mouse cursor doesn't stop instantly, it smoothly glides and coasts to a stop, just like cursor sliding on the Steam Deck or using the Steam Controller.
*   **Haptic Feedback:** It can trigger tiny vibration or buzzer. Every time you scroll, drag, or glide, it can give your finger a tiny, satisfying *bzzzt* feeling so you know it worked. This is optinal, but it is a MUST HAVE.

> [!IMPORTANT]
> Cirque's pinnacle touchpad does runtime calibration after bootup. It will present few expected funky choastic jumps on first few contact. Just re-touch your finger on the pad few times to let to finish its calibration.

## Installation

Include this module on your ZMK's west manifest in `config/west.yml`:

```yaml
manifest:
  remotes:
    #...
    # START #####
    - name: badjeff
      url-base: https://github.com/badjeff
    # END #######
    #...
  projects:
    #...
    # START #####
    - name: zmk-input-processor-cirque
      remote: badjeff
      revision: main
    # optional haptic feedback driver
    - name: zmk-drv2605-driver
      remote: badjeff
      revision: main
    # END #######
    #...
```

## Configuration

Sample configuration of a Cirque GlidePoint touchpad on `spi2` with haptic
feedback pulses wired to [drv2605 gpio](https://github.com/badjeff/zmk-drv2605-driver) pin shims.

> [!NOTE]
> You'd prompt AI for summarising what is this module does. Require it to ignore the readme, review the source code, and explains the features to you like you're 5.

```dts
&pinctrl {
  spi2_default: spi2_default {
    group1 {
      psels = <NRF_PSEL(SPIM_SCK, 1, 13)>,
              <NRF_PSEL(SPIM_MOSI, 1, 15)>,
              <NRF_PSEL(SPIM_MISO, 1, 14)>;
    };
  };
  spi2_sleep: spi2_sleep {
    group1 {
      psels = <NRF_PSEL(SPIM_SCK, 1, 13)>,
              <NRF_PSEL(SPIM_MOSI, 1, 15)>,
              <NRF_PSEL(SPIM_MISO, 1, 14)>;
      low-power-enable;
    };
  };
  i2c1_default: i2c1_default {
    group1 {
      psels = <NRF_PSEL(TWIM_SDA, 0, 4)>,
              <NRF_PSEL(TWIM_SCL, 0, 5)>;
    };
  };

  i2c1_sleep: i2c1_sleep {
    group1 {
      psels = <NRF_PSEL(TWIM_SDA, 0, 4)>,
              <NRF_PSEL(TWIM_SCL, 0, 5)>;
      low-power-enable;
    };
  };
};

// release gpio1 11 & 12 from xiao_ble-pinctrl.dtsi
// for &trackball { irq-gpios }
&uart0 {
  status = "disabled";
};

&spi2 {
  compatible = "nordic,nrf-spim";
  status = "disabled"; // <<< disabled in main .dtsi, okay in .overlay
  tpad0: glidepoint@0 {
    compatible = "cirque,pinnacle";
    status = "disabled";
    label = "TOUCHPAD0";
    reg = <0>;
    spi-max-frequency = <4000000>;
  };
};

// Config custom gpios
&spi2 {
  status = "okay";
  pinctrl-0 = <&spi2_default>;
  pinctrl-1 = <&spi2_sleep>;
  pinctrl-names = "default", "sleep";
  cs-gpios = <&gpio1 12 GPIO_ACTIVE_LOW>;
};

&tpad0 {
  status = "okay";
  data-ready-gpios = <&gpio1 11 GPIO_ACTIVE_HIGH>;
  sleep-mode-enable;
  sensitivity = "2x";
  data-mode = "absolute";
};

&i2c1 {
  status = "okay";
  compatible = "nordic,nrf-twim";
  pinctrl-0 = <&i2c1_default>;
  pinctrl-1 = <&i2c1_sleep>;
  pinctrl-names = "default", "sleep";

  drv2605_0: drv2605@5a {
    compatible = "ti,drv2605-alt";
    reg = <0x5a>;
    library = <6>; /* Library to use, 0 = Empty, 1-5 are ERM, 6 is LRA. */
    standby-ms = <1000>; /* switch to standby mode after idling for 1000ms */
  };
};

/{
  gpio_drv2605_0: gpio_drv2605_0 {
    compatible = "ti,drv2605-alt-gpio";
    status = "okay";
    gpio-controller;
    #gpio-cells = <2>;
    dev-reg = <0x5a>;
  };
  gpio_drv2605_1: gpio_drv2605_1 {
    compatible = "ti,drv2605-alt-gpio";
    status = "okay";
    gpio-controller;
    #gpio-cells = <2>;
    dev-reg = <0x5a>;
  };
  gpio_drv2605_2: gpio_drv2605_2 {
  	compatible = "ti,drv2605-alt-gpio";
  	status = "okay";
  	gpio-controller;
  	#gpio-cells = <2>;
  	dev-reg = <0x5a>;
  };
  gpio_drv2605_3: gpio_drv2605_3 {
  	compatible = "ti,drv2605-alt-gpio";
  	status = "okay";
  	gpio-controller;
  	#gpio-cells = <2>;
  	dev-reg = <0x5a>;
  };
};

#include <input/processors/cirque.dtsi>

&zip_cirque {
  feedback-tap-drag-gpios = <&gpio_drv2605_0 0 9>; // waveform: 9
  feedback-tap-drag-ms = <1>;

  feedback-edge-scroll-start-gpios = <&gpio_drv2605_1 0 8>; // waveform: 8
  feedback-edge-scroll-start-ms = <1>;

  feedback-edge-scroll-trigger-gpios = <&gpio_drv2605_2 0 9>; // waveform: 9
  feedback-edge-scroll-trigger-ms = <1>;

  /* Enable haptic feedback on touching/gliding
   * (Steam Deck / Steam Controller favor)
   */
  // feedback-glide-gpios = <&gpio_drv2605_3 0 9>; // waveform: 9
  // feedback-glide-ms = <1>;
  // feedback-glide-interval-ms = <25>;

  /* Finetune scrolling edge margin for feedback enabled */
  edge-scroll-margin-h-pct = <30>; // default: 25
  edge-scroll-margin-v-pct = <24>; // default: 19
};

/* use one input-listener to collect input from physical touchpad device
 * and use &zip_cirque to intercept all absolute input
 */
tpad0_abs: tpad0_abs {
  compatible = "zmk,input-listener";
  device = <&tpad0>;
  input-processors = <&zip_cirque>; // as an interceptor
};

/* use another input-listener to report intercepted input from &zip_cirque */
tpad0_rel {
  compatible = "zmk,input-listener";
  device = <&zip_cirque>; // as a virtual device
  default {
    input-processors = <&xy_scaler 4 7>;
    layers = <DEFAULT>;
  };
};
```

## Device Tree Properties

Node Properties of `zmk,input-processor-cirque`

| Property | Type | Description | Default |
|----------|------|-------------|---------|
| `abs-center-x` | int | Circle center of the touchpad coordinates, x-axis; X-axis value range [0..2047] (left to right) | 1024 |
| `abs-center-y` | int | Circle center of the touchpad coordinates, y-axis; Y-axis value range [0..1535] (top to bottom), Z-axis value range [0..63] (hover to touch down) | 768 |
| `primary-tap-disable` | boolean | Disable primary tap detection | n (tap enabled) |
| `secondary-tap-disable` | boolean | Disable secondary tap detection (tap on the edge-scroll ring as mouse right click, depends on primary tap) | n (tap enabled) |
| `tap-max-ms` | int | Max gesture duration for tap detection, ms; shorter gestures click instead of dragging or coasting | 120 |
| `tap-max-drift` | int | Max gesture movement per axis for tap detection, counts | 25 |
| `tap-drag-disable` | boolean | Disable the tap-drag gesture (a fast tap + swift re-touch that holds on the spot engages a drag: the mouse button presses once the hold window elapses; a quick lift completes the next click) | n (enabled) |
| `tap-drag-window-ms` | int | Max time between an armed tap and its re-touch, ms; retouches after this window are plain touches again | 200 |
| `tap-drag-drift` | int | Max per-axis offset between an armed tap position and its re-touch, counts; the re-touch must also stay within this offset for the hold window to switch into a drag | 70 |
| `tap-drag-hold-start-ms` | int | Hold window after the re-touch, ms; staying on the armed spot this long presses the button, switches into a drag and fires the haptic pulse; lifting earlier stays a plain click (should be recognised as double click from host) | 200 |
| `edge-scroll-margin-h-pct` | int | Horizontal margin of the center (cursor) region, percent of the pad half-width | 25 |
| `edge-scroll-margin-v-pct` | int | Vertical margin of the center (cursor) region, percent of the pad half-height | 19 |
| `edge-scroll-segment` | int | Number of angular segments the edge-scroll ring is split into; crossing a boundary emits one wheel tick; < 2 falls back to a simple swipe up/down heuristic | 64 |
| `edge-scroll-gesture-delay-ms` | int | Hold-on-edge delay before an edge touch qualifies as a scroll gesture, ms; 0 = no hold, scroll starts immediately | 175 |
| `edge-scroll-gesture-delay-drift` | int | Max movement per axis during the hold-on-edge window, counts | 20 |
| `session-end-idle-ms` | int | Idle time after the last touch frame before a gesture session ends: runs tap detection and, for cursor sessions, starts the inertial glide | 20 |
| `sync-rel-xy-ms` | int | Rate limit for emitting accumulated REL_X/REL_Y cursor deltas, ms | 1 |
| `sync-rel-v-ms` | int | Rate limit for emitting accumulated REL_WHEEL scroll ticks, ms | 1 |
| `history-len` | int | Number of recent touch samples kept in the history ring; both the inertial glide tick and the launch velocity are derived from this window | 6 |
| `lift-filter-pct` | int | Detects finger-lift trailing frames: lift-tail samples whose Z fell this percentage below the session Z average (recalculated on the running sample history) are swallowed before movement or gesture detection; rising Z ramps always pass; 0 = disabled | 45 |
| `inertial-disable` | boolean | Disable inertial cursor motion (flick then coast) | n (glide enabled) |
| `inertial-decay-pct` | int | Glide velocity decay per tick, percent of velocity passed to the next tick | 93 |
| `inertial-tick-ms` | int | Fixed glide tick override, ms; 0 = derive the tick from the sample history and clamp it with `inertial-tick-min-ms`/`inertial-tick-max-ms` | 0 |
| `inertial-tick-min-ms` | int | Lower clamp bound for the glide tick derived from the sample history, ms | 4 |
| `inertial-tick-max-ms` | int | Upper clamp bound for the glide tick derived from the sample history, ms | 40 |
| `inertial-glide-max-ms` | int | Fail-safe cap on a single inertial glide, ms, so a glide can never run forever | 2950 |
| `inertial-velocity-cap` | int | Sanity cap on the launch velocity of an inertial glide, counts/s | 16384 |
| `inertial-start-velocity` | int | Minimum release velocity to start coasting, counts/s; 0 = coast on every non-tap release | 500 |
| `feedback-tap-drag-gpios` | phandle-array | Haptic feedback GPIO pin for the button press when a tap-drag engages | (none) |
| `feedback-tap-drag-ms` | int | Duration of the tap-drag haptic feedback pulse, ms | 5 |
| `feedback-edge-scroll-start-gpios` | phandle-array | Haptic feedback GPIO pin for the start of edge-scroll; pin driven high while the pulse plays | (none) |
| `feedback-edge-scroll-start-ms` | int | Duration of the scroll-start haptic pulse, ms | 5 |
| `feedback-edge-scroll-trigger-gpios` | phandle-array | Haptic feedback GPIO pin for edge-scroll wheel output | (none) |
| `feedback-edge-scroll-trigger-ms` | int | Duration of the segment-crossing haptic pulse, ms | 2 |
| `feedback-glide-gpios` | phandle-array | Haptic feedback GPIO pin for cursor output | (none) |
| `feedback-glide-ms` | int | Duration of the cursor haptic pulse, ms | 2 |
| `feedback-glide-interval-ms` | int | Min interval between two cursor haptic feedback pulses, ms; 0 = no throttling | 25 |

## Runtime Attributes

Most settings can be adjusted at runtime through
[`zmk-behavior-sensor-attr-cycle`](https://github.com/badjeff/zmk-behavior-sensor-attr-cycle)
without rebuilding. Changes apply immediately and are not persisted: the DTS
defaults return at reboot.

The input processor is not a Zephyr sensor device, so the module provides a
small `zmk,input-processor-cirque-setter` bridge node that forwards sensor attributes to it.
The bridge is enabled by `CONFIG_ZMK_INPUT_PROCESSOR_CIRQUE_SETTER` and is only
built when a `zmk,behavior-sensor-attr-cycle` node exists.

```dts
#include <dt-bindings/zmk/zip_cirque_attr.h>
#include <input/processors/cirque.dtsi>

/ {
    behaviors {
        cirque_inertial: cirque_inertial {
            compatible = "zmk,behavior-sensor-attr-cycle";
            #binding-cells = <1>;
            sensor_device = <&zip_cirque_setter>;
            attr = <ZIP_CIRQUE_ATTR_INERTIAL_ENABLE>;
            values = <1 0>;
        };
        cirque_scroll_trigger_haptic: cirque_scroll_trigger_haptic {
            compatible = "zmk,behavior-sensor-attr-cycle";
            #binding-cells = <1>;
            sensor_device = <&zip_cirque_setter>;
            attr = <ZIP_CIRQUE_ATTR_FEEDBACK_EDGE_SCROLL_TRIGGER_ENABLE>;
            values = <1 0>;
        };
        cirque_inertial_decay: cirque_inertial_decay {
            compatible = "zmk,behavior-sensor-attr-cycle";
            #binding-cells = <1>;
            sensor_device = <&zip_cirque_setter>;
            attr = <ZIP_CIRQUE_ATTR_INERTIAL_DECAY_PCT>;
            values = <100 97 93 90 85 80>;
        };
    };
};
```

Keymap:

```dts
&cirque_inertial 1
&cirque_scroll_trigger_haptic 1
&cirque_inertial_decay 1
```

### Attributes

Attribute IDs live in `<dt-bindings/zmk/zip_cirque_attr.h>` with the
`ZIP_CIRQUE_ATTR_*` prefix.

| Group | Attributes |
|-------|------------|
| Tap / drag | `PRIMARY_TAP_ENABLE`, `SECONDARY_TAP_ENABLE`, `TAP_MAX_MS`, `TAP_MAX_DRIFT`, `TAP_DRAG_ENABLE`, `TAP_DRAG_WINDOW_MS`, `TAP_DRAG_DRIFT`, `TAP_DRAG_HOLD_START_MS` |
| Edge scroll | `EDGE_SCROLL_MARGIN_H_PCT`, `EDGE_SCROLL_MARGIN_V_PCT`, `EDGE_SCROLL_SEGMENT`, `EDGE_SCROLL_GESTURE_DELAY_MS`, `EDGE_SCROLL_GESTURE_DELAY_DRIFT` |
| Session | `SESSION_END_IDLE_MS`, `SYNC_REL_XY_MS`, `SYNC_REL_V_MS`, `LIFT_FILTER_PCT` |
| Inertial | `INERTIAL_ENABLE`, `INERTIAL_TICK_MS`, `INERTIAL_TICK_MIN_MS`, `INERTIAL_TICK_MAX_MS`, `INERTIAL_DECAY_PCT`, `INERTIAL_GLIDE_MAX_MS`, `INERTIAL_VELOCITY_CAP`, `INERTIAL_START_VELOCITY` |
| Haptics | `FEEDBACK_TAP_DRAG_ENABLE`, `FEEDBACK_EDGE_SCROLL_START_ENABLE`, `FEEDBACK_EDGE_SCROLL_TRIGGER_ENABLE`, `FEEDBACK_GLIDE_ENABLE`, `FEEDBACK_TAP_DRAG_MS`, `FEEDBACK_EDGE_SCROLL_START_MS`, `FEEDBACK_EDGE_SCROLL_TRIGGER_MS`, `FEEDBACK_GLIDE_MS`, `FEEDBACK_GLIDE_INTERVAL_MS` |

Notes:

*   Haptic channels default to enabled only when their feedback GPIO exists.
    Disabling a channel mutes any pulse currently playing.
*   Disabling `INERTIAL_ENABLE` stops an active glide immediately.
*   `INERTIAL_TICK_MS = 0` restores derived-tick behaviour at the next launch.
*   Values are clamped to a safe range; out-of-range values are adjusted rather
    than rejected.

## License

MIT License
