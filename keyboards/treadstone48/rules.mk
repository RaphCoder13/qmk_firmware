# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = caterina

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite
EXTRAKEY_ENABLE = no       # Audio control and System control
CONSOLE_ENABLE = no        # Console for debug
COMMAND_ENABLE = no        # Commands for debug and configuration
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = no            # USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
AUDIO_ENABLE = no           # Audio output
SPLIT_KEYBOARD = yes

MOUSEKEY_ENABLE = yes       # Mouse keys

RGBLIGHT_ENABLE = yes        # Enable keyboard RGB underglow
OLED_ENABLE = yes
OLED_DRIVER = SSD1306

DEFAULT_FOLDER = treadstone48/rev1
