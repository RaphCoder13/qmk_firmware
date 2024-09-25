// enumlayers.h
// List of layers for use in casemodes

#include "quantum_keycodes.h"
#include QMK_KEYBOARD_H

#define BASE 0 // default layer
#define COLEMAK_FR_1DK 0 // Colemak (FR 1DK)
#define GAME 1 // Game (QWERTY)
#define SYMB 2 // symbols
#define NUMBERS 3 // Numbers (top row)
#define NAV 4 // media keys
#define FUN 5 // Function keys and media keys
#define APPS_LAYER	6 // Apps shortcuts
#define MOUSE_LAYER     7
#define EXCEL_LAYER     8 // Numpad numbers and nav...
#define ACCENT_COLEMAK_LAYER     9 // One shot accents for Colemak


#define _NUM EXCEL_LAYER

// Enum for Tap Dance
enum {
    TD_LSFT_LAYER0,
    TD_RSFT_LAYER0,
    TD_TRIPLE_BOOTLOADER,
    TD_TRIPLE_REBOOT,
    TD_SWAP_HANDS,
    TD_OSM_LSFT,
    TD_OSM_RSFT,
};

enum custom_keycodes {
    VRSN = SAFE_RANGE,
    LAYER_LOCK,
    BACKSLASH_UNDERSCORE,
    BACKSLASH_DEG,
    GREV_APOST,
    CIRC_TILD,
    MINUS_DEG,
    ASTER_DIV,
    RGB_SLD,
    AT_MU,
    AMPR_PIPE,
    PCT_EXLM,
    MINUS_UNDSCORE,
    UNDSCORE_MINUS,
    ST_MACRO_0,
    ST_MACRO_1,
    ST_MACRO_2,
    FR_LSPO,
    FR_RSPC,
    BSP_DEL,
    AGRV_MAJ,
    EACU_MAJ,
    EGRV_MAJ,
    ECRC_MAJ,
    ETREM_MAJ,
    UGRV_MAJ,
    UCRC_MAJ,
    OCRC_MAJ,
    OE_MAJ,
    ACRC_MAJ,
    CCED_MAJ,
    ICRC_MAJ,
    ITREM_MAJ,
    QUOTE_DBLE,
    GUIM_OPEN_CLOSE,
    PARENTH_OPEN_CLOSE,
    SUP_AGRAVE,
    INF_APOST,
    K_VSCROLL,
    K_HSCROLL,
    SELWORD,
    SH_OS_TT,
    MOUSE_TOGGLE,
    MOUSE_MODE_TOGGLE,
    VIM_SYST_CLIPB,
    VIM_LAST_YANK,
    RDB_REPEAT,
    RDB_ALT_REPEAT,
    MAG_ION,
    MAG_LES,
    MAG_OUR,
    MAG_AIT,
    MAG_MENT,
    MAG_TION,
    MAG_RAPHAEL,
    MAG_DEBUG,
    MAG_PHONE,
};

