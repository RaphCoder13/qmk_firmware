#include "quantum.h"
#include "ascii.h"
#include "unicode_mod.h"

uint8_t unicode_saved_mods;
led_t   unicode_saved_led_state;

void ascii_input_start(void) {
    unicode_saved_led_state = host_keyboard_led_state();

    unicode_saved_mods = get_mods(); // Save current mods
    clear_mods();                    // Unregister mods to start from a clean state
    clear_weak_mods();

    // For increased reliability, use numpad keys for inputting digits
    if (!unicode_saved_led_state.num_lock) {
        tap_code(KC_NUM_LOCK);
    }
    register_code(KC_LEFT_ALT);
    wait_ms(UNICODE_TYPE_DELAY);
}

void ascii_input_finish(void) {
    unregister_code(KC_LEFT_ALT);
    if (!unicode_saved_led_state.num_lock) {
        tap_code(KC_NUM_LOCK);
    }
    set_mods(unicode_saved_mods); // Reregister previously set mods
}

void register_asciimap(const char* str) {
    ascii_input_start();
    int i = 0;
    while (i < 4) {
        char ch = str[i++];
        if (!ch || (ch < '0') || (ch > '9')) break;
        uint16_t code = KC_KP_0;
        if (ch >= '1') code = ch - '1' + KC_KP_1;
        tap_code(code);
    }
    ascii_input_finish();
}

// Function to return a decimal string from an hexadecimal value of 4 bytes

char* hex_to_dec(uint8_t hex) {
    static char dec[5];
    dec[0] = (hex / 1000) + '0';
    dec[1] = ((hex / 100) % 10) + '0';
    dec[2] = ((hex / 10) % 10) + '0';
    dec[3] = (hex % 10) + '0';
    dec[4] = '\0';
    return dec;
}

bool process_ascii_overrides(uint16_t keycode) {

    SEND_ASCII(hex_to_dec(keycode));



    return true;
}
