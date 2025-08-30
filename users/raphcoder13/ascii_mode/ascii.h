#pragma once

#ifndef UNICODE_TYPE_DELAY
#    define UNICODE_TYPE_DELAY 10
#endif
#define ASCII_SAFE_RANGE  32600

#define SEND_ASCII(X) register_asciimap(PSTR(X))

bool process_ascii_overrides(uint16_t keycode);
