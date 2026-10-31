# Copyright (c) 2026 The ZMK Contributors
# SPDX-License-Identifier: MIT

target_sources_ifdef(CONFIG_ZMK_INPUT_PROCESSOR_CIRQUE app PRIVATE input_processor_cirque.c)
target_sources_ifdef(CONFIG_ZMK_INPUT_PROCESSOR_CIRQUE_SETTER app PRIVATE input_processor_cirque_setter.c)
