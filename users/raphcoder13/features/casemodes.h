/* Copyright 2021 Andrew Rae ajrae.nv@gmail.com @andrewjrae, NumWord by Joshua T., adaption by 2022 @vvhg1
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// #pragma once
// #include "customkeycodes.h"

#include QMK_KEYBOARD_H

// task to call for timeout
void xcase_task(void);

// Check whether caps word is on
bool caps_word_enabled(void);
bool is_caps_word_on(void);
// Check whether x-case is on
bool x_case_enabled(void);
// Enable caps word
void enable_caps_word(void);
// Disable caps word
void disable_caps_word(void);
// Toggle caps word
void toggle_caps_word(void);
// Toggle xcase
void toggle_xcase(void);

uint16_t get_xcase_delimiter(void);

// Check whether num word is on
bool num_word_enabled(void);
// Enable num word
void enable_num_word(void);
// Disable num word
void disable_num_word(void);
// Toggle num word
void toggle_num_word(void);

// enum for the xcase states
enum xcase_state {
    XCASE_OFF = 0, // xcase is off
    XCASE_ON,      // xcase is actively on
    XCASE_WAIT,    // xcase is waiting for the delimiter input
};

// Get xcase state
enum xcase_state get_xcase_state(void);
// Enable xcase and pickup the next keystroke as the delimiter
void enable_xcase(void);
// Enable xcase with the specified delimiter
void enable_xcase_with(uint16_t delimiter);
// Disable xcase
void disable_xcase(void);

// Function to be put in process user
bool process_case_modes(uint16_t keycode, const keyrecord_t *record);
