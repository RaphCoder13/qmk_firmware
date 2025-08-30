// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
	{{0,4},{1,4},{2,4},{3,4},{4,4}},
	{{0,5},{1,5},{2,5},{3,5},{4,5}},
	{{0,6},{1,6},{2,6},{3,6},{4,6}},
	{{0,7},{1,7},{2,7},{3,7},{4,7}},
	{{0,0},{1,0},{2,0},{3,0},{4,0}},
	{{0,1},{1,1},{2,1},{3,1},{4,1}},
	{{0,2},{1,2},{2,2},{3,2},{4,2}},
	{{0,3},{1,3},{2,3},{3,3},{4,3}}
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │Tab│ Q │ W │ E │ R │ T │       │ Y │ U │ I │ O │ P │Bsp│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Ctl│ A │ S │ D │ F │ G │       │ H │ J │ K │ L │ ; │ ' │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Sft│ Z │ X │ C │ V │ B │       │ N │ M │ , │ . │ / │Sft│
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      *               ┌───┐                   ┌───┐
      *               │GUI├───┐           ┌───┤Alt│
      *               └───┤Bsp├───┐   ┌───┤Ent├───┘
      *                   └───┤   │   │   ├───┘
      *                       └───┘   └───┘
      */
    [0] = LAYOUT_wrapper(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                 KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                 KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_RSFT,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                 KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RCTL,
                                KC_LGUI, KC_BSPC, KC_SPC,               KC_SPC,  KC_ENT,  KC_RALT
    )
};
