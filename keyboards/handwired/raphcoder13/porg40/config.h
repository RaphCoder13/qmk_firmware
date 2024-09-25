/*
 * Copyright (c) 2024 Raphael (@RaphCoder13)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
// Copyright 2023 Raphael (@RaphCoder13)
// SPDX-License-Identifier: GPL-2.0-or-later


// #define MATRIX_ROWS 8
// #define MATRIX_COLS 6

// #define MATRIX_ROW_PINS { GP26, GP27, GP28,  GP29 }
// #define MATRIX_COL_PINS { GP2, GP3, GP4, GP5, GP6, GP7 }
// #define MATRIX_COL_PINS_RIGHT { GP2, GP3, GP4, GP5, GP6}

/* Handedness. */
#define MASTER_RIGHT

//#define USE_I2C

#define SERIAL_PIO_USE_PIO1

#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_PIN_SWAP
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1
//#define SOFT_SERIAL_PIN GP1


#define MOUSEKEY_INTERVAL       16
#define MOUSEKEY_DELAY          20
#define MOUSEKEY_TIME_TO_MAX    30
#define MOUSEKEY_MAX_SPEED      40
#define MOUSEKEY_WHEEL_DELAY    20
#define MOUSEKEY_MOVE_DELTA    1
#define MOUSEKEY_INERTIA

#define TAPPING_TOGGLE  2

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

#define TAPPING_TERM    220
// #define IGNORE_MOD_TAP_INTERRUPT // this makes it possible to do rolling combos (zx) with keys that convert to other keys on hold (z becomes ctrl when you hold it, and when this option isn't enabled, z rapidly followed by x actually sends Ctrl-x. That's bad.)
#define PERMISSIVE_HOLD
#define QUICK_TAP_TERM 120


/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#define LAYER_LOCK_IDLE_TIMEOUT 60000

// #define IGNORE_MOD_TAP_INTERRUPT
// #define TAPPING_FORCE_HOLD

/* key combination for command */
// rem Disabled MASK_SHIFT for Caps Word
#ifndef IS_COMMAND
#define IS_COMMAND() (get_mods() == MOD_MASK_CTRL)
/* #define IS_COMMAND() ( \
    get_mods() == MOD_MASK_CTRL || \
    get_mods() == MOD_MASK_SHIFT   \
) */
#endif

// Use both Shift for Caps Word
// #define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
// Use double tab shift for Caps Word
#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#define CAPS_WORD_IDLE_TIMEOUT 5000  // 5 seconds.

#define ACHORDION_STREAK

#define COMBO_ONLY_FROM_LAYER 0
#define COMBO_TERM 100
#define DYNAMIC_MACRO_DELAY 50
#define AUTO_SHIFT_TIMEOUT 200
#define NO_AUTO_SHIFT_SPECIAL
#define NO_AUTO_SHIFT_SYMBOLS
#define NO_AUTO_SHIFT_ALPHA
#define NO_AUTO_SHIFT_NUM
#define NO_AUTO_SHIFT_TAB
#define NO_AUTO_SHIFT_ENTER
#define AUTO_SHIFT_REPEAT
#define CAPS_WORD_INVERT_ON_SHIFT
#define LEADER_NO_TIMEOUT
#define CAPSWORD_USE_CAPS
#define ONESHOT_TAP_TOGGLE 4
#define ONESHOT_TIMEOUT 2000  //2 seconds.

/* fix space cadet rollover issue */
#define DISABLE_SPACE_CADET_ROLLOVER

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    5

#define USB_MAX_POWER_CONSUMPTION 500

#define SPLIT_WATCHDOG_ENABLE


/* Gestion Trackpad */
#define SPI_DRIVER SPID1
#define POINTING_DEVICE_RIGHT
#define SPLIT_POINTING_ENABLE

#define SPI_MISO_PIN GP8
#define POINTING_DEVICE_CS_PIN GP9
#define SPI_SCK_PIN GP10
#define SPI_MOSI_PIN GP11


#undef CIRQUE_PINNACLE_DIAMETER_MM
#define CIRQUE_PINNACLE_DIAMETER_MM 40
#define CIRQUE_PINNACLE_CURVED_OVERLAY
#define CIRQUE_PINNACLE_POSITION_MODE   CIRQUE_PINNACLE_ABSOLUTE_MODE
#define CIRQUE_PINNACLE_TAP_ENABLE
// #define CIRQUE_PINNACLE_SECONDARY_TAP_ENABLE
#define POINTING_DEVICE_GESTURES_SCROLL_ENABLE
#define POINTING_DEVICE_GESTURES_CURSOR_GLIDE_ENABLE
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define POINTING_DEVICE_MODES_ENABLE

#define POINTING_DRAG_DIVISOR 16
// #define POINTING_DEVICE_INVERT_Y
// #define POINTING_DEVICE_ROTATION_270_RIGHT
#define POINTING_DEVICE_ROTATION_270

#define AUTO_MOUSE_TIME 400

#ifdef RGB_MATRIX_ENABLE
#define RGB_MATRIX_SPLIT { 25, 25 }
#define SPLIT_LAYER_STATE_ENABLE
#define WS2812_BYTE_ORDER  WS2812_BYTE_ORDER_GRB
#endif
#define SPLIT_TRANSPORT_MIRROR

#undef RGBLIGHT_ENABLE
// Gestion des leds
#ifdef RGBLIGHT_ENABLE

// RP2040-Matrix clone is GRB instead of RGB like Waveshare RP2040-Matrix.
#define WS2812_BYTE_ORDER  WS2812_BYTE_ORDER_GRB

#define RGBLIGHT_SLEEP
#define RGBLIGHT_TIMEOUT 60100
#define SPLIT_TRANSPORT_MIRROR

// #define RGBLIGHT_SPLIT
// #define RGBLIGHT_EFFECT_BREATHING
// #define RGBLIGHT_EFFECT_BREATHE_CENTER 2.0
// #define RGBLIGHT_EFFECT_BREATHE_MAX 64
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    define RGBLIGHT_EFFECT_SNAKE
#    define RGBLIGHT_EFFECT_KNIGHT
#    define RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    define RGBLIGHT_EFFECT_RGB_TEST
#    define RGBLIGHT_EFFECT_ALTERNATING
#    define RGBLIGHT_EFFECT_TWINKLE
#define RGBLIGHT_LIMIT_VAL 64
#define RGBLIGHT_EFFECT_BREATHE_MAX 64
#define RGBLIGHT_LED_COUNT 50
// #define RGBLIGHT_LAYERS_RETAIN_VAL
#define RGBLIGHT_LAYERS
#define RGBLIGHT_MAX_LAYERS 16
#undef RGBLED_SPLIT
#define RGBLED_SPLIT { 25, 25}
#define WS2812_DI_PIN GP16

#define WS2812_PIO_USE_PIO1
#define SPLIT_LAYER_STATE_ENABLE
#endif
