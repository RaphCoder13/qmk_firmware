# This file intentionally left blank
# MCU name
BOARD = GENERIC_RP_RP2040
SPLIT_KEYBOARD = yes
SERIAL_DRIVER = vendor




# Build Options
#   change yes to no to disable
#
UNICODE_ENABLE    = yes # Unicode
BOOTMAGIC_ENABLE  = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE   = yes  # Mouse keys
EXTRAKEY_ENABLE   = yes # Audio control and System control
CONSOLE_ENABLE    = yes # Console for debug
COMMAND_ENABLE    = no  # Commands for debug and configuration
NKRO_ENABLE       = no           # Enable N-Key Rollover
BACKLIGHT_ENABLE  = no # Enable keyboard backlight functionality
SWAP_HANDS_ENABLE = yes  # Disable Onehand
AUTO_SHIFT_ENABLE = yes

RGBLIGHT_ENABLE   = yes
RGBLIGHT_DRIVER   = ws2812
WS2812_DRIVER     = vendor

DYNAMIC_MACRO_ENABLE = yes

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = cirque_pinnacle_spi


REPEAT_KEY_ENABLE = yes

# Enable Caps Word
# Disabled for v3
# CAPS_WORD_ENABLE = yes
# # Enable Key Override
# KEY_OVERRIDE_ENABLE = yes
# Enable Tap Dance

USER_NAME := raphcoder13

# VPATH += keyboards/gboards
# VPATH += users/raphcoder13

# SRC += features/layer_lock.c
# # following for v3
# SRC += features/custom_shift_keys.c
# SRC += features/casemodes.c
# SRC += features/select_word.c
ACHORDION_ENABLE ?= yes
# ifeq ($(ACHORDION_ENABLE),yes)
# 	OPT_DEFS += -DACHORDION_ENABLE
# 	SRC += features/achordion.c
# endif

