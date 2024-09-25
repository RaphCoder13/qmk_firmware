#pragma once
#include <sendstring_french.h>

#include <features/custom_shift_keys.h>

#include QMK_KEYBOARD_H



// Left-hand home row mods
#define HOME_A_GUI LGUI_T(FR_A)
#define HOME_R_ALT LALT_T(KC_R)
#define HOME_S_SFT LSFT_T(KC_S)
#define HOME_T_CTL LCTL_T(KC_T)
#define HOME_D_RALT RALT_T(FR_D)

// Right-hand home row mods
#define HOME_N_CTL RCTL_T(KC_N)
#define HOME_E_SFT RSFT_T(KC_E)
#define HOME_I_ALT LALT_T(KC_I)
#define HOME_U_GUI RGUI_T(FR_U)
#define HOME_H_RALT RALT_T(FR_H)

// #define HOME_Y RALT_T(FR_Y)
// #define HOME_S_SFTCLN RALT_T(FR_SCLN)


// #define HOME_O RGUI_T(KC_O)

// // Modifiers pous OPTIMOT
// #define HOME_OP_Y RALT_T(FR_Y)
// #define HOME_OP_A LGUI_T(FR_A)
// #define HOME_OP_I LALT_T(FR_I)
// #define HOME_OP_E LSFT_T(FR_E)
// #define HOME_OP_U LCTL_T(FR_U)

// #define HOME_OP_H RALT_T(FR_H)
// #define HOME_OP_N RGUI_T(FR_N)
// #define HOME_OP_R LALT_T(FR_R)
// #define HOME_OP_S RSFT_T(FR_S)
// #define HOME_OP_T RCTL_T(FR_T)

// // Modifiers pous Colemak-FR
// #define DOT_HR_RALT RALT_T(FR_DOT)
// #define COMM_HR_RALT RALT_T(FR_COMM)
// #define C_HR_RALT RALT_T(FR_C)

// // Home row for the ErgoL
// #define X_HR_RALT RALT_T(FR_X)
// #define A_HR_GUI LGUI_T(FR_A)
// #define S_HR_LALT LALT_T(FR_S)
// #define E_HR_SFT LSFT_T(FR_E)
// #define N_HR_CTL LCTL_T(FR_N)
// #define Y_HR_RALT RALT_T(FR_Y)
// #define I_HR_LALT LALT_T(FR_I)
// #define T_HR_SFT RSFT_T(FR_T)
// #define R_HR_CTL RCTL_T(FR_R)

// // Idem pour AZERTY
// // Left-hand home row mods
// #define FR_GUI_A LGUI_T(KC_A)
// #define FR_GUI_Q LGUI_T(FR_Q)
// #define FR_ALT_S LALT_T(KC_S)
// #define FR_SFT_D LSFT_T(KC_D)
// #define FR_RALT_X RALT_T(KC_X)
// #define FR_RALT_W RALT_T(KC_W)
// #define FR_RALT_Z RALT_T(FR_Z)

// #define FR_CTL_F LCTL_T(KC_F)

// // Right-hand home row mods
// #define FR_CTL_J RCTL_T(KC_J)
// #define FR_SFT_K RSFT_T(KC_K)
// #define FR_ALT_L LALT_T(KC_L)
// #define FR_RALT_O RALT_T(KC_O)
// #define FR_RALT_COLN RALT_T(FR_COLN)

// #define FR_GUI_M RGUI_T(FR_M)

// Clipboard operations
#define USR_CUT C(KC_X)
#define USR_COPY C(KC_C)
#define USR_PASTE C(KC_V)

// #define USR_CUT S(KC_DEL)
// #define USR_COPY C(KC_INS)
// #define USR_PASTE S(KC_INS)

#define USR_UNDO C(FR_Z)
#define USR_REDO C(FR_Y)

// Caractères spéciaux non présent dans keymap_french.h
#define FR_UCRC ALGR(FR_U) // û
#define FR_ECRC ALGR(FR_R) // ê
#define FR_OCRC ALGR(FR_P) // ô
#define FR_ICRC ALGR(FR_I) // î
#define FR_ACRC ALGR(FR_Z) // â
#define FR_ITREM ALGR(FR_K) // ï
#define FR_OE ALGR(FR_O)    // œ
#define FR_ETREM ALGR(FR_D) // ë
#define FR_DAQ ALGR(FR_W) // «
#define FR_AE ALGR(FR_A) // æ

const custom_shift_key_t custom_shift_keys[] = {
    {FR_AMPR, FR_PIPE, true},           // Shift & is |
    {FR_QUOT, FR_DQUO, false},          // Shift ' is "
    {FR_LPRN, FR_RPRN, false},          // Shift ( is )
    {FR_RPRN, FR_DQUO, false},          // Shift ) is "
    {FR_BSLS, FR_DEG, true},            // Shift \ is °
    {FR_LABK, ALGR(KC_G), false},       // Shift < is ’
    {FR_RABK, FR_GRV, false},           // Shift > is `
    {FR_ASTR, FR_SLSH, true},           // Shift * is /
    {FR_AT, FR_MICR, true},             // Shift @ is μ
    {FR_EXLM, FR_SUP2, true},           // Shift ! is ²
    {ALGR(FR_CCED), S(FR_HASH), false}, // Shift ^ is ˘
    {FR_HASH, FR_TILD, false},          // Shift # is ~
    {FR_DAQ, ALGR(FR_X), false},        // Shift « is »
    {FR_UGRV, SAGR(FR_UGRV), false},    // Shift ù is Ù
    {FR_AGRV, SAGR(FR_AGRV), false},    // Shift à is À
    {FR_EGRV, SAGR(FR_EGRV), false},    // Shift è is È
    {FR_EACU, SAGR(FR_EACU), false},    // Shift é is É
    {FR_CCED, SAGR(FR_CCED), false},    // Shift ç is Ç
    {FR_PERC, FR_MINS, false},
    {FR_EXLM, FR_UNDS, false},
    {S(FR_SCLN), FR_COLN, false},       // Shift . is :
    // {S(FR_SCLN), FR_SCLN, true}      // Shift . is ;
    {FR_COMM, FR_SCLN, false},          // Shift , is ;
    // {FR_QUOT, S(FR_COMM), true},     // Shift ' is ?
    {S(FR_SLSH), FR_BSLS, true},        // Shift / is anti-slash

    // {S(FR_SLSH), S(FR_COMM), true},  // Shift / is ?
    {FR_MINS, S(FR_COMM), true},        // Shift - is ?
    // {FR_MINS, FR_UNDS, true},        // Shift - is _
    {FR_QUES, FR_EXLM, true},           // Shift - is _

    {FR_UNDS, FR_SLSH, true},           // Shift _ is slash
    // {FR_UNDS, FR_MINS, false},       // Shift _ is -

};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);


#define ________COLEMAK_FR_1DK_L1________        FR_Q,            FR_Z,             FR_F,           FR_P,           FR_B
#define ________COLEMAK_FR_1DK_L2________        HOME_A_GUI,      HOME_R_ALT,       HOME_S_SFT,     HOME_T_CTL,     FR_G
#define ________COLEMAK_FR_1DK_L3________        FR_W,            FR_X,             FR_C,           HOME_D_RALT,    FR_V

#define ________COLEMAK_FR_1DK_R1________        FR_J,            FR_L,            FR_O,           OSL(ACCENT_COLEMAK_LAYER),     FR_Y
#define ________COLEMAK_FR_1DK_R2________        FR_M,            HOME_N_CTL,      HOME_E_SFT,     HOME_I_ALT,      HOME_U_GUI
#define ________COLEMAK_FR_1DK_R3________        FR_K,            HOME_H_RALT,     FR_MINS,        FR_COMM,         FR_DOT

#define ___________ACCENT_NEW_COLEMAK_L1___________          FR_ACRC,            FR_ITREM,         FR_ICRC,      FR_UCRC,    FR_QUOT
#define ___________ACCENT_NEW_COLEMAK_L2___________          FR_AGRV,            ETREM_MAJ,        FR_ECRC,      FR_DQUO,    FR_AMPR
#define ___________ACCENT_NEW_COLEMAK_L3___________          FR_AE,              FR_EURO,          FR_CCED,      FR_CIRC,    FR_BSLS

#define ___________ACCENT_NEW_COLEMAK_R1___________          FR_OE,            KC_NO,            FR_OCRC,        S(FR_CIRC),    SAGR(FR_EQL)
#define ___________ACCENT_NEW_COLEMAK_R2___________          ALGR(KC_G),       FR_EACU,          FR_EGRV,        FR_MICR,       FR_UGRV
#define ___________ACCENT_NEW_COLEMAK_R3___________          ALGR(FR_W),       ALGR(FR_X),       SAGR(FR_MINS),  SAGR(FR_COMM), FR_EXLM



////////////////////////////////
#define ____________QWERTY_L1____________        KC_Q,                 KC_W,               KC_E,           KC_R,            KC_T
#define ____________QWERTY_L2____________        KC_A,                 KC_S,               KC_D,           KC_F,            KC_G
#define ____________QWERTY_L3____________        KC_Z,                 KC_X,               KC_C,           KC_V,            KC_B

#define ____________QWERTY_R1____________        KC_Y,                 KC_U,               KC_I,           KC_O,            KC_P
#define ____________QWERTY_R2____________        KC_H,                 KC_J,               KC_K,           KC_L,            KC_SLSH
#define ____________QWERTY_R3____________        KC_N,                 KC_M,               KC_COMM,        KC_SCLN,         KC_COLN


#define ____________SYMBOL_L1____________        FR_AT,                FR_PERC,            FR_LABK,        FR_RABK,         FR_QUOT
#define ____________SYMBOL_L2____________        FR_HASH,              FR_EXLM,            FR_LPRN,        FR_RPRN,         FR_EQL
#define ____________SYMBOL_L3____________        ALGR(FR_CCED),        FR_AMPR,            FR_ASTR,        FR_BSLS,         FR_DLR

#define ____________SYMBOL_R1____________        FR_DQUO,              FR_LBRC,            FR_RBRC,        KC_NO,         KC_NO
#define ____________SYMBOL_R2____________        FR_GRV,               OSM(MOD_RCTL),      TD(TD_OSM_RSFT),  OSM(MOD_LALT),   OSM(MOD_RGUI)
#define ____________SYMBOL_R3____________        FR_QUOT,              FR_LCBR,            FR_RCBR,        OSM(MOD_RALT),         KC_NO

#define ____________NUMBS_L1____________         KC_PSLS,              FR_7,               FR_8,           FR_9,            KC_PMNS
#define ____________NUMBS_L2____________         KC_PAST,              FR_4,               FR_5,           FR_6,            KC_PPLS
#define ____________NUMBS_L3____________         FR_0,                 FR_1,               FR_2,           FR_3,            KC_EQL
#define ____________NUMBS_R1____________         KC_BSPC,              DM_RSTP,            DM_PLY1,        DM_PLY2,         KC_NO
#define ____________NUMBS_R2____________         KC_DEL,               OSM(MOD_RCTL),      OSM(MOD_RSFT),  OSM(MOD_LALT),   OSM(MOD_RGUI)
#define ____________NUMBS_R3____________         KC_NO,                OSM(MOD_RALT),      DM_REC1,        DM_REC2,         KC_NO

#define ____________NAV_L1____________           C(FR_A),              USR_CUT,            USR_COPY,       USR_PASTE,       VIM_SYST_CLIPB
#define ____________NAV_L2____________           OSM(MOD_LGUI),        OSM(MOD_LALT),      OSM(MOD_LSFT),  OSM(MOD_LCTL), USR_UNDO
#define ____________NAV_L3____________           VIM_LAST_YANK,        LALT(KC_F4),        KC_APP,         OSM(MOD_RALT),   USR_REDO
// #define ____________NAV_L3____________           VIM_LAST_YANK,        LALT(KC_F4),        LGUI(KC_LALT),  KC_APP,          C(FR_Y)
#define ____________NAV_R1____________           KC_INS,               KC_HOME,            KC_UP,          KC_END,          KC_PGUP
#define ____________NAV_R2____________           KC_PSCR,              KC_LEFT,            KC_DOWN,        KC_RGHT,         KC_PGDN
#define ____________NAV_R3____________           KC_PAUS,              C(KC_LEFT),         C(KC_UP),       C(KC_RGHT),      KC_APP

#define ____________FUN_L1____________           KC_F12,               KC_F7,              KC_F8,          KC_F9,           KC_SCRL
#define ____________FUN_L2____________           KC_F11,               KC_F4,              KC_F5,          KC_F6,           KC_PSCR
#define ____________FUN_L3____________           KC_F10,               KC_F1,              KC_F2,          KC_F3,           KC_PAUS
#define ____________FUN_R1____________           KC_INS,               KC_NO,              KC_MUTE,        KC_VOLD,         KC_VOLU
#define ____________FUN_R2____________           KC_NUM,               KC_RCTL,            KC_RSFT,        KC_LALT,         KC_RGUI
#define ____________FUN_R3____________           KC_NO,                KC_RALT,            KC_MPRV,        KC_MPLY,         KC_MNXT

#define ____________APPS_L1___________           VRSN,                KC_NO,              KC_NO,          KC_NO,           TD(TD_TRIPLE_BOOTLOADER)
#define ____________APPS_L2___________           KC_LGUI,              KC_LALT,            KC_LSFT,        KC_LCTL,           KC_NO
#define ____________APPS_L3___________           KC_NO,                KC_NO,              KC_NO,          KC_RALT,           KC_NO
#define ____________APPS_R1___________           TD(TD_TRIPLE_REBOOT), LALT(FR_L),         LCTL(KC_GRV),   KC_NO,           KC_NO
#define ____________APPS_R2___________           KC_APP,               KC_WHOM,            KC_NO,        MEH(KC_T),       KC_NO
#define ____________APPS_R3___________           MEH(KC_K),            LCA(KC_F),          KC_CALC,        SGUI(KC_S),      KC_NO


#define ___________MOUSE_L1___________           KC_NO,                USR_CUT,            USR_COPY,       USR_PASTE,       KC_NO
#define ___________MOUSE_L2___________           OSM(MOD_LGUI),        OSM(MOD_LALT),      OSM(MOD_LSFT),  OSM(MOD_LCTL),   KC_ESC
#define ___________MOUSE_L3___________           KC_NO,                KC_RALT,            KC_NO,          KC_MS_BTN4,      KC_MS_BTN5
#define ___________MOUSE_R1___________           KC_MS_WH_UP,          KC_MS_LEFT,         KC_MS_DOWN,       KC_MS_UP,      KC_MS_RIGHT
#define ___________MOUSE_R2___________           KC_MS_WH_DOWN,        OSM(MOD_RCTL),      OSM(MOD_RSFT),  OSM(MOD_LALT),   OSM(MOD_RGUI)
#define ___________MOUSE_R3___________           KC_NO,                KC_MS_WH_LEFT,      KC_MS_WH_RIGHT, KC_RALT,         KC_NO


#define ___________EXCEL_L1___________           KC_LEFT,              KC_DOWN,            KC_UP,          KC_RIGHT,        KC_F2
#define ___________EXCEL_L2___________           OSM(MOD_LGUI),        OSM(MOD_LALT),      TD(TD_OSM_LSFT),  OSM(MOD_LCTL),   S(KC_G)
#define ___________EXCEL_L3___________           KC_NO,                USR_CUT,            USR_COPY,       OSM(MOD_RALT),   USR_PASTE
#define ___________EXCEL_R1___________           KC_PSLS,              KC_P7,              KC_P8,          KC_P9,           KC_PMNS
#define ___________EXCEL_R2___________           KC_PAST,              KC_P4,              KC_P5,          KC_P6,           KC_PPLS
#define ___________EXCEL_R3___________           KC_P0,                KC_P1,              KC_P2,          KC_P3,           KC_PDOT
