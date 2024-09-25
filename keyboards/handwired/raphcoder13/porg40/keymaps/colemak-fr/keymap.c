#include QMK_KEYBOARD_H
// #include "birdy44.h"
#include <version.h>
#include <print.h>
#include <transactions.h>
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

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)
//#pragma region declarations

static uint8_t last_layer = 0;
static uint32_t layer_timer;
bool is_layer_info_timeout = false;
bool is_recording_macro = false;

static void refresh_layer_info(void);
static void update_layer_info(int highLayer, bool hand_swapped) ;
static void check_layer_info_timeout(void);

uint8_t mod_state, weak_mod_state;
bool set_scrolling = false;



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
    bool       is_press_action;
    td_state_t state;
} td_tap_t;

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;


// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
// Pour Media Layer and Base
void osmShift_finished(tap_dance_state_t *state, void *user_data);
void osmShift_reset(tap_dance_state_t *state, void *user_data);
// Pour Swap Hands Tap Dance
void swh_finished(tap_dance_state_t *state, void *user_data);
void swh_reset(tap_dance_state_t *state, void *user_data);

const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 0}},
    {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 1}},
    {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 2}},
    {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 3}},
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 4}},
    {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 5}},
    {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 6}},
    {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 7}}};

#ifdef RGBLIGHT_ENABLE
static uint32_t key_timer;               // timer for last keyboard activity, use 32bit value and function to make longer idle time possible
static void     refresh_rgb(void);       // refreshes the activity timer and RGB, invoke whenever any activity happens
static void     check_rgb_timeout(void); // checks if enough time has passed for RGB to timeout
bool            is_rgb_timeout = false;  // store if RGB has timed out or not in a boolean

const uint8_t RGBLED_BREATHING_INTERVALS[] PROGMEM     = {2,6,2,6};
const uint8_t RGBLED_RAINBOW_SWIRL_INTERVALS[] PROGMEM = {5, 5, 5} ;
uint8_t _mycolors[13][3] = {{HSV_WHITE},   {HSV_CHARTREUSE}, {HSV_GOLD}, {HSV_GOLDENROD}, {HSV_SPRINGGREEN}, {HSV_BLUE},   {HSV_PURPLE},
                                  {HSV_RED},  {HSV_MAGENTA}, {HSV_ORANGE}, {HSV_CYAN},{HSV_GREEN}, {HSV_CORAL}, };
#endif

//#pragma endregion //declarations

//#pragma region Layers

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[COLEMAK_FR_1DK] = LAYOUT_wrapper(
                            ________COLEMAK_FR_1DK_L1________, KC_UP, KC_NO,                                       ________COLEMAK_FR_1DK_R1________,
        TD(TD_OSM_LSFT),	________COLEMAK_FR_1DK_L2________, KC_DOWN, KC_NO,                                        ________COLEMAK_FR_1DK_R2________,   FR_UNDS,
        TD(TD_SWAP_HANDS),	________COLEMAK_FR_1DK_L3________, KC_LEFT, KC_NO,                                       ________COLEMAK_FR_1DK_R3________,	TD(TD_SWAP_HANDS),
                LT(APPS_LAYER, KC_ESC), LT(EXCEL_LAYER, KC_SPC), LT(NAV, KC_TAB), KC_RIGHT, KC_NO,               LT(NUMBERS, KC_ENT), LT(SYMB, KC_BSPC), LT(FUN, KC_DEL)),

    [GAME]   = LAYOUT_wrapper(
                     ____________QWERTY_L1____________ , KC_TRNS, KC_TRNS               , ____________QWERTY_R1____________,
            KC_TRNS,____________QWERTY_L2____________   , KC_TRNS, KC_TRNS                            , ____________QWERTY_R2____________                 , KC_QUOT,
            KC_LCTL,____________QWERTY_L3____________   , KC_TRNS, KC_TRNS                            , ____________QWERTY_R3____________                 , KC_RCTL,
                KC_TRNS, KC_TRNS, KC_TRNS               , KC_TRNS, KC_TRNS              ,          KC_TRNS, KC_TRNS, KC_TRNS),
    [SYMB]   = LAYOUT_wrapper(
                  ____________SYMBOL_L1____________   , KC_MS_WH_UP, KC_TRNS                            ,    ____________SYMBOL_R1____________                ,
            KC_CAPS, ____________SYMBOL_L2____________   , KC_MS_WH_DOWN, KC_TRNS                            ,    ____________SYMBOL_R2____________                , KC_NO,
            KC_TRNS, ____________SYMBOL_L3____________  , KC_MS_WH_LEFT, KC_TRNS                             ,    ____________SYMBOL_R3____________                , KC_TRNS,
                TG(GAME), QK_LEAD, KC_TAB               , KC_MS_WH_RIGHT, KC_NO,                   KC_ENT,	KC_TRNS,	KC_DEL),
    [NUMBERS] = LAYOUT_wrapper(
                  ____________NUMBS_L1____________    , KC_TRNS, KC_TRNS                            , ____________NUMBS_R1____________                 ,
            FR_LPRN,____________NUMBS_L2____________   , KC_TRNS, KC_TRNS                             , ____________NUMBS_R2____________                 , KC_NO,
            KC_TRNS,____________NUMBS_L3____________   , KC_TRNS, KC_TRNS                             , ____________NUMBS_R3____________                 , KC_TRNS,
                FR_DOT, KC_SPC,	KC_TAB                 , KC_TRNS, KC_TRNS               ,           KC_TRNS,	KC_BSPC,	KC_DEL),
    [NAV]          = LAYOUT_wrapper(
                    ____________NAV_L1____________  , KC_TRNS, KC_TRNS                                , ____________NAV_R1____________                  ,
            KC_CAPS,____________NAV_L2____________  , KC_TRNS, KC_TRNS                                , ____________NAV_R2____________                  , KC_NUM,
            KC_TRNS,____________NAV_L3____________  , KC_TRNS, KC_TRNS                                , ____________NAV_R3____________                  , KC_TRNS,
                KC_TRNS, KC_TRNS, KC_TRNS           , KC_TRNS, KC_TRNS                  ,             KC_TRNS, KC_TRNS, KC_TRNS),
    [FUN]          = LAYOUT_wrapper(
            ____________FUN_L1____________           , KC_MS_UP, KC_TRNS                       , ____________FUN_R1____________                  ,
            KC_NO,   ____________FUN_L2____________   , KC_MS_DOWN, KC_TRNS                               , ____________FUN_R2____________                  ,  KC_NO,
            KC_TRNS, ____________FUN_L3____________   , KC_MS_LEFT, KC_TRNS                               , ____________FUN_R3____________                  , KC_TRNS,
                KC_TRNS, KC_TRNS, KC_TRNS,             KC_MS_RIGHT, KC_TRNS                   ,          KC_TRNS, KC_TRNS, KC_TRNS),
    [APPS_LAYER]   = LAYOUT_wrapper(
            ____________APPS_L1___________            , KC_TRNS, KC_TRNS                      , ____________APPS_R1___________                  ,
            KC_NO,   ____________APPS_L2___________   , KC_TRNS, KC_TRNS                               , ____________APPS_R2___________                  , KC_NO,
            KC_TRNS, ____________APPS_L3___________   , KC_TRNS, KC_TRNS                               , ____________APPS_R3___________                  , KC_TRNS,
                KC_TRNS, KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS                  ,           KC_TRNS, KC_TRNS, KC_TRNS),
    [ACCENT_COLEMAK_LAYER] = LAYOUT_wrapper(
        ___________ACCENT_NEW_COLEMAK_L1___________           , KC_TRNS, KC_TRNS       ,___________ACCENT_NEW_COLEMAK_R1___________                  ,
        KC_TRNS,___________ACCENT_NEW_COLEMAK_L2___________   , KC_TRNS, KC_TRNS               ,___________ACCENT_NEW_COLEMAK_R2___________                  ,	KC_NO,
        KC_TRNS,___________ACCENT_NEW_COLEMAK_L3___________   , KC_TRNS, KC_TRNS               ,___________ACCENT_NEW_COLEMAK_R3___________                  ,	KC_TRNS,
                KC_TRNS, SAGR(KC_SPC),	KC_TRNS,				KC_TRNS, KC_TRNS                  ,KC_TRNS,	KC_TRNS,	KC_DEL),
    [MOUSE_LAYER] = LAYOUT_wrapper(
            ___________MOUSE_L1___________           , KC_TRNS, KC_TRNS, ___________MOUSE_R1___________                ,
            KC_NO,   ___________MOUSE_L2___________  , KC_TRNS, KC_TRNS         , ___________MOUSE_R2___________                ,  KC_NO,
            KC_TRNS, ___________MOUSE_L3___________  , KC_TRNS, KC_TRNS         , ___________MOUSE_R3___________                , KC_TRNS,
             KC_MS_BTN2, KC_MS_BTN1, KC_MS_BTN3,      KC_TRNS, KC_TRNS                  ,      KC_TRNS, KC_TRNS, KC_TRNS),

    [EXCEL_LAYER] = LAYOUT_wrapper(
            ___________EXCEL_L1___________           , KC_TRNS, KC_TRNS        , ___________EXCEL_R1___________                  ,
            KC_NO,  ___________EXCEL_L2___________   , KC_TRNS, KC_TRNS                , ___________EXCEL_R2___________                  , KC_NUM,
            KC_TRNS,___________EXCEL_L3___________   , KC_TRNS, KC_TRNS                , ___________EXCEL_R3___________                  , KC_TRNS,
                KC_TRNS, KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS                  ,             KC_PENT, KC_BSPC, KC_EQL),

};

//#pragma endregion //Layers



//#pragma region PointingDevice

void pointing_device_init_user(void) {
    set_auto_mouse_layer(MOUSE_LAYER); // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of <mouse_layer>
    set_auto_mouse_enable(true);       // always required before the auto mouse feature will work
}

#define SCROLL_DIVISOR_H 64.0
#define SCROLL_DIVISOR_V 64.0


report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (set_scrolling) {
        mouse_report.h = mouse_report.x;
        mouse_report.v = mouse_report.y;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }

    return mouse_report;
}
    // Update accumulated scroll values by subtracting the integer parts


//#pragma endregion


//#pragma region Features

void matrix_scan_user(void) {
#ifdef ACHORDION_ENABLE
    achordion_task();
#endif
    layer_lock_task();
    xcase_task();
}

#ifdef ACHORDION_ENABLE
uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    return 550;
}

bool achordion_eager_mod(uint8_t mod) {
    switch (mod) {
        case MOD_LSFT:
        case MOD_RSFT:
        case MOD_LCTL:
        case MOD_RCTL:
            // return true; // Eagerly apply Shift and Ctrl mods.
            return false; // Eagerly apply Shift and Ctrl mods.

        default:
            return false;
    }
}
bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    // Exceptionally consider the following chords as holds, even though they
    // are on the same hand in Dvorak.
    switch (tap_hold_keycode) {
        // case LT(NAV, KC_TAB): // for clipboard operation.
        //     if (other_keycode == KC_A || other_keycode == KC_W || other_keycode == KC_F || other_keycode == KC_P || other_keycode == KC_G || other_keycode == KC_V) {
        //         return true;
        //     }
        //     break;
        // case LT(SYMB, KC_BSPC): // for Symbol.
        //     if (other_keycode == KC_L || other_keycode == KC_U || other_keycode == KC_H || other_keycode == FR_COMM) {
        //         return true;
        //     }
        //     break;

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

        // // Pour Ergo-L
        // case N_HR_CTL:
        //     if (other_keycode == FR_S || other_keycode == FR_W || other_keycode == S_HR_LALT ||
        //         other_keycode == A_HR_GUI
        //         // || other_keycode == LT(ACCENT_LAYER,KC_SPC)
        //         || other_keycode == KC_MS_BTN1 || other_keycode == KC_MS_BTN2 || other_keycode == KC_MS_BTN3)
        //         return true;
        //     break;
    }

    // Also allow same-hand holds when the other key is in the rows below the
    // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboard is split.
    // Pour Alt-Tab, Alt-Shift-Tab par exemple
    if (other_record->event.key.row % (MATRIX_ROWS / 2) >= 3 ) {
        return true;
    }

    // When the hold key is in the rows below the alphas (layers), allow the same-hand holds
    if (tap_hold_record->event.key.row % (MATRIX_ROWS / 2) >= 3 ) {
        return true;
    }
    // Otherwise, follow the opposite hands rule.
    return achordion_opposite_hands(tap_hold_record, other_record);
}
#endif


void leader_start_user(void) {
    // Do something when the leader key is pressed
}

// All xcase functions driven by leader key
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
    }

}


bool remember_last_key_user(uint16_t keycode, keyrecord_t *record, uint8_t *remembered_mods){
    switch (keycode) {
        case RDB_REPEAT:
        case RDB_ALT_REPEAT:
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
        case MAG_DEBUG:  SEND_STRING(/*d*/"ebug"); break;
        case MAG_PHONE:  SEND_STRING(/*0*/"12 34 56 78"); break;

        default:
            return true;
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // tap_dance_action_t *action;
    // static uint8_t saved_mods   = 0;
    uint16_t temp_keycode = keycode;
    // uprintf("keycode: %d\n", keycode);

#ifdef ACHORDION_ENABLE
    if (!process_achordion(keycode, record)) { return false; }
#endif
    if (!process_layer_lock(keycode, record, LAYER_LOCK)) { return false; }
    // if (!process_select_word(keycode, record, SELWORD)) { return false; }
    if (!process_custom_shift_keys(keycode, record)) { return false; }
    if (!process_case_modes(keycode, record)) { return false; }
    if (!process_alt_repeat_macros_user(keycode, record)) { return false; }




    mod_state      = get_mods();
    weak_mod_state = get_weak_mods();

    switch (temp_keycode) {
            // dynamically generate these.
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

        case RDB_ALT_REPEAT:
            // uprintf("RDB_ALT_REPEAT: %d\n", record->tap.count);
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

            // uprintf("Tap count : %d -- toggle %d\n", record->tap.count, ONESHOT_TAP_TOGGLE);

            if (record->event.pressed) {
                if (record->tap.count > ONESHOT_TAP_TOGGLE && !record->tap.interrupted) {
                    // Toggle swap hands permanently
                    action_t action;
                    action.code = ACTION_SWAP_HANDS_ON_OFF();
                    // action.swap.code = OP_SH_TOGGLE;

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


        case VRSN:
            if (record->event.pressed) {
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
#ifdef ACHORDION_ENABLE
    uprintf("ACHORDION\n");
    #else
    uprintf("NO ACHORDION\n");
#endif
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
            if (record->event.pressed) {
                if ((mod_state | get_weak_mods() ) & MOD_MASK_SHIFT )
                {
                    SEND_STRING(SS_TAP(X_LBRC) SS_TAP(X_E));
                }
                else
                {
                    if (get_oneshot_mods() & MOD_MASK_SHIFT) {
                        // Otherwise cancel shift mods, press the key, and restore mods.
                        del_weak_mods(MOD_MASK_SHIFT);
#ifndef NO_ACTION_ONESHOT
                        del_oneshot_mods(MOD_MASK_SHIFT);
#endif // NO_ACTION_ONESHOT
                        register_mods(MOD_MASK_SHIFT);
                        SEND_STRING(SS_TAP(X_LBRC));
                        SEND_STRING(SS_TAP(X_E));
                        unregister_mods(MOD_MASK_SHIFT);

                    } else {
                        uprintf("etrem maj no shift\n");
                        register_code16(ALGR(FR_D));
                    }
                }
            }
            else
                unregister_code16(ALGR(FR_D));
            return false;
            break;

    }
    return true;
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
                if (record->event.pressed && (get_mods() & !MOD_MASK_SHIFT) != 0) {
                    if(get_mods() & MOD_MASK_SHIFT) {
                        return false;} else {
                        return true;}

                    return true;
                }
                break;
            default:
                if (record->event.pressed) {
                    if(keycode == get_xcase_delimiter()) {
                        return false;
                    } else {
                        return true;
                    }
                }
                break;
        }
  } else if (num_word_enabled()) {
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



//#pragma region TapDances

void tap_dance_tap_hold_momentary_layer_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {

            layer_on(tap_hold->hold - QK_MOMENTARY);
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
        layer_off(tap_hold->hold - QK_MOMENTARY);

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
        soft_reset_keyboard();
        reset_tap_dance(state);
    }
}
tap_dance_action_t tap_dance_actions[] = {
    [TD_TRIPLE_BOOTLOADER] = ACTION_TAP_DANCE_FN(bootloader_reset),
    [TD_TRIPLE_REBOOT] = ACTION_TAP_DANCE_FN(reboot_keyboard),
    [TD_SWAP_HANDS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, swh_finished, swh_reset),
    [TD_OSM_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, osmShift_finished, osmShift_reset),
    [TD_OSM_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, osmShift_finished, osmShift_reset),

};

// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {

    switch (state->count) {
        case 1:
            if (!state->pressed) return TD_SINGLE_TAP;
            // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
            else return TD_SINGLE_HOLD;
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
static td_tap_t ql_tap_state = {.is_press_action = true, .state = TD_NONE};

// Functions that control what our tap dance key does

// Tap dance for shift keys :
// - One shot mode on single tap
// - Hold mode on single hold
// - Caps word on double tap
// - Caps lock on triple tap
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

// Swap hands tap dance
// - One shot mode on single tap
// - Hold mode on single hold
// - Toggle swap hands on double tap
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

//#pragma endregion //TapDances

#ifdef RGBLIGHT_ENABLE
bool dynamic_macro_record_start_user(int8_t direction)
{

    rgblight_sethsv_noeeprom(HSV_RED);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL + (direction==1? 0:5));
    is_recording_macro = true;
    return true;
}

bool dynamic_macro_record_key_user(int8_t direction, keyrecord_t *record)
{
    if(rgblight_get_mode() < RGBLIGHT_MODE_RAINBOW_SWIRL && rgblight_get_mode() > RGBLIGHT_MODE_RAINBOW_SWIRL+5)
        rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL + (direction==1? 0:5));
    return true;
}

bool dynamic_macro_record_end_user(int8_t direction)
{
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    is_recording_macro = false;
    return true;
}
void refresh_rgb(void) {
    key_timer = timer_read32(); // store time of last refresh
    if (is_rgb_timeout) {
        print("Waking up RGB\n");
        is_rgb_timeout = false;
        rgblight_wakeup();
        // update_rgblight( get_highest_layer(layer_state), is_swap_hands_on());
    }
}
void check_rgb_timeout(void) {
    if (!is_rgb_timeout && timer_elapsed32(key_timer) > RGBLIGHT_TIMEOUT) // check if RGB has already timeout and if enough time has passed
    {
        print("Suspending RGB\n");
        rgblight_suspend();
        is_rgb_timeout = true;
    }
}


const rgblight_segment_t PROGMEM my_mouse_layer[] =  RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_ORANGE},
    {5, 2, HSV_ORANGE},
    {10, 1, HSV_ORANGE},
    {12, 1, HSV_ORANGE},
    {18, 1, HSV_ORANGE}
);

const rgblight_segment_t PROGMEM my_number_layer[] =  RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, HSV_CYAN},
    {3, 1, HSV_CYAN},
    {5, 2, HSV_CYAN},
    {8, 1, HSV_CYAN},
    {10, 1, HSV_CYAN},
    {12, 2, HSV_CYAN},
    {15, 1, HSV_CYAN},
    {17, 2, HSV_CYAN},
    {20, 1, HSV_CYAN},
    {22, 2, HSV_CYAN}
);

const rgblight_segment_t PROGMEM my_accent_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 5, HSV_RED}
);
// gblight_segment_t PROGMEM my_num_layer[] =  RGBLIGHT_LAYER_SEGMENTS(
//     {0, 1, HSV_CYAN},
//     {3, 1, HSV_CYAN},
//     {5, 4, HSV_CYAN},
//     {10, 1, HSV_CYAN},
//     {13, 1, HSV_CYAN},
//     {15, 1, HSV_CYAN},
//     {18, 1, HSV_CYAN},
//     {20, 1, HSV_CYAN},
//     {23, 1, HSV_CYAN}
// );

const rgblight_segment_t PROGMEM my_symbol_layer[] = RGBLIGHT_LAYER_SEGMENTS(

    {1,2, HSV_CHARTREUSE},
    {5,1, HSV_CHARTREUSE},
    {10,3, HSV_CHARTREUSE},
    {17,1, HSV_CHARTREUSE},
    {20,2, HSV_CHARTREUSE}

);

const rgblight_segment_t PROGMEM my_excel_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, HSV_GREEN},
    {2,1, HSV_GREEN},
    {5,1, HSV_GREEN},
    {7,1, HSV_GREEN},
    {11,1, HSV_GREEN},
    {15,1, HSV_GREEN},
    {17,1, HSV_GREEN},
    {20,1, HSV_GREEN},
    {22,1, HSV_GREEN}
);

const rgblight_segment_t PROGMEM my_nav_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {2, 1, HSV_TURQUOISE},
    {7,1, HSV_TURQUOISE},
    {10,5, HSV_TURQUOISE},
    {17,1, HSV_TURQUOISE},
    {22,1, HSV_TURQUOISE}
);


const rgblight_segment_t PROGMEM my_func_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_AZURE},
    {5,1, HSV_AZURE},
    {10,2, HSV_AZURE},
    {15,1, HSV_AZURE},
    {20,1, HSV_AZURE}
);

const rgblight_segment_t PROGMEM my_apps_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1,2, HSV_SPRINGGREEN},
    {5,1, HSV_SPRINGGREEN},
    {8, 1, HSV_SPRINGGREEN},
    {10, 4, HSV_SPRINGGREEN},
    {15, 1, HSV_SPRINGGREEN},
    {18, 1, HSV_SPRINGGREEN},
    {20, 1, HSV_SPRINGGREEN},
    {23, 1, HSV_SPRINGGREEN}
);

const rgblight_segment_t PROGMEM my_game_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 3, HSV_PURPLE},
    {5, 1, HSV_PURPLE},
    {10, 1, HSV_PURPLE},
    {12, 2, HSV_PURPLE},
    {15, 1, HSV_PURPLE},
    {18, 1, HSV_PURPLE},
    {21, 3, HSV_PURPLE}

);
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {24,1, HSV_RED}
);

const rgblight_segment_t PROGMEM my_base_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 25, HSV_PURPLE}
);
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_capslock_layer,
    my_base_layer,
    my_game_layer,
    my_symbol_layer,
    my_number_layer,
    my_nav_layer,
    my_func_layer,
    my_apps_layer,
    my_mouse_layer,
    my_excel_layer,
    my_accent_layer
);


bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(0, led_state.caps_lock);
    return true;
}

#endif

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (host_keyboard_led_state().caps_lock) {
        for (uint8_t i = led_min; i < led_max; i++) {
            if (g_led_config.flags[i] & LED_FLAG_INDICATOR) {
                rgb_matrix_set_color(i, RGB_RED);
            }
        }
    }
    return false;
}
//     if (get_highest_layer(layer_state) != last_layer) {

//         uint8_t layer = get_highest_layer(layer_state);
//         last_layer = layer;
//    switch(layer){
//         case BASE:
//             rgb_matrix_mode_noeeprom(RGB_MATRIX_TYPING_HEATMAP);
//             break;
//         case GAME:
//             rgb_matrix_mode_noeeprom(RGB_MATRIX_GRADIENT_LEFT_RIGHT);
//             break;
//         case SYMB:
//             rgb_matrix_mode_noeeprom(RGB_MATRIX_FLOWER_BLOOMING);
//             break;
//         case NUMBERS:
//             rgb_matrix_mode_noeeprom(RGB_MATRIX_SPLASH);
//             break;
//         case NAV:
//             rgb_matrix_mode_noeeprom(RGB_MATRIX_RIVERFLOW);
//             break;
//         case FUN:
//             rgb_matrix_mode_noeeprom(RGB_MATRIX_HUE_WAVE);
//             break;
//         case APPS_LAYER:
//             rgb_matrix_mode_noeeprom(RGB_MATRIX_BREATHING);
//             break;
//         case MOUSE_LAYER:
//             rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_OUT_IN);
//             break;
//         case EXCEL_LAYER:
//             rgb_matrix_mode_noeeprom(RGB_MATRIX_DUAL_BEACON);

//      }

//     return false;
// }
#endif



void update_layer_info(int highLayer, bool hand_swapped) {
    uprintf("KBHLayer%d%s\n", highLayer, hand_swapped ? "M" : "");
    layer_timer = timer_read32();
    is_layer_info_timeout = false;
    if(highLayer == last_layer)
        return;
    last_layer = highLayer;
#ifdef RGB_MATRIX_ENABLE
    switch(highLayer){
        case BASE:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_TYPING_HEATMAP);
            break;
        case GAME:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_GRADIENT_LEFT_RIGHT);
            break;
        case SYMB:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_FLOWER_BLOOMING);
            break;
        case NUMBERS:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_RAINBOW_MOVING_CHEVRON);
            break;
        case NAV:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_RIVERFLOW);
            break;
        case FUN:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_HUE_WAVE);
            break;
        case APPS_LAYER:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_BREATHING);
            break;
        case MOUSE_LAYER:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_OUT_IN);
            break;
        case EXCEL_LAYER:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_DUAL_BEACON);

    }
#endif

#ifdef RGBLIGHT_ENABLE
    // rgblight_set_layer_state(0, highLayer == BASE);
    rgblight_set_layer_state(1, false);
    rgblight_set_layer_state(2, highLayer == GAME);
    rgblight_set_layer_state(3, highLayer == SYMB);
    rgblight_set_layer_state(4, highLayer == NUMBERS);
    rgblight_set_layer_state(5, highLayer == NAV);
    rgblight_set_layer_state(6, highLayer == FUN);
    rgblight_set_layer_state(7, highLayer == APPS_LAYER);
    rgblight_set_layer_state(8, highLayer == MOUSE_LAYER);
    rgblight_set_layer_state(9, highLayer == EXCEL_LAYER);
    rgblight_set_layer_state(10, highLayer == ACCENT_COLEMAK_LAYER);
    if(!is_recording_macro)
    {
        if(hand_swapped){
            rgblight_sethsv_noeeprom(HSV_RED);
            if(rgblight_get_mode() != RGBLIGHT_MODE_SNAKE + 3)
                rgblight_mode_noeeprom(RGBLIGHT_MODE_SNAKE + 3);

            // rgblight_sethsv_noeeprom(HSV_TURQUOISE);
            // if(rgblight_get_mode() != RGBLIGHT_MODE_BREATHING)
            //     rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
        // rgblight_sethsv_noeeprom(HSV_GOLD);
        }
        else{
            if (rgblight_get_mode() != RGBLIGHT_MODE_STATIC_LIGHT)
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            // rgblight_sethsv_noeeprom(0, 0, 0);
    rgblight_sethsv_noeeprom(0,0,0);
        }
    }
    // rgblight_set_layer_state(1, layer_state_cmp(layer_state, NAV));
    // rgblight_set_layer_state(2, layer_state_cmp(layer_state, SYMB));
    // update_rgblight(highLayer, hand_swapped);
#endif
}


layer_state_t layer_state_set_user(layer_state_t state) {
    update_layer_info(get_highest_layer(state), is_swap_hands_on());
    return state;
}

// Ajout renvoi de l'état des layers en HID pour s'assurer de la bonne indication dans KBLayerHelper
//
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
#ifdef RGBLIGHT_TIMEOUT
    check_rgb_timeout();
#endif
}
// Send current layer on Swap Hands
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef RGBLIGHT_TIMEOUT
    if (record->event.pressed) refresh_rgb();
#endif
    switch (keycode) {
        case SH_TT:
            update_layer_info(get_highest_layer(layer_state), is_swap_hands_on());
            break;

        default:
            break;
    }
}

//#pragma endregion //Features


void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    // debug_enable=true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;  // Read the user config from EEPROM

    // pointing_device_set_cpi_on_side(true, 1000); //Set cpi on left side to a low value for slower scrolling.
    // pointing_device_set_cpi_on_side(false, 8000); //Set cpi on right side to a reasonable value for mousing.
    // transaction_register_rpc(RPC_ID_USER_SWAP_SYNC, user_sync_swap_hands);

    refresh_layer_info();
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_SPIRAL);
#endif
#ifdef RGBLIGHT_ENABLE
    // rgblight_sethsv(HSV_BLUE);
    // rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
    /* The above code is written in the C programming language. It appears to be using a function
    called `rgblight_sethsv` to set the color of an RGB light to blue. However, without more context
    or the definition of the `rgblight_sethsv` function, it is difficult to determine the exact
    functionality of the code. */
    // rgblight_sethsv_at(85,255,128, 8);
    rgblight_set_clipping_range(0,50); //rgblight_set_effect_range(0, uint8_t num_leds)
    // rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
    // rgblight_enable_noeeprom();
    // rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(0,0,0);
    rgblight_layers = my_rgb_layers;

#endif
}
