// Copyright 2023 RaphCoder13 (@RaphCoder13)
// SPDX-License-Identifier: GPL-2.0-or-later

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

#define USB_SUSPEND_WAKEUP_DELAY 200
#define MATRIX_ROWS 10
#define MATRIX_COLS 6

#define MATRIX_ROW_PINS { GP27, GP26, GP22, GP21, GP20 }
#define MATRIX_COL_PINS { GP11, GP10, GP9, GP8, GP7, GP6}

/* Handedness. */
#define MASTER_RIGHT
// #define EE_HANDS
// #define USB_POLLING_INTERVAL_MS 10
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

#define TAPPING_TERM    250
#define PERMISSIVE_HOLD
#define QUICK_TAP_TERM 120


/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
// #define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
// #define LOCKING_RESYNC_ENABLE

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

#define COMBO_ONLY_FROM_LAYER 0
#define COMBO_TERM 100

#define DYNAMIC_MACRO_DELAY 40

// Autoshift pour la ponctuation
#define AUTO_SHIFT_TIMEOUT 250
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

// #define KEY_OVERRIDE_INCLUDE_WEAK_MODS
/* fix space cadet rollover issue */
#define DISABLE_SPACE_CADET_ROLLOVER
/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    5

#define USB_MAX_POWER_CONSUMPTION 500
#define SPLIT_WATCHDOG_ENABLE

/* Gestion Trackpad */

#define POINTING_DEVICE_MODES_ENABLE

#define POINTING_DRAG_DIVISOR 16
// #define POINTING_DEVICE_INVERT_Y

#define SPI_DRIVER SPID1
#define POINTING_DEVICE_RIGHT
#define SPLIT_POINTING_ENABLE
#define SPI_MISO_PIN GP12
#define POINTING_DEVICE_CS_PIN GP13
#define SPI_SCK_PIN GP14
#define SPI_MOSI_PIN GP15
#undef CIRQUE_PINNACLE_DIAMETER_MM
#define CIRQUE_PINNACLE_DIAMETER_MM 40
#define CIRQUE_PINNACLE_CURVED_OVERLAY
#define CIRQUE_PINNACLE_POSITION_MODE   CIRQUE_PINNACLE_ABSOLUTE_MODE
#define CIRQUE_PINNACLE_TAP_ENABLE
// #define CIRQUE_PINNACLE_SECONDARY_TAP_ENABLE
#define POINTING_DEVICE_GESTURES_SCROLL_ENABLE
#define POINTING_DEVICE_GESTURES_CURSOR_GLIDE_ENABLE
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE

#define AUTO_MOUSE_TIME 400

/* #define AUTO_MOUSE_DEFAULT_LAYER    10 */

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
// #define NO_DEBUG

/* disable print */
// #define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

//#define DEBUG_MATRIX_SCAN_RATE

