#include QMK_KEYBOARD_H
#include "version.h"
#include "print.h"
#include "transactions.h"
#include "sendstring_french.h"
#include "features/layer_lock.h"
#include "features/select_word.h"
#ifdef ACHORDION_ENABLE
#include <features/achordion.h>
#endif
#include <features/custom_shift_keys.h>
#include <features/casemodes.h>
#include <action_code.h>

#include <enumlayers.h>
#include <g/keymap_combo.h>


#ifdef PS2_MOUSE_ENABLE
#include "ps2_mouse.h"
#endif
#include <math.h>
#include <common_keymap_defs_colemak-fr.h>

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)
static uint32_t layer_timer;
static void refresh_layer_info(void);
static void check_layer_info_timeout(void);
bool is_layer_info_timeout = false;
bool is_recording_macro = false;

#define TRACKPOINT_RST_PIN GP3


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

uint8_t mod_state, weak_mod_state;

bool set_scrolling = false;

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
// Pour Media Layer and Base
void mb_finished(tap_dance_state_t *state, void *user_data);
void mb_reset(tap_dance_state_t *state, void *user_data);
void osmShift_finished(tap_dance_state_t *state, void *user_data);
void osmShift_reset(tap_dance_state_t *state, void *user_data);
// Pour Swap Hands Tap Dance
void swh_finished(tap_dance_state_t *state, void *user_data);
void swh_reset(tap_dance_state_t *state, void *user_data);
// Pour Game/Apps Tap Dance
void g_apps_finished(tap_dance_state_t *state, void *user_data);
void g_apps_reset(tap_dance_state_t *state, void *user_data);

#ifdef RGBLIGHT_ENABLE


const uint8_t RGBLED_BREATHING_INTERVALS[] PROGMEM = {1, 3, 1, 3};
const uint8_t RGBLED_RAINBOW_SWIRL_INTERVALS[] PROGMEM = {5, 5, 5} ;


// const uint8_t _mycolors[13][3] = {{RGB_WHITE},   {RGB_CHARTREUSE}, {RGB_GOLD}, {RGB_GOLDENROD}, {RGB_SPRINGGREEN}, {RGB_BLUE},   {RGB_YELLOW},
//                                   {RGB_CORAL},  {RGB_MAGENTA}, {RGB_ORANGE}, {RGB_CYAN},{RGB_GREEN}, {RGB_RED},

// };
const uint8_t _mycolors[13][3] = {{HSV_WHITE},   {HSV_CHARTREUSE}, {HSV_GOLD}, {HSV_GOLDENROD}, {HSV_SPRINGGREEN}, {HSV_BLUE},   {HSV_PURPLE},
                                  {HSV_RED},  {HSV_MAGENTA}, {HSV_ORANGE}, {HSV_CYAN},{HSV_GREEN}, {HSV_CORAL},

};
#endif



const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
	{{0,4},{1,4},{2,4},{3,4},{4,4},{5,4}},
	{{0,5},{1,5},{2,5},{3,5},{4,5},{5,5}},
	{{0,6},{1,6},{2,6},{3,6},{4,6},{5,6}},
	{{0,7},{1,7},{2,7},{3,7},{4,7},{5,7}},
	{{0,0},{1,0},{2,0},{3,0},{4,0},{5,0}},
	{{0,1},{1,1},{2,1},{3,1},{4,1},{5,1}},
	{{0,2},{1,2},{2,2},{3,2},{4,2},{5,2}},
	{{0,3},{1,3},{2,3},{3,3},{4,3},{5,3}}
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[COLEMAK_FR_1DK] = LAYOUT_wrapper(
        	________COLEMAK_FR_1DK_L1________,                                        ________COLEMAK_FR_1DK_R1________,
        TD(TD_OSM_LSFT),	________COLEMAK_FR_1DK_L2________,                                        ________COLEMAK_FR_1DK_R2________,   FR_SLSH,
        TD(TD_SWAP_HANDS),	________COLEMAK_FR_1DK_L3________,                                        ________COLEMAK_FR_1DK_R3________,	TD(TD_SWAP_HANDS),
                KC_NO,LT(APPS_LAYER, KC_ESC), LT(EXCEL_LAYER, KC_SPC), LT(NAV, KC_TAB),MO(MOUSE_LAYER),          LAYER_LOCK, LT(NUMBERS, KC_ENT), LT(SYMB, KC_BSPC), LT(FUN, KC_DEL),KC_NO),

    [GAME]   = LAYOUT_wrapper(
             ____________QWERTY_L1____________                , ____________QWERTY_R1____________                 ,
            KC_TRNS,____________QWERTY_L2____________                , ____________QWERTY_R2____________                 , KC_QUOT,
            KC_LCTL,____________QWERTY_L3____________                , ____________QWERTY_R3____________                 , KC_RCTL,
		    KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),

    [SYMB]   = LAYOUT_wrapper(
            ____________SYMBOL_L1____________                ,    ____________SYMBOL_R1____________                ,
            KC_CAPS, ____________SYMBOL_L2____________                ,    ____________SYMBOL_R2____________                , KC_NO,
            KC_TRNS, ____________SYMBOL_L3____________                ,    ____________SYMBOL_R3____________                , KC_TRNS,
                KC_TRNS,TG(GAME), QK_LEAD, KC_TAB,KC_TRNS,                         KC_TRNS, KC_ENT,	KC_BSPC,	KC_DEL,KC_TRNS),


    [NUMBERS] = LAYOUT_wrapper(
             ____________NUMBS_L1____________                 , ____________NUMBS_R1____________                 ,
            FR_LPRN,____________NUMBS_L2____________                 , ____________NUMBS_R2____________                 , KC_NO,
            KC_TRNS,____________NUMBS_L3____________                 , ____________NUMBS_R3____________                 , KC_TRNS,
		    KC_TRNS,	FR_DOT,	KC_SPC, KC_TAB,	KC_TRNS,							KC_TRNS,KC_TRNS,	KC_BSPC,	KC_DEL,	KC_TRNS),


    [NAV]          = LAYOUT_wrapper(
              ____________NAV_L1____________                   , ____________NAV_R1____________                  ,
            KC_TRNS,____________NAV_L2____________                   , ____________NAV_R2____________                  , TP_RESET,
            KC_TRNS,____________NAV_L3____________                   , ____________NAV_R3____________                  , KC_TRNS,
		    KC_TRNS,	KC_TRNS, KC_TRNS, KC_TRNS,	KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_TRNS,KC_TRNS,	C(KC_DOWN)),
    [FUN]          = LAYOUT_wrapper(
            ____________FUN_L1____________                   , ____________FUN_R1____________                  ,
            KC_NO,   ____________FUN_L2____________                   , ____________FUN_R2____________                  ,  KC_NO,
            KC_TRNS, ____________FUN_L3____________                   , ____________FUN_R3____________                  , KC_TRNS,
		    KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),
   [APPS_LAYER]   = LAYOUT_wrapper(
            ____________APPS_L1___________                   , ____________APPS_R1___________                  ,
            KC_NO,   ____________APPS_L2___________                   , ____________APPS_R2___________                  , KC_NO,
            KC_TRNS, ____________APPS_L3___________                   , ____________APPS_R3___________                  , KC_TRNS,
		    KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),

    [ACCENT_COLEMAK_LAYER] = LAYOUT_wrapper(
        	___________ACCENT_NEW_COLEMAK_L1___________                  ,___________ACCENT_NEW_COLEMAK_R1___________                  ,
        KC_TRNS,___________ACCENT_NEW_COLEMAK_L2___________                  ,___________ACCENT_NEW_COLEMAK_R2___________                  ,	KC_NO,
        KC_TRNS,___________ACCENT_NEW_COLEMAK_L3___________                  ,___________ACCENT_NEW_COLEMAK_R3___________                  ,	KC_TRNS,
		    KC_TRNS,	SAGR(KC_SPC),	ALGR(KC_G),	KC_TRNS,	KC_TRNS,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),


    [MOUSE_LAYER] = LAYOUT_wrapper(
             ___________MOUSE_L1___________           , ___________MOUSE_R1___________                ,
            KC_NO,   ___________MOUSE_L2___________           , ___________MOUSE_R2___________                ,  KC_NO,
            KC_TRNS, ___________MOUSE_L3___________           , ___________MOUSE_R3___________                , KC_TRNS,
		// KC_TRNS,	FR_W,	KC_RALT,	KC_NO ,	PM_MO(PM_CARET), PM_MO(PM_DRAG),				    KC_NO,	KC_NO,	KC_NO,	KC_RALT,	KC_NO,	KC_TRNS,
		    KC_TRNS,	KC_MS_BTN2,	    KC_MS_BTN1,	KC_MS_BTN3,KC_MS_BTN3,							KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS),


    [EXCEL_LAYER] = LAYOUT_wrapper(
            ___________EXCEL_L1___________                   , ___________EXCEL_R1___________                  ,
            KC_NO,  ___________EXCEL_L2___________                   , ___________EXCEL_R2___________                  , KC_NUM,
            KC_TRNS,___________EXCEL_L3___________                   , ___________EXCEL_R3___________                  , KC_TRNS,
		    KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,							LT( NUMBERS,KC_PENT),	KC_TRNS,	KC_TRNS,	KC_P0,	KC_PDOT),

};

// Gestion de la souris
// Activation/Désactivation de la souris
// Ecriture de la variable en EEPROM
// Développement
typedef union {
    uint32_t raw;
    struct {
        bool mouse_activated;
    };
} user_config_t;

user_config_t user_config;
// bool mouse_active = true;
// bool mouse_mode_remote = true;

#ifdef RGBLIGHT_ENABLE
    static uint32_t key_timer;           // timer for last keyboard activity, use 32bit value and function to make longer idle time possible
    static void refresh_rgb(void);       // refreshes the activity timer and RGB, invoke whenever any activity happens
    static void check_rgb_timeout(void); // checks if enough time has passed for RGB to timeout
    bool is_rgb_timeout = false;         // store if RGB has timed out or not in a boolean

    // void refresh_rgb(void) {
    //     key_timer = timer_read32(); // store time of last refresh
    //     if (is_rgb_timeout)
    //     {
    //         print("Waking up RGB\n");
    //         is_rgb_timeout = false;
    //         rgblight_wakeup();
    //     }
    // }
    // void check_rgb_timeout(void) {
    //     if (!is_rgb_timeout && timer_elapsed32(key_timer) > RGBLIGHT_TIMEOUT) // check if RGB has already timeout and if enough time has passed
    //     {
    //         print("Suspending RGB\n");
    //         rgblight_suspend();
    //         is_rgb_timeout = true;
    //     }
    // }
#endif


void trackpoint_reset(void) {
    uprintf("Resetting Trackpoint\n");
    setPinOutput(TRACKPOINT_RST_PIN);
    writePinLow(TRACKPOINT_RST_PIN);
    wait_ms(500);
    setPinInputHigh(TRACKPOINT_RST_PIN);
    wait_ms(100);
}



void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
//   debug_enable=true;
//   debug_matrix=true;
  //debug_keyboard=true;
//   debug_mouse=true;  // Read the user config from EEPROM

    refresh_layer_info();
#ifdef PS2_MOUSE_ENABLE
    user_config.raw = eeconfig_read_user();
    // Set mouse active or not
    if (user_config.mouse_activated) {
        ps2_mouse_enable_data_reporting();
        ps2_mouse_enable_task();
    } else {
        ps2_mouse_disable_data_reporting();
        ps2_mouse_disable_task();
    }
    trackpoint_reset();
    // if(debug_mouse)
        uprintf("Init Mouse is %s\n", user_config.mouse_activated  ? "active" : "inactive");

#endif
#ifdef RGBLIGHT_ENABLE
    rgblight_sethsv_noeeprom(HSV_CORAL);

    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    // rgblight_sethsv(HSV_BLUE);
    // rgblight_mode(RGBLIGHT_MODE_BREATHING);
#endif
}




void pointing_device_init_user(void) {
	// set_auto_mouse_layer(MOUSE_LAYER); // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of <mouse_layer>
	// set_auto_mouse_enable(true);         // always required before the auto mouse feature will work
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {

#ifdef  PS2_MOUSE_ENABLE
	if (set_scrolling) {
		mouse_report.h = mouse_report.x/PS2_MOUSE_SCROLL_DIVISOR_H;
		mouse_report.v = mouse_report.y/PS2_MOUSE_SCROLL_DIVISOR_V;
		mouse_report.x = 0;
		mouse_report.y = 0;
	}
#endif
	return mouse_report;
}





#if defined MH_AUTO_BUTTONS && defined PS2_MOUSE_ENABLE && defined MOUSEKEY_ENABLE


/* // Define variables to keep track of the last few mouse movements
#define MAX_VELOCITY_COUNT 20
static int velocity_x[MAX_VELOCITY_COUNT];
static int velocity_y[MAX_VELOCITY_COUNT];
static int velocity_index = 0; */

// Variables for simulating momentum
const float MOMENTUM_DECAY = 0.8f;
float deltaX = 0;
float deltaY = 0;

    void ps2_mouse_process_user(report_mouse_t *mouse_report) {

        deltaX += mouse_report->x;
        deltaY += mouse_report->y;

        deltaX *= MOMENTUM_DECAY;
        deltaY *= MOMENTUM_DECAY;

        deltaX = (deltaX < -100 ? -100 : (deltaX > 100 ? 100 : deltaX));

        deltaY = (deltaY < -100 ? -100 : (deltaY > 100 ? 100 : deltaY));
        // uprintf("Momentum : src: %d, %d - target: %ld, %ld \n",  mouse_report->x, mouse_report->y, (long)deltaX, (long)deltaY);

        mouse_report->x = (int8_t) deltaX;
        mouse_report->y = (int8_t) deltaY;

    }

	static uint16_t mh_auto_buttons_timer;
	extern int tp_buttons; // mousekey button state set in action.c and used in ps2_mouse.c

	void ps2_mouse_moved_user(report_mouse_t *mouse_report) {
		if (mh_auto_buttons_timer) {
			mh_auto_buttons_timer = timer_read();
		} else {
			if (!tp_buttons) {
				layer_on(MOUSE_LAYER);
				mh_auto_buttons_timer = timer_read();
	#    if defined CONSOLE_ENABLE
				print("mh_auto_buttons: on\n");
	#    endif
			}
		}

        if (set_scrolling) {
            mouse_report->h = mouse_report->x/PS2_MOUSE_SCROLL_DIVISOR_H;
            mouse_report->v = mouse_report->y/PS2_MOUSE_SCROLL_DIVISOR_V;
            mouse_report->x = 0;
            mouse_report->y = 0;
        }
	}


	void matrix_scan_user(void) {
#ifdef ACHORDION_ENABLE
		achordion_task();
#endif

		layer_lock_task();
		    xcase_task();

		if (mh_auto_buttons_timer && (timer_elapsed(mh_auto_buttons_timer) > MH_AUTO_BUTTONS_TIMEOUT)) {
			if (!tp_buttons) {
				layer_off(MOUSE_LAYER);
				mh_auto_buttons_timer = 0;
	#    if defined CONSOLE_ENABLE
				print("mh_auto_buttons: off\n");
	#    endif
			}
		}
	}

#else

	void matrix_scan_user(void) {
#ifdef ACHORDION_ENABLE
		achordion_task();
#endif
		layer_lock_task();
		xcase_task();
	}

#endif

// void alternate_shift(keyrecord_t *record, uint16_t keycode, uint16_t shifted_keycode)
// {
// 	if (record->event.pressed) {
// 		if (mod_state & MOD_MASK_SHIFT) {
// 			register_code16(shifted_keycode);
// 		} else
// 			register_code16(keycode);
// 	} else {
// 		if (mod_state & MOD_MASK_SHIFT) {

// 			unregister_code16(shifted_keycode);
// 		}
// 		else
// 			unregister_code16(keycode);
// 	}
// }

// void alternate_shifted_shift(keyrecord_t *record, uint16_t keycode, uint16_t shifted_keycode)
// {
// 	if (record->event.pressed) {
// 		if (mod_state & MOD_MASK_SHIFT) {
// 			del_mods(MOD_MASK_SHIFT);
// 			register_code16(shifted_keycode);
// 			set_mods(mod_state);
// 		} else
// 			register_code16(keycode);
// 	} else {
// 		unregister_code16(shifted_keycode);
// 		unregister_code16(keycode);
// 	}
// }
// // Send current layer on Swap Hands

// bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case FR_COMM:
//         case KC_DOT:
//         case FR_SCLN:
//         case FR_SLSH:
//         case FR_MINS:
//             return true;
//     }
//     return false;
// }

// bool get_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
// #ifndef NO_AUTO_SHIFT_ALPHA
//         case KC_A ... KC_Z:
// #endif
// #ifndef NO_AUTO_SHIFT_NUMERIC
//         case KC_1 ... KC_0:
// #endif
// #ifndef NO_AUTO_SHIFT_SPECIAL
//         case AUTO_SHIFT_SPECIAL:
// #    ifndef NO_AUTO_SHIFT_TAB
//         case KC_TAB:
// #    endif
// #    ifndef NO_AUTO_SHIFT_SYMBOLS
//         case AUTO_SHIFT_SYMBOLS:
// #    endif
// #endif
// #ifdef AUTO_SHIFT_ENTER
//         case KC_ENT:
// #endif
//             return true;
//     }
//     return get_custom_auto_shifted_key(keycode, record);
// }

// void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
//     switch (keycode) {
//         case FR_MINS:
//             register_code16((!shifted) ? FR_MINS : FR_UNDS);
//             break;
//         case FR_QUOT:
//             register_code16((!shifted) ? FR_QUOT : FR_DQUO);
//             break;
//         default:
//             if (shifted) {
//                 add_weak_mods(MOD_BIT(KC_LSFT));
//             }
//             register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
//     }
// }
// void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
//     switch (keycode) {
//         case FR_MINS:
//             unregister_code16((!shifted) ? FR_MINS : FR_UNDS);
//             break;
//         case FR_QUOT:
//             unregister_code16((!shifted) ? FR_QUOT : FR_DQUO);
//             break;
//         default:
//             unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
//     }
// }

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    return 550;
}

bool achordion_eager_mod(uint8_t mod) {
    switch (mod) {
        case MOD_LSFT:
        case MOD_RSFT:
        // case MOD_LCTL:
        // case MOD_RCTL:
            return true; // Eagerly apply Shift and Ctrl mods.

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

// bool caps_word_press_user(uint16_t keycode) {
//     switch (keycode) {
//         // Keycodes that continue Caps Word, with shift applied.
//         case KC_A ... KC_Z:

//         // case KC_MINS:
//         case FR_UNDS:
//         case KC_SEMICOLON:
//             add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
//             return true;

//         // Keycodes that continue Caps Word, without shifting.
//         // case KC_1 ... KC_0:  // A voir, interressant pour _
//         case KC_KP_1 ... KC_KP_0:
//         case KC_BSPC:
//         case KC_DEL:
//         case KC_UNDS:
//         case FR_MINS:
//         case KC_UP:
//         case KC_LEFT:
//         case KC_DOWN:
//         case KC_RIGHT:
//             return true;

//         default:
//             return false; // Deactivate Caps Word.
//     }
// }
#ifdef RGBLIGHT_ENABLE
bool dynamic_macro_record_start_user(int8_t direction)
{
    rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
    is_recording_macro = true;
    return true;
}

bool dynamic_macro_record_key_user(int8_t direction, keyrecord_t *record)
{
    if(rgblight_get_mode() != RGBLIGHT_MODE_RAINBOW_SWIRL)        rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL);
    return true;
}

bool dynamic_macro_record_end_user(int8_t direction)
{
    // rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    is_recording_macro = false;
    return true;
}
#endif
void update_rgblight(int highLayer, bool hand_swapped) {
#ifdef RGBLIGHT_ENABLE

    if(is_recording_macro)
        return;

    if (hand_swapped)
    {
        rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
        rgblight_sethsv_noeeprom(_mycolors[highLayer][0], _mycolors[highLayer][1], 64);

    }
    else
    {
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);

        if(highLayer != 0)
            rgblight_sethsv_noeeprom(_mycolors[highLayer][0], _mycolors[highLayer][1], 64);
        else
            rgblight_sethsv_noeeprom(HSV_BLACK);
    }
#endif
}
// Call on every layer change.
// Send KBHLayerN (N is layer index)
//      Add M if Swap Hands is active
// Used by Autohotkey to display current layer info.
void update_layer_info(int highLayer, bool hand_swapped) {
    uprintf("KBHLayer%d%s\n", highLayer, hand_swapped ? "M" : "");
    layer_timer = timer_read32();
    is_layer_info_timeout = false;

    update_rgblight(highLayer, hand_swapped);
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
    // else if (leader_sequence_two_keys(KC_D, KC_D)) {
    //     // Leader, d, d => Ctrl+A, Ctrl+C
    //     SEND_STRING(SS_LCTL("a") SS_LCTL("c"));
    // } else if (leader_sequence_three_keys(KC_D, KC_D, KC_S)) {
    //     // Leader, d, d, s => Types the below string
    //     SEND_STRING("https://start.duckduckgo.com\n");
    // } else if (leader_sequence_two_keys(FR_A, FR_S)) {
    //     // Leader, a, s => GUI+S
    //     uprint("Leader, a, s\n");
    //     tap_code16(LGUI(KC_S));
    // }
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
		//static uint8_t saved_mods   = 0;
	uint16_t       temp_keycode = keycode;

#ifdef ACHORDION_ENABLE
    if (!process_achordion(keycode, record)) { return false; }
#endif
    if (!process_layer_lock(keycode, record, LAYER_LOCK)) { return false; }
    // if (!process_select_word(keycode, record, SELWORD)) { return false; }
    if (!process_custom_shift_keys(keycode, record)) { return false; }
    if (!process_case_modes(keycode, record)) { return false; }
	mod_state = get_mods();
	weak_mod_state = get_weak_mods();


	switch (temp_keycode) {
		// dynamically generate these.

		case SH_OS_TT:
			// One shot tap toggle swap hands with momentary on hold

            // uprintf("Tap count : %d -- toggle %d\n", record->tap.count, ONESHOT_TAP_TOGGLE);

			if (record->event.pressed) {
				if (record->tap.count > ONESHOT_TAP_TOGGLE && !record->tap.interrupted) {
					// Toggle swap hands permanently
					action_t action;
					action.code = ACTION_SWAP_HANDS_ON_OFF();
					//action.swap.code = OP_SH_TOGGLE;

					process_action(record, action);
				} else {
					// One shot swap hands on hold
					set_oneshot_swaphands();
			   }
			} else {
				clear_oneshot_swaphands();
			}
			return false;

			// POUR LA GESTION DE LA SOURIS, ON VERRA COMMENT ON PEUT DÉSACTIVER AU NIVEAU HARDWARE POUR QUAND ELLE N'EST PAS BRANCHÉE

#ifdef PS2_MOUSE_ENABLE
		case MOUSE_TOGGLE:
			if (record->event.pressed) {

                user_config.mouse_activated ^= 1; // Toggles the status
                eeconfig_update_user(user_config.raw); // Writes the new status to EEPROM
				if(user_config.mouse_activated) {
                    ps2_mouse_enable_data_reporting();
					ps2_mouse_enable_task();

				}
				else {
                    ps2_mouse_disable_data_reporting();
					ps2_mouse_disable_task();
				}
				uprintf("Mouse is %s\n", user_config.mouse_activated  ? "active" : "inactive");

			}
			return false;

			break;
#endif


		case K_VSCROLL :

			if(record->event.pressed) {
				set_scrolling   = true;
            }
            else {
                set_scrolling   = false;
            }
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
            // alternate_shift(record, ALGR(FR_D), S(ALGR(FR_D)));
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
       // unregister_mods(MOD_MASK_SHIFT);
                        register_mods(MOD_MASK_SHIFT);
                        SEND_STRING(SS_TAP(X_LBRC));
                        // set_mods(mod_state);
                        SEND_STRING(SS_TAP(X_E));
                        unregister_mods(MOD_MASK_SHIFT);

                    } else {
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
		// 	action = &tap_dance_actions[TD_INDEX(temp_keycode)];
		// 	if (!record->event.pressed && action->state.count && !action->state.finished) {
		// 		tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
		// 		tap_code16(tap_hold->tap);
		// 	}
		// 	break;
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
        soft_reset_keyboard();
        reset_tap_dance(state);
    }
}
tap_dance_action_t tap_dance_actions[] = {
    [TD_RSFT_LAYER0] = ACTION_TAP_DANCE_LAYER_MOVE(KC_RSFT, BASE),
    [TD_TRIPLE_BOOTLOADER] = ACTION_TAP_DANCE_FN(bootloader_reset),
     [TD_TRIPLE_REBOOT] = ACTION_TAP_DANCE_FN(reboot_keyboard),
    //  [TD_MEDIA_BASE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mb_finished, mb_reset),
    //  [TD_GAME_APPS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, g_apps_finished, g_apps_reset),
     [TD_SWAP_HANDS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, swh_finished, swh_reset),
    [TD_OSM_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, osmShift_finished, osmShift_reset),
    [TD_OSM_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, osmShift_finished, osmShift_reset),

};




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
static td_tap_t ql_tap_state = {.is_press_action = true, .state = TD_NONE};

// Functions that control what our tap dance key does
// void mb_finished(tap_dance_state_t *state, void *user_data) {
//     ql_tap_state.state = cur_dance(state);
//     switch (ql_tap_state.state) {
//             /*         case TD_SINGLE_TAP:
//                         tap_code(KC_QUOT);
//                         break; */
//         case TD_SINGLE_HOLD:
//         case TD_DOUBLE_TAP:
//             layer_on(MEDIA);
//             break;
//         case TD_TRIPLE_TAP:
//             layer_move(ERGOL_MOD);
//             break;
//         case TD_QUAD_TAP:
//             layer_move(BASE);
//             break;
//         default:
//             break;
//     }
// }

// void mb_reset(tap_dance_state_t *state, void *user_data) {
// 	// If the key was held down and now is released then switch off the layer
// 	if (ql_tap_state.state == TD_SINGLE_HOLD) {
// 		layer_off(MEDIA);
// 	}
// 	ql_tap_state.state = TD_NONE;
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
//             layer_on(APPS_LAYER);
//             break;
//         case TD_DOUBLE_TAP:
//             layer_on(APPS_LAYER);
//             break;
//         case TD_TRIPLE_TAP:
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


//     // }
//     // ql_tap_state.state = TD_NONE;
// }
void osmShift_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            set_oneshot_mods(MOD_LSFT);
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_LSFT);
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

void oneshot_swap_hands_changed_user(bool swap_hands_state) {
    update_layer_info(get_highest_layer(layer_state), swap_hands);
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

layer_state_t layer_state_set_user(layer_state_t state) {
    update_layer_info(get_highest_layer(state), is_swap_hands_on());

    // uprintf("KBHLayer%u%s\n", get_highest_layer(state), is_swap_hands_on() ? "M" : "");
    // uprintf("KBHLayer%u%s\n", get_highest_layer(state), swap_hands ? "M" : "");

#ifdef RGBLIGHT_ENABLE
    //   rgblight_sethsv_at(_mycolors[get_highest_layer(state)][0],_mycolors[get_highest_layer(state)][1],64, swap_hands ? 0 : 1); // led 0
    //   rgblight_sethsv_at(HSV_BLACK,   swap_hands ? 1 : 0); // led 1

    /*   rgblight_sethsv_at(_mycolors[get_highest_layer(state)], is_swap_hands_on() ? 0 : 1); // led 0
      rgblight_sethsv_at(_mycolors[get_highest_layer(state)+1], is_swap_hands_on() ? 1 : 0); // led 0
    */

    // rgblight_sethsv(_mycolors[get_highest_layer(state)][0], _mycolors[get_highest_layer(state)][1], 255);

    // if (is_swap_hands_on())
    // {
    //     // rgblight_sethsv_master(HSV_BLACK);
    //     // rgblight_sethsv_slave(_mycolors[get_highest_layer(state)]);
    //     rgblight_sethsv_range(HSV_BLACK, 0, (uint8_t)RGBLED_NUM / 2);
    //     rgblight_sethsv_range(_mycolors[get_highest_layer(state)][0], _mycolors[get_highest_layer(state)][1], 64, (uint8_t)RGBLED_NUM / 2, (uint8_t)RGBLED_NUM);

    // }
    // else
    // {
    //     // rgblight_sethsv_master(_mycolors[get_highest_layer(state)]);hhhhhhhhhhhhhh
    //     // rgblight_sethsv_slave(HSV_BLACK);
    //     rgblight_sethsv_range(_mycolors[get_highest_layer(state)][0], _mycolors[get_highest_layer(state)][1], 64, 0, (uint8_t)RGBLED_NUM / 2);
    //     rgblight_sethsv_range(HSV_BLACK, (uint8_t)RGBLED_NUM / 2, (uint8_t)RGBLED_NUM);
    // }

#endif

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



#ifdef RGBLIGHT_ENABLE

void refresh_rgb(void) {
    key_timer = timer_read32(); // store time of last refresh
    if (is_rgb_timeout) {
        print("Waking up RGB\n");
        is_rgb_timeout = false;
        rgblight_wakeup();
        update_rgblight( get_highest_layer(layer_state), is_swap_hands_on());
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


#endif

