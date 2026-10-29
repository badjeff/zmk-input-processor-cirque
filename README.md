# Input Processor for Cirque GlidePoint (Pinnacle) touchpad

Input processor for the Cirque GlidePoint (Pinnacle) touchpad: converts absolute
touch coordinates to relative cursor output, with an edge-scroll ring emitting
wheel output, primary/secondary tap detection, inertial glide, and haptic
feedback pulses driven on gpio pins (e.g. the [drv2605 gpio](https://github.com/badjeff/zmk-drv2605-driver) wrapper).

## Features

- Compatible with Zephyr upstream `cirque,pinnacle` driver.
- Absolute-to-relative cursor output from the touchpad report
- Edge-scroll ring: the pad rim is split into angular segments, each segment crossing emits one wheel tick (`REL_WHEEL`)
- Hold-on-edge gesture qualification: rim touches must hold within a drift threshold before wheel output starts (`edge-scroll-gesture-delay-ms` / `edge-scroll-gesture-delay-drift`)
- Primary / secondary tap detection with configurable duration and drift thresholds (`tap-max-ms` / `tap-max-drift`); secondary tap via the edge-scroll ring as mouse right click
- Inertial glide: flick then coast on release, with per-tick velocity decay and a minimum release velocity (`inertial-decay-pct` / `inertial-start-velocity`)
- Optional per-event haptic feedback pulses on GPIO pins, i2c-backed pins like the [drv2605 gpio](https://github.com/badjeff/zmk-drv2605-driver) shim supported; pulses are played off the input thread, one pin per pulse source (scroll-start / scroll-trigger / glide)
- Glide feedback throttling (`feedback-glide-interval-ms`)
- Rate limited report sync for cursor (`sync-rel-xy-ms`) and scroll wheel (`sync-rel-v-ms`) deltas
- Finger-lift filter: pads that keep reporting as contact strength drains are pre-processed, and lift-tail frames whose Z fell below the session Z average are swallowed before movement or gesture detection (`lift-filter-pct`)

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
feedback pulses wired to [drv2605 gpio](https://github.com/badjeff/zmk-drv2605-driver) pin shims:

```
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
};

#include <input/processors/cirque.dtsi>

&zip_cirque {
  feedback-edge-scroll-start-gpios = <&gpio_drv2605_0 0 9>; // waveform: 9
  feedback-edge-scroll-start-ms = <5>;

  feedback-edge-scroll-trigger-gpios = <&gpio_drv2605_1 0 9>; // waveform: 9
  feedback-edge-scroll-trigger-ms = <2>;

  /* Enable haptic feedback on touching/gliding
   * (Steam Deck / Steam Controller favor)
   */
  feedback-glide-gpios = <&gpio_drv2605_2 0 9>; // waveform: 9
  feedback-glide-ms = <2>;
  feedback-glide-interval-ms = <25>;

  /* Finetune scrolling edge margin for feedback enabled */
  edge-scroll-margin-h-pct = <29>; // default: 25
  edge-scroll-margin-v-pct = <22>; // default: 19
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
| `edge-scroll-margin-h-pct` | int | Horizontal margin of the center (cursor) region, percent of the pad half-width | 25 |
| `edge-scroll-margin-v-pct` | int | Vertical margin of the center (cursor) region, percent of the pad half-height | 19 |
| `edge-scroll-segment` | int | Number of angular segments the edge-scroll ring is split into; crossing a boundary emits one wheel tick; < 2 falls back to a simple swipe up/down heuristic | 64 |
| `edge-scroll-gesture-delay-ms` | int | Hold-on-edge delay before an edge touch qualifies as a scroll gesture, ms; 0 = no hold, scroll starts immediately | 175 |
| `edge-scroll-gesture-delay-drift` | int | Max movement per axis during the hold-on-edge window, counts | 20 |
| `session-end-idle-ms` | int | Idle time after the last touch frame before a gesture session ends: runs tap detection and, for cursor sessions, starts the inertial glide | 20 |
| `sync-rel-xy-ms` | int | Rate limit for emitting accumulated REL_X/REL_Y cursor deltas, ms | 1 |
| `sync-rel-v-ms` | int | Rate limit for emitting accumulated REL_WHEEL scroll ticks, ms | 8 |
| `lift-filter-pct` | int | Detects finger-lift trailing frames: lift-tail samples whose Z fell this percentage below the session Z average (recalculated on the running sample history) are swallowed before movement or gesture detection; rising Z ramps always pass; 0 = disabled | 45 |

| `inertial-disable` | boolean | Disable inertial cursor motion (flick then coast) | n (glide enabled) |
| `inertial-decay-pct` | int | Glide velocity decay per tick, percent of velocity passed to the next tick | 90 |
| `inertial-start-velocity` | int | Minimum release velocity to start coasting, counts/s; 0 = coast on every non-tap release | 500 |
| `feedback-edge-scroll-start-gpios` | phandle-array | Haptic feedback GPIO pin for the start of edge-scroll; pin driven high while the pulse plays | (none) |
| `feedback-edge-scroll-start-ms` | int | Duration of the scroll-start haptic pulse, ms | 5 |
| `feedback-edge-scroll-trigger-gpios` | phandle-array | Haptic feedback GPIO pin for edge-scroll wheel output | (none) |
| `feedback-edge-scroll-trigger-ms` | int | Duration of the segment-crossing haptic pulse, ms | 2 |
| `feedback-glide-gpios` | phandle-array | Haptic feedback GPIO pin for cursor output | (none) |
| `feedback-glide-ms` | int | Duration of the cursor haptic pulse, ms | 2 |
| `feedback-glide-interval-ms` | int | Min interval between two cursor haptic feedback pulses, ms; 0 = no throttling | 25 |

## License

MIT License
