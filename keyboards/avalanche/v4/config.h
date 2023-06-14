// Copyright 2022 Will Winder (@winder)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#ifdef RGBLIGHT_ENABLE
#    define RGBLED_NUM 64
#    define RGBLIGHT_SPLIT
#    define RGBLIGHT_HUE_STEP  10    
#    define RGBLIGHT_SAT_STEP  17    
#    define RGBLIGHT_VAL_STEP  17    

#    define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_RAINBOW_SWIRL+2

#    define RGBLIGHT_EFFECT_ALTERNATING
#    define RGBLIGHT_EFFECT_RGB_TEST
#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_EFFECT_KNIGHT
#    define RGBLIGHT_EFFECT_SNAKE
#    define RGBLIGHT_EFFECT_CHRISTMAS
#endif

#ifdef OLED_ENABLE
#    define OLED_DISPLAY_128X64
#    define OLED_TIMEOUT 30000
#endif
