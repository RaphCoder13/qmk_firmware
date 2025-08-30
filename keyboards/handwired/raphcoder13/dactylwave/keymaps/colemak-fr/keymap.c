#include QMK_KEYBOARD_H


// #include "dactylwave.h"
#include <version.h>
#include <print.h>
#include <sendstring_french.h>
#include <features/layer_lock.h>
#include <features/select_word.h>
#ifdef ACHORDION_ENABLE
#include <features/achordion.h>
#endif
#include <features/custom_shift_keys.h>
#include <features/casemodes.h>
#include <action_code.h>

#include <enumlayers.h>
#include <g/keymap_combo.h>

#include <common_keymap_defs_colemak-fr.h>

#define LAYOUT_wrapper(...) LAYOUT_split_4x6_5(__VA_ARGS__)
const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    {{0,5},{1,5},{2,5},{3,5},{4,5},{5,5}},
    {{0,6},{1,6},{2,6},{3,6},{4,6},{5,6}},
    {{0,7},{1,7},{2,7},{3,7},{4,7},{5,7}},
    {{0,8},{1,8},{2,8},{3,8},{4,8},{5,8}},
    {{0,9},{1,9},{2,9},{3,9},{4,9},{5,9}},
    {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0}},
    {{0,1},{1,1},{2,1},{3,1},{4,1},{5,1}},
    {{0,2},{1,2},{2,2},{3,2},{4,2},{5,2}},
    {{0,3},{1,3},{2,3},{3,3},{4,3},{5,3}},
    {{0,4},{1,4},{2,4},{3,4},{4,4},{5,4}}

};
static uint32_t layer_timer;
static void refresh_layer_info(void);
static void check_layer_info_timeout(void);
bool is_layer_info_timeout = false;


// Enum for Tap Dance
enum {
    // TD_LSFT_LAYER0,
    // TD_RSFT_LAYER0,
    // TD_MO_LAYER_KEYP_NAV,
    // TD_MO_LAYER_EVRY_SYMB,
    // TD_MO_LAYER_CALC_AZERTY,
    // TD_LIBERK_LALT,
    // TD_KEYP_RALT,
    TD_TRIPLE_BOOTLOADER,
    TD_TRIPLE_REBOOT,
    // TD_MEDIA_BASE,
    TD_SWAP_HANDS,
    // TD_GAME_APPS,
    TD_OSM_LSFT,
    TD_OSM_RSFT,
    // TD_SPC_EXCEL,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_TRIPLE_TAP,
    TD_QUAD_TAP,
} td_state_t;
typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
// Pour Media Layer and Base
void mb_finished(tap_dance_state_t *state, void *user_data);
void mb_reset(tap_dance_state_t *state, void *user_data);
// Pour OSM Shift Tap Dance
void osmShift_finished(tap_dance_state_t *state, void *user_data);
void osmShift_reset(tap_dance_state_t *state, void *user_data);
// Pour Swap Hands Tap Dance
void swh_finished(tap_dance_state_t *state, void *user_data);
void swh_reset(tap_dance_state_t *state, void *user_data);
// Pour Game/Apps Tap Dance
void g_apps_finished(tap_dance_state_t *state, void *user_data);
void g_apps_reset(tap_dance_state_t *state, void *user_data);


// clang-format off


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


    [COLEMAK_FR_1DK] = LAYOUT_wrapper(
        KC_ESC,	KC_1,	KC_2,	KC_3,	DM_REC1,	DM_PLY1,                                          DM_REC2,	DM_PLY2, KC_8,	KC_9,	KC_0,	KC_DEL,
        MO(MOUSE_LAYER),	________COLEMAK_FR_1DK_L1________,                                        ________COLEMAK_FR_1DK_R1________,   KC_BSPC,
        TD(TD_OSM_LSFT),	________COLEMAK_FR_1DK_L2________,                                        ________COLEMAK_FR_1DK_R2________,   FR_UNDS,
        TD(TD_SWAP_HANDS),	________COLEMAK_FR_1DK_L3________,                                        ________COLEMAK_FR_1DK_R3________,	TD(TD_SWAP_HANDS),
         TT(GAME),	 RDB_REPEAT,	LT(APPS_LAYER,KC_ESC),	LT(EXCEL_LAYER,KC_SPC),	LT(NAV,KC_TAB),				LT( NUMBERS,KC_ENT),	LT(SYMB,KC_BSPC),	LT(FUN,KC_DEL),	RDB_ALT_REPEAT, LAYER_LOCK),

    [GAME] = LAYOUT_wrapper(
        KC_ESC,	KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_F5,																KC_F6,	KC_F7,	KC_F8,	KC_F9,	KC_F10,	KC_BSLS,
        KC_TAB, ____________QWERTY_L1____________	   ,	____________QWERTY_R1____________											,	KC_LBRC,
        KC_TRNS,____________QWERTY_L2____________	,				____________QWERTY_R2____________								,	KC_QUOT,
        KC_LCTL,____________QWERTY_L3____________	,	   ____________QWERTY_R3____________        ,	KC_RCTL,
        KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),
    [SYMB] = LAYOUT_wrapper(
        KC_ESC,	KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_F5,																KC_F6,	KC_F7,	KC_F8,	KC_F9,	KC_F10,	KC_BSLS,
        KC_TRNS,	____________SYMBOL_L1____________                ,	____________SYMBOL_R1____________                                       ,	KC_NO,
        KC_CAPS,	____________SYMBOL_L2____________                ,	____________SYMBOL_R2____________                						, KC_NO,
        KC_TRNS,	____________SYMBOL_L3____________                ,	____________SYMBOL_R3____________                	                    ,	KC_TRNS,
        KC_TRNS,	KC_TRNS,	TG(GAME),	QK_LEAD,	KC_TAB,								KC_ENT,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),
    [NUMBERS] = LAYOUT_wrapper(
        KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_PSLS,	KC_PAST,	                                                    KC_NO,	KC_NO,  KC_NO,	KC_NO,	KC_NO,	KC_NO,
        KC_NO,	____________NUMBS_L1____________                 ,	____________NUMBS_R1____________                            , KC_NO,
        FR_LPRN,____________NUMBS_L2____________                 ,	____________NUMBS_R2____________                            ,	KC_NO,
        KC_TRNS,____________NUMBS_L3____________                 ,	____________NUMBS_R3____________                            ,	KC_TRNS,
        KC_TRNS,	FR_DOT,	KC_ESC, KC_SPC,	KC_TAB,							KC_TRNS,	KC_BSPC,	KC_DEL,	KC_TRNS,	KC_TRNS),

    [NAV] = LAYOUT_wrapper(
        KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,				KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,
        KC_NO,	____________NAV_L1____________                   ,	____________NAV_R1____________                   	,	KC_NUM,
        KC_CAPS,____________NAV_L2____________                   ,	____________NAV_R2____________                   	,	KC_NO,
        KC_TRNS,____________NAV_L3____________                   , ____________NAV_R3____________                         ,	KC_TRNS,
        KC_TRNS,	KC_TRNS,	KC_TRNS,	RDB_REPEAT,	KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),
    [FUN] = LAYOUT_wrapper(
        KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,				KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,
        KC_NO,	____________FUN_L1____________                   ,	____________FUN_R1____________                   		,	KC_NO,
        KC_NO,	____________FUN_L2____________                   ,	____________FUN_R2____________                   		, KC_NO,
        KC_TRNS,____________FUN_L3____________                   ,	____________FUN_R3____________                           ,	KC_TRNS,
        KC_TRNS,	KC_TRNS,	KC_ESC,	KC_SPC,	KC_TAB,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),
     [APPS_LAYER] = LAYOUT_wrapper(
        KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	TD(TD_TRIPLE_BOOTLOADER),			KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,
        KC_NO,	____________APPS_L1___________                   ,	____________APPS_R1___________                   	,	KC_NO,
        KC_NO,	____________APPS_L2___________                   ,	____________APPS_R2___________                   	,	KC_NO,
        KC_TRNS,____________APPS_L3___________                   ,	____________APPS_R3___________                       ,	KC_TRNS,
        KC_TRNS,	KC_TRNS,	KC_ESC,	KC_SPC,	KC_TAB,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),
     [ACCENT_COLEMAK_LAYER] = LAYOUT_wrapper(
         KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,				KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,
        KC_NO,	___________ACCENT_NEW_COLEMAK_L1___________                  ,___________ACCENT_NEW_COLEMAK_R1___________                  ,	KC_NO,
        KC_TRNS,___________ACCENT_NEW_COLEMAK_L2___________                  ,___________ACCENT_NEW_COLEMAK_R2___________                  ,	KC_NO,
        KC_TRNS,___________ACCENT_NEW_COLEMAK_L3___________                  ,___________ACCENT_NEW_COLEMAK_R3___________                  ,	KC_TRNS,
        KC_TRNS,	KC_TRNS,	ALGR(KC_SPC),	ALGR(KC_G), KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_DEL,	KC_TRNS,	KC_TRNS),

    [MOUSE_LAYER] = LAYOUT_wrapper(
         KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,				KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,
        KC_NO,	___________MOUSE_L1___________           ,	___________MOUSE_R1___________          , KC_NO,
        KC_NO,	___________MOUSE_L2___________           ,	___________MOUSE_R2___________          ,	KC_NO,
        KC_TRNS,___________MOUSE_L3___________           ,	___________MOUSE_R3___________          ,	KC_TRNS,
        KC_NO,	KC_ESC,	KC_MS_BTN2,	KC_MS_BTN1,	KC_MS_BTN3,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),




    [EXCEL_LAYER] = LAYOUT_wrapper(
        KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,	KC_NO,			  KC_NO,	KC_NO,	KC_NO,	KC_PSLS,	KC_PAST,	KC_NO,
        KC_NO,	___________EXCEL_L1___________                   ,___________EXCEL_R1___________                   ,	Kr_NUM,
        KC_NO,	___________EXCEL_L2___________                   ,___________EXCEL_R2___________                   ,	KC_TRNS,
        KC_TRNS,___________EXCEL_L3___________                   ,___________EXCEL_R3___________                   ,	KC_TRNS,
        KC_TRNS,	KC_TRNS,	KC_ESC,	KC_TRNS,	KC_TAB,							KC_PENT,	KC_BSPC,	KC_EQL,	KC_TRNS,	KC_TRNS),

};
// clang-format on
uint8_t mod_state, weak_mod_state;

bool set_scrolling = false;

typedef struct
{
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

void pointing_device_init_user(void) {
    set_auto_mouse_layer(MOUSE_LAYER); // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of <mouse_layer>
    set_auto_mouse_enable(true);         // always required before the auto mouse feature will wor
}

void keyboard_post_init_user(void) {
  // Call the post init code.
    refresh_layer_info();
}

#define SCROLL_DIVISOR_H 64.0
#define SCROLL_DIVISOR_V 64.0

float scroll_accumulated_h = 0;
float scroll_accumulated_v = 0;
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (set_scrolling) {
        if (abs(mouse_report.x) > abs(mouse_report.y)) {
            mouse_report.h = (float)mouse_report.x / SCROLL_DIVISOR_H;
            mouse_report.v = 0;
            scroll_accumulated_h += (float)mouse_report.x / SCROLL_DIVISOR_H;
            mouse_report.h = (int8_t)scroll_accumulated_h;
            scroll_accumulated_h -= (int8_t)scroll_accumulated_h;
        } else {
            mouse_report.h = 0;
            mouse_report.v = (float)mouse_report.y / SCROLL_DIVISOR_V;
            scroll_accumulated_v += (float)mouse_report.y / SCROLL_DIVISOR_V;
            mouse_report.v = (int8_t)scroll_accumulated_v;
            scroll_accumulated_v -= (int8_t)scroll_accumulated_v;
        }
        // mouse_report.h = mouse_report.x;
        // mouse_report.v = mouse_report.y;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }

    return mouse_report;
}

// Call on every layer change.
// Send KBHLayerN (N is layer index)
//      Add M if Swap Hands is active
// Used by Autohotkey to display current layer info.
void update_layer_info(int highLayer, bool hand_swapped) {
    uprintf("KBHLayer%d%s\n", highLayer, hand_swapped ? "M" : "");
    layer_timer = timer_read32();
    is_layer_info_timeout = false;
}


// Send current layer on Swap Hands
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SH_TT:
            update_layer_info(get_highest_layer(layer_state), swap_hands);
            break;

        default:
            break;
    }
}

#ifdef ACHORDION_ENABLE
uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    return 350;
}

bool achordion_eager_mod(uint8_t mod) {
    switch (mod) {
        case MOD_LSFT:
        case MOD_RSFT:
        // case MOD_LCTL:
        // case MOD_RCTL:
            return true; // Eagerly apply Shift and Ctrl mods.
            // return false; // Eagerly apply Shift and Ctrl mods.

        default:
            return false;
    }
}
bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    // Exceptionally consider the following chords as holds, even though they
    // are on the same hand in Dvorak.
    switch (tap_hold_keycode) {
        case HOME_T_CTL:
            if ( other_keycode == FR_S || other_keycode == FR_W || other_keycode == HOME_S_SFT || other_keycode == HOME_A_GUI
        //         // || other_keycode == LT(ACCENT_LAYER,KC_SPC)
                || other_keycode == KC_MS_BTN1 || other_keycode == KC_MS_BTN2 || other_keycode == KC_MS_BTN3) {
                return true;
            }
            break;
        case HOME_N_CTL:
            if ( other_keycode == FR_O)
             return true;
            break;
    }

    // Also allow same-hand holds when the other key is in the rows below the
    // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboard is split.
    // Pour Alt-Tab, Alt-Shift-Tab par exemple
    if (other_record->event.key.row % (MATRIX_ROWS / 2) >= 3 ) {
        return true;
    }

    if (tap_hold_record->event.key.row % (MATRIX_ROWS / 2) >= 4) {
        return true;
    }

    // Otherwise, follow the opposite hands rule.
    return achordion_opposite_hands(tap_hold_record, other_record);
}
#endif
void leader_start_user(void) {
    // Do something when the leader key is pressed
}

void leader_end_user(void) {
    if (leader_sequence_one_key(FR_P)) {
        // Enable Pascal case
        enable_xcase_with(OSM(MOD_RSFT));
        add_oneshot_mods(MOD_LSFT);
    } else if (leader_sequence_one_key(FR_S)) {
        // Enable Snake case
        enable_xcase_with(FR_UNDS);
    } else if (leader_sequence_one_key(FR_K)) {
        // Enable Kebab case
        enable_xcase_with(FR_MINS);
    } else if (leader_sequence_one_key(FR_D)) {
        // Enable caps word
        toggle_caps_word();
    } else if (leader_sequence_one_key(FR_N)) {
        // Enable Num word
        toggle_num_word();
    } else if (leader_sequence_one_key(FR_T)) {
        // Enable Capital Snake case
        enable_xcase_with(FR_UNDS);
        toggle_caps_word();
    } else if (leader_sequence_one_key(FR_H)) {
        // Enable Capital Kebab case
        enable_xcase_with(FR_MINS);
        toggle_caps_word();
    } else if (leader_sequence_one_key(FR_C)) {
        // Enable Camel case
        enable_xcase_with(OSM(MOD_RSFT));
    } else if (leader_sequence_one_key(FR_V)) {
        // Enable Path case
        enable_xcase_with(FR_BSLS);
    } else if (leader_sequence_one_key(FR_G)) {
        // Enable Slash case
        enable_xcase_with(FR_SLSH);
    } else if (leader_sequence_one_key(FR_R)) {
        SEND_STRING("Rapha");
        tap_code16(S(FR_CIRC));
        SEND_STRING("el");

    } else if (leader_sequence_two_keys(FR_R, FR_D)) {
        SEND_STRING("DEBEUGNY");
    } else if (leader_sequence_two_keys(FR_R, FR_T)) {
        SEND_STRING("06 12 43 19 94");
    }

}
bool remember_last_key_user(uint16_t keycode, keyrecord_t *record, uint8_t *remembered_mods){
    switch (keycode) {
        case RDB_REPEAT:
        case RDB_ALT_REPEAT:
        // case LT(APPS_LAYER,RDB_REPEAT):
        // case LT(MEDIA,RDB_ALT_REPEAT):

            return false;
        case LT(EXCEL_LAYER,KC_SPC):
        case LT(NAV,KC_TAB):
        case LT( NUMBERS,KC_ENT):
        case LT(SYMB,KC_BSPC):
            if(!record->tap.count)
            {
                return false;
            }
    }

    return true;
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    bool shifted = (mods & MOD_MASK_SHIFT); // Was Shift held?

    uint16_t clean_keycode = keycode & 0xFF;
    switch (clean_keycode) {

        case KC_TAB:
        {
            bool ctrlled = (mods & MOD_MASK_CTRL); // Was Ctrl held?
            bool altled  = (mods & MOD_MASK_ALT);  // Was Alt held?
            uint16_t tab_returned = (ctrlled) ? C(KC_TAB) : KC_TAB;
            tab_returned = (altled) ? A(tab_returned) : tab_returned;
            if (shifted) {        // If the last key was Shift + Tab,
                return  tab_returned;    // ... the reverse is Tab.
            } else {              // Otherwise, the last key was Tab,
                return  S(tab_returned); // ... and the reverse is Shift + Tab.
            }
        }
    }
    switch (keycode) {
        case HOME_I_ALT:
            return MAG_ION;
        case FR_L:
            return MAG_LES;
        case FR_O:
            return MAG_OUR;
        case HOME_A_GUI:
            return MAG_AIT;
        case FR_M:
            return MAG_MENT;
        case HOME_T_CTL:
            return MAG_TION;
        case HOME_R_ALT:
            return MAG_RAPHAEL;
        case KC_P0:
        case FR_0:
            return MAG_PHONE;
        case RALT_T(FR_D):
            return MAG_DEBUG;
        case FR_LBRC:
            return FR_RBRC;
        case FR_LCBR:
            return FR_RCBR;
    }
    return KC_TRNS;
}


bool process_alt_repeat_macros_user(uint16_t keycode, keyrecord_t *record) {

    if (!record->event.pressed) {
        return true;
    }
    switch (keycode) {

        case MAG_ION: SEND_STRING(/*i*/"on"); break;
        case MAG_LES: SEND_STRING(/*l*/"es");  break;
        case MAG_OUR: SEND_STRING(/*o*/"ur");  break;
        case MAG_AIT: SEND_STRING(/*a*/"it"); break;
        case MAG_MENT: SEND_STRING(/*m*/"ent"); break;
        case MAG_TION: SEND_STRING(/*t*/"ion"); break;
        case MAG_RAPHAEL:  SEND_STRING(/*r*/"apha"); tap_code16(S(FR_CIRC));SEND_STRING("el"); break;
        default:
            return true;
    }
    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // tap_dance_action_t *action;
        //static uint8_t saved_mods   = 0;
    uint16_t       temp_keycode = keycode;
    // uprintf("keycode: %d\n", keycode);

#ifdef ACHORDION_ENABLE
    if (!process_achordion(keycode, record)) { return false; }
#endif
    if (!process_layer_lock(keycode, record, LAYER_LOCK)) { return false; }
    // if (!process_select_word(keycode, record, SELWORD)) { return false; }
    if (!process_custom_shift_keys(keycode, record)) { return false; }
    if (!process_case_modes(keycode, record)) { return false; }
    if (!process_alt_repeat_macros_user(keycode, record)) { return false; }
    mod_state = get_mods();
    weak_mod_state = get_weak_mods();
    // Filter out the actual keycode from MT and LT keys.
/*     if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
        temp_keycode &= 0xFF;
    } */
    switch (temp_keycode) {
        // dynamically generate these.

        // case LT(APPS_LAYER,RDB_REPEAT):
        //     if(record->tap.count && record->event.pressed && get_repeat_key_count() == 0) {
        //             keyrecord_t press;
        //             press.event.pressed = true;
        //             process_repeat_key(QK_REP, &press);
        //             keyrecord_t release;
        //             release.event.pressed = false;
        //             process_repeat_key(QK_REP, &release);
            //     return false;
            // }


            return true;
        case RDB_REPEAT:
            if(record->event.pressed && get_repeat_key_count() == 0) {
                    keyrecord_t press;
                    press.event.pressed = true;
                    process_repeat_key(QK_REP, &press);
                    keyrecord_t release;
                    release.event.pressed = false;
                    process_repeat_key(QK_REP, &release);
                return false;
            }

            return true;

        // case LT(MEDIA,RDB_ALT_REPEAT):
        //     if(record->tap.count && record->event.pressed && get_repeat_key_count() == 0) {
        //         keyrecord_t press;
        //         press.event.pressed = true;
        //         process_repeat_key(QK_AREP, &press);
        //         keyrecord_t release;
        //         release.event.pressed = false;
        //         process_repeat_key(QK_AREP, &release);
            //     return false;
            // }
            // return true;

        case RDB_ALT_REPEAT:
            if(record->event.pressed && get_repeat_key_count() == 0) {
                    keyrecord_t press;
                    press.event.pressed = true;
                    process_repeat_key(QK_AREP, &press);
                    keyrecord_t release;
                    release.event.pressed = false;
                    process_repeat_key(QK_AREP, &release);
                return false;
            }
            return true;

        case SH_OS_TT:
            // One shot tap toggle swap hands with momentary on hold

            uprintf("Tap count : %d -- toggle %d\n", record->tap.count, ONESHOT_TAP_TOGGLE);

            if (record->event.pressed) {
                if (record->tap.count > ONESHOT_TAP_TOGGLE && !record->tap.interrupted) {
                    // Toggle swap hands permanently
                    action_t action;
                    action.code = ACTION_SWAP_HANDS_ON_OFF();
                    //action.swap.code = OP_SH_TOGGLE;

                    uprintf("Swap Hands TOGGLE\n");
                    process_action(record, action);
                } else {
                    // One shot swap hands on hold
                    set_oneshot_swaphands();
                    uprintf("Swap Hands OneShot - Hold\n");
               }
            } else {
                clear_oneshot_swaphands();
                uprintf("Swap Hands OneShot - Release\n");
            }
            return false;


        case K_VSCROLL :
            set_scrolling = record->event.pressed;
        //     if(!record->tap.count && record->event.pressed){
        //         set_scrolling   = !set_scrolling;
        //         }
                return false;
            break;

        case VRSN:
            if (record->event.pressed) {
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
            }
            return false;

            break;
        case VIM_LAST_YANK:
            if (record->event.pressed) {
                SEND_STRING("\"0p");
            }
            return false;
            break;
        case VIM_SYST_CLIPB:
            if (record->event.pressed) {
                SEND_STRING("\"+");
            }
            return false;
            break;
        case ETREM_MAJ: /* ë Ë */
            //alternate_shift(record, ALGR(FR_D), S(ALGR(FR_D)));
            if(record->event.pressed){
                if ((mod_state | get_weak_mods() ) & MOD_MASK_SHIFT )
                {
                    SEND_STRING(SS_TAP(X_LBRC) SS_TAP(X_E));
                }
                else
                {
                    // if (0)
                    if (get_oneshot_mods() & MOD_MASK_SHIFT )
                    {
                    // Otherwise cancel shift mods, press the key, and restore mods.
                    del_weak_mods(MOD_MASK_SHIFT);
#ifndef NO_ACTION_ONESHOT
                    del_oneshot_mods(MOD_MASK_SHIFT);
#endif  // NO_ACTION_ONESHOT
                    // unregister_mods(MOD_MASK_SHIFT);
                    register_mods(MOD_MASK_SHIFT);
                    SEND_STRING(SS_TAP(X_LBRC));
                    // set_mods(mod_state);
                    SEND_STRING(SS_TAP(X_E));
                    unregister_mods(MOD_MASK_SHIFT);

                    }
                    else {
                    uprintf("etrem maj no shift\n");
                        register_code16(ALGR(FR_D));
                    }
                }
            }
            else
                unregister_code16(ALGR(FR_D));
            return false;
        break;

        // case TD(TD_MO_LAYER_KEYP_NAV): // list all tap dance keycodes with tap-hold configurations
        // case TD(TD_MO_LAYER_EVRY_SYMB):
        // case TD(TD_MO_LAYER_CALC_AZERTY):
        // case TD(TD_LIBERK_LALT):
        // case TD(TD_KEYP_RALT):
        //     action = &tap_dance_actions[TD_INDEX(temp_keycode)];
        //     if (!record->event.pressed && action->state.count && !action->state.finished) {
        //         tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
        //         tap_code16(tap_hold->tap);
        //     }
        //     break;
    }
  return true;
}

char* integer_to_binary_string(uint16_t value) {
    static char buffer[17];
    for (int i = 7; i >= 0; i--) {
        buffer[i] = (value & 1) + '0';
        value >>= 1;    // equivalent to value = value / 2  // equivalent to value = value >> 1
    }
    buffer[16] = '\0';
    return buffer;
}

// Returns true if the case modes should terminate, false if they continue
// Note that the keycodes given to this function will be stripped down to
// basic keycodes if they are dual function keys. Meaning a modtap on 'a'
// will pass KC_A rather than LSFT_T(KC_A).
// Case delimiters will also not be passed into this function.
bool terminate_case_modes(uint16_t keycode, const keyrecord_t *record) {
    if (caps_word_enabled() || (get_xcase_state() == XCASE_ON)) {
        switch (keycode) {
            // Keycodes to ignore (don't disable caps word)
            case FR_A:
            case FR_B:
            case FR_C:
            case FR_D:
            case FR_E:
            case FR_F:
            case FR_G:
            case FR_H:
            case FR_I:
            case FR_J:
            case FR_K:
            case FR_L:
            case FR_M:
            case FR_N:
            case FR_O:
            case FR_P:
            case FR_Q:
            case FR_R:
            case FR_S:
            case FR_T:
            case FR_U:
            case FR_V:
            case FR_W:
            case FR_X:
            case FR_Y:
            case FR_Z:
            case KC_P1 ... KC_P0:
            case FR_1 ... FR_0:
            case FR_UCRC:
            case FR_ECRC:
            case FR_ETREM:
            case FR_OCRC:
            case FR_ICRC:
            case FR_ITREM:
            case FR_ACRC:
            case FR_OE:

            case FR_AGRV:
            case FR_EGRV:
            case FR_EACU:
            case FR_UGRV:
            case FR_CCED:
            case ETREM_MAJ:
            // case KC_MINS:
            // case KC_UNDS:
            // case FR_UNDS:
            // case FR_COLN:
            case KC_LEFT:
            case KC_RIGHT:
            case KC_BSPC:

#ifdef CAPS_WORD_INVERT_ON_SHIFT
            case KC_LSFT:
            case KC_RSFT:
#endif
            case TD(TD_SWAP_HANDS):
                // If mod chording disable the mods if chording is not Shift
                // if (record->event.pressed && (get_mods() != 0)) {
                if (record->event.pressed && (get_mods() & !MOD_MASK_SHIFT) != 0) {
                    if(get_mods() & MOD_MASK_SHIFT) {
                        return false;} else {
                        return true;}

                    return true;
                }
                break;
            default:
                if (record->event.pressed) {
                    // if(!caps_word_enabled() && keycode == get_xcase_delimiter()) {
                    if(keycode == get_xcase_delimiter()) {
                        return false;
                    } else {
                        return true;
                    }
                }
                break;
        }
    }
    else if (num_word_enabled()) {
        switch (keycode) {
            // Keycodes to ignore (don't disable num word)
            case FR_1 ... FR_0:
            case KC_1 ... KC_0:
            case KC_EQL:
            case KC_SCLN:
            case KC_MINS:
            case KC_DOT:
            case FR_COMM:
            case FR_DOT:
            case FR_SCLN:

            // Numpad keycodes
            case KC_P1 ... KC_P0:
            case KC_PSLS ... KC_PPLS:
            case KC_PDOT:

            // Misc
            case KC_UNDS:
            case KC_BSPC:
            case KC_DEL:
            case KC_LEFT:
            case KC_RIGHT:
#ifdef CAPS_WORD_INVERT_ON_SHIFT
            case KC_LSFT:
            case KC_RSFT:
#endif
            case TD(TD_SWAP_HANDS):
                return false;

            default:
                if (record->event.pressed) {
                    return true;
                }
                break;
        }
  }

  return false;
}


void tap_dance_tap_hold_momentary_layer_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {

            layer_on(tap_hold->hold-QK_MOMENTARY);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_momentary_layer_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        layer_off(tap_hold->hold-QK_MOMENTARY);

        tap_hold->held = 0;
    }
}



void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD_MOMENTARY_LAYER(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_momentary_layer_finished, tap_dance_tap_hold_momentary_layer_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }


#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

void bootloader_reset(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 3) {
        // Reset the keyboard if you tap the key more than three times
        reset_keyboard();
        reset_tap_dance(state);
    }
}

void reboot_keyboard(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 3) {
        // Reset the keyboard if you tap the key more than three times
        soft_reset_keyboard();
        reset_tap_dance(state);
    }
}


// static td_tap_t td_state = {
//     .is_press_action = true,
//     .state = TD_NONE
// };


// void spc_excel_finished(tap_dance_state_t *state, void *user_data) {
//     td_state.state = cur_dance(state);
//     switch (td_state.state) {
//         case TD_SINGLE_TAP:
//             tap_code(KC_SPC);
//             break;
//         case TD_SINGLE_HOLD:
//             layer_on(EXCEL_LAYER);
//             break;
//         default:
//             break;
//     }

// }

// void spc_excel_reset(tap_dance_state_t *state, void *user_data) {
//     if(td_state.state == TD_SINGLE_HOLD) {
//         layer_off(EXCEL_LAYER);
//     }
//     td_state.state = TD_NONE;
// }

tap_dance_action_t tap_dance_actions[] = {
    // [TD_RSFT_LAYER0] = ACTION_TAP_DANCE_LAYER_MOVE(KC_RSFT, BASE),
    [TD_TRIPLE_BOOTLOADER] = ACTION_TAP_DANCE_FN(bootloader_reset),
    [TD_TRIPLE_REBOOT] = ACTION_TAP_DANCE_FN(reboot_keyboard),
    // [TD_MEDIA_BASE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mb_finished, mb_reset),
    // [TD_GAME_APPS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, g_apps_finished, g_apps_reset),
    [TD_SWAP_HANDS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, swh_finished, swh_reset),
    [TD_OSM_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, osmShift_finished, osmShift_reset),
    [TD_OSM_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, osmShift_finished, osmShift_reset),
    // [TD_SPC_EXCEL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, spc_excel_finished, spc_excel_reset),


};


// Call on every layer change.
// Send KBHLayerN (N is layer index)
//      Add M if Swap Hands is active
// Used by Autohotkey to display current layer info.

layer_state_t layer_state_set_user(layer_state_t state) {
    // uprintf("KBHLayer%u%s\n", get_highest_layer(state), swap_hands ? "M" : "");
    update_layer_info(get_highest_layer(state), is_swap_hands_on());
    return state;
}

void matrix_scan_user(void) {
#ifdef ACHORDION_ENABLE
    achordion_task();
#endif
    layer_lock_task();
    xcase_task();
}

// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {
/*     if (state->count == 1) {
    } else if (state->count == 2) return TD_DOUBLE_TAP;
    else return TD_UNKNOWN; */

    switch (state->count) {
        case 1:
            if (!state->pressed) return TD_SINGLE_TAP;
            // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
            else return TD_SINGLE_HOLD;
            // if (!state->pressed) return TD_SINGLE_TAP;
            // else return TD_SINGLE_HOLD;
        case 2:
            return TD_DOUBLE_TAP;
        case 3:
            return TD_TRIPLE_TAP;
        case 4:
            return TD_QUAD_TAP;
        default:
            return TD_UNKNOWN;
    }
}

// Initialize tap structure associated with example tap dance key
static td_tap_t ql_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// // Functions that control what our tap dance key does
// void mb_finished(tap_dance_state_t *state, void *user_data) {
//     ql_tap_state.state = cur_dance(state);
//     switch (ql_tap_state.state) {
// /*         case TD_SINGLE_TAP:
//             tap_code(KC_QUOT);
//             break; */
//         case TD_SINGLE_HOLD:
//         case TD_DOUBLE_TAP:
//             layer_on(MEDIA);
//             break;
//         case TD_TRIPLE_TAP:
//             layer_move(AZERTY);
//             break;
//         case TD_QUAD_TAP:
//             layer_move(BASE);
//             break;
//         default:
//             break;
//     }
// }

// void mb_reset(tap_dance_state_t *state, void *user_data) {
//     // If the key was held down and now is released then switch off the layer
//     if (ql_tap_state.state == TD_SINGLE_HOLD) {
//         layer_off(MEDIA);
//     }
//     ql_tap_state.state = TD_NONE;
// }

// // Functions that control what our tap dance key does
// void g_apps_finished(tap_dance_state_t *state, void *user_data) {
//     ql_tap_state.state = cur_dance(state);
//     switch (ql_tap_state.state) {
//         case TD_SINGLE_TAP:
//             set_oneshot_layer(APPS_LAYER, ONESHOT_START);
//             clear_oneshot_layer_state(ONESHOT_PRESSED);
//             break;
//         case TD_SINGLE_HOLD:
//         // case TD_SINGLE_TAP:
//             layer_on(APPS_LAYER);
//             break;
//         case TD_DOUBLE_TAP:
//             layer_on(APPS_LAYER);
//             break;
//        case TD_TRIPLE_TAP:
//             layer_move(GAME);
//             break;

//         case TD_QUAD_TAP:
//             layer_move(BASE);
//             break;
//         default:
//             break;
//     }
// }

// void g_apps_reset(tap_dance_state_t *state, void *user_data) {
//     // If the key was held down and now is released then switch off the layer
//     // if (ql_tap_state.state == TD_SINGLE_HOLD) {
//     //     layer_off(APPS_LAYER);
//     // }
//     switch(ql_tap_state.state) {
//         case TD_SINGLE_TAP:
//             // clear_oneshot_layer_state(APPS_LAYER);
//             break;
//         case TD_SINGLE_HOLD:
//             layer_off(APPS_LAYER);
//             break;
//         default:
//         break;


//     }
//     ql_tap_state.state = TD_NONE;
// }



void osmShift_finished(tap_dance_state_t *state, void *user_data) {
    int left_hand = TAP_DANCE_KEYCODE(state) == TD(TD_OSM_LSFT) ? 1 : 0;

    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            if(left_hand)
                set_oneshot_mods(MOD_LSFT);
            else
                set_oneshot_mods(MOD_BIT(KC_RSFT));
            break;
        case TD_SINGLE_HOLD:
            if(left_hand)
                register_code(KC_LSFT);
            else
                register_code(KC_RSFT);
            break;
        case TD_DOUBLE_TAP:
            enable_caps_word();
            break;
        case TD_TRIPLE_TAP:
            register_code(KC_CAPS_LOCK);
            break;

        default:
            break;
    }
}


void osmShift_reset(tap_dance_state_t *state, void *user_data) {
     int left_hand = TAP_DANCE_KEYCODE(state) == TD(TD_OSM_LSFT) ? 1 : 0;
   // If the key was held down and now is released then switch off the layer
    switch (ql_tap_state.state) {
        case TD_SINGLE_HOLD:
            if(left_hand)
                unregister_code(KC_LSFT);
            else
                unregister_code(KC_RSFT);
            break;
        case TD_TRIPLE_TAP:
            unregister_code(KC_CAPS_LOCK);
            break;
        default:
            break;
    }
    ql_tap_state.state = TD_NONE;
}

void oneshot_swap_hands_changed_user(bool swap_hands_state) {
    update_layer_info(get_highest_layer(layer_state), swap_hands);
}

// Functions that control what our tap dance key does
void swh_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            set_oneshot_swaphands();
            break;
        case TD_SINGLE_HOLD:
            set_oneshot_swaphands();
            break;
        case TD_DOUBLE_TAP:
            // swap_hands = !swap_hands;
            swap_hands_toggle();

            break;
        default:
            break;
    }
    oneshot_swap_hands_changed_user(is_swap_hands_on());
}


void swh_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            release_oneshot_swaphands();

            break;
        case TD_SINGLE_HOLD:
            clear_oneshot_swaphands();
            break;
        default:
            break;
    }
    ql_tap_state.state = TD_NONE;
}



// Set a long-ish tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 275;
        default:
            return TAPPING_TERM;
    }
}





// Ajout renvoi de l'état des layers en HID pour s'assurer de la bonne indication dans KBLayerHelper

void refresh_layer_info(void) {
    layer_timer = timer_read32();
    if(is_layer_info_timeout){
        is_layer_info_timeout = false;
        update_layer_info(get_highest_layer(layer_state), is_swap_hands_on());
        layer_timer = 0;
    }
}

void check_layer_info_timeout(void) {
    if (!is_layer_info_timeout && layer_timer != 0 && timer_elapsed32(layer_timer) > 1000) {
        is_layer_info_timeout = true;
        refresh_layer_info();
    }
}

void housekeeping_task_user(void) {
    check_layer_info_timeout();
}
