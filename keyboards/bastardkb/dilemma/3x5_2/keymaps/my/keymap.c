/**
 * Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
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
#include QMK_KEYBOARD_H
#include "keymap_german.h"

#ifdef DILEMMA_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // DILEMMA_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum dilemma_keymap_layers {
    LAYER_BASE = 0,
    LAYER_GERMAN,
    LAYER_FUNCTION,
    LAYER_NAVIGATION,
    LAYER_POINTER,
    LAYER_NUMERAL,
    LAYER_SYMBOLS,
    LAYER_MEDIA,
    LAYER_SYSTEM,
    LAYER_GAME,
    LAYER_BROWSE
};

enum tap_dances {
    TD_ESC_CAPS = 0,
    TD_O_OE,
    TD_U_UE,
    TD_I_AE,
    TD_DOT_SS,
    TD_F_GUIF,
    TD_PANIC
};

// Automatically enable sniping-mode on the pointer layer.
#define DILEMMA_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef DILEMMA_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef DILEMMA_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define DILEMMA_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // DILEMMA_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef DILEMMA_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define DILEMMA_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // DILEMMA_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // DILEMMA_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define SPC_NAV LT(LAYER_NAVIGATION, KC_SPC)
#define ENT_SYM LT(LAYER_SYMBOLS, KC_ENT)
#define BSP_NUM LT(LAYER_NUMERAL, KC_BSPC)
#define GAME_TG TG(LAYER_GAME)
#define BRWS_TG TG(LAYER_BROWSE)
#define ENGL_DF DF(LAYER_BASE)
#define GERL_DF DF(LAYER_GERMAN)
#define TAB_SFT LSFT_T(KC_TAB)
#define _L_PTR(KC) LT(LAYER_POINTER, KC)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off
#define LAYOUT_LAYER_BASE                                                                     \
       KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, \
       KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L, KC_QUOT, \
       KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, \
                               TAB_SFT, SPC_NAV, ENT_SYM, BSP_NUM

#define LAYOUT_LAYER_GERMAN                                                                                            \
       KC_Q,        KC_W,        KC_E,    KC_R,    KC_T,    DE_Y,    TD(TD_U_UE), TD(TD_I_AE), TD(TD_O_OE),   KC_P   , \
       KC_A,        KC_S,        KC_D,    KC_F,    KC_G,    KC_H,    KC_J,        KC_K,        KC_L,          KC_2   , \
       DE_Z,        KC_X,        KC_C,    KC_V,    KC_B,    KC_N,    KC_M,        DE_COMM,     TD(TD_DOT_SS), KC_7   , \
                                       TAB_SFT, SPC_NAV,    ENT_SYM, BSP_NUM

#define _______________DEAD_HALF_ROW_______________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ______________HOME_ROW_GACS_L______________ KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
#define ______________HOME_ROW_GACS_R______________ XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI

#define LAYOUT_LAYER_FUNCTION                                                                 \
    _______________DEAD_HALF_ROW_______________, KC_PSCR,   KC_F7,   KC_F8,   KC_F9,  KC_F12, \
    ______________HOME_ROW_GACS_L______________, KC_CAPS,   KC_F4,   KC_F5,   KC_F6,  KC_F11, \
    _______________DEAD_HALF_ROW_______________, KC_PAUS,   KC_F1,   KC_F2,   KC_F3,  KC_F10, \
                               _______, XXXXXXX, XXXXXXX, _______

#define LAYOUT_LAYER_POINTER                                                                  \
    XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD, S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, \
    ______________HOME_ROW_GACS_L______________, ______________HOME_ROW_GACS_R______________, \
    _______, DRGSCRL, SNIPING, KC_BTN3, XXXXXXX, XXXXXXX, KC_BTN3, SNIPING, DRGSCRL, _______, \
                               KC_BTN2, KC_BTN1, KC_BTN1, KC_BTN2
#define LAYOUT_LAYER_NAVIGATION                                                               \
    _______________DEAD_HALF_ROW_______________, KC_LBRC, KC_LCBR, KC_RCBR, KC_LPRN, KC_RPRN, \
    ______________HOME_ROW_GACS_L______________, KC_LEFT, KC_UP,   KC_DOWN, KC_RGHT, KC_RBRC, \
    _______________DEAD_HALF_ROW_______________, KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  \
                               XXXXXXX, _______, KC_ENT,  KC_BSPC

#define LAYOUT_LAYER_NUMERAL                                                                  \
    KC_LBRC,    KC_7,    KC_8,    KC_9, KC_RBRC, _______________DEAD_HALF_ROW_______________, \
    KC_SCLN,    KC_4,    KC_5,    KC_6,  KC_EQL, ______________HOME_ROW_GACS_R______________, \
     KC_DOT,    KC_1,    KC_2,    KC_3, KC_BSLS, _______________DEAD_HALF_ROW_______________, \
                                  KC_0, KC_MINS, XXXXXXX, _______
#define LAYOUT_LAYER_SYMBOLS                                                                  \
    KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, _______________DEAD_HALF_ROW_______________, \
    KC_COLN,  KC_DLR, KC_PERC, KC_CIRC, KC_PLUS, ______________HOME_ROW_GACS_R______________, \
    KC_TILD, KC_EXLM,   KC_AT, KC_HASH, KC_PIPE, _______________DEAD_HALF_ROW_______________, \
                                KC_GRV, KC_UNDS, _______, XXXXXXX

#define LAYOUT_LAYER_MEDIA                                                                     \
    _______________DEAD_HALF_ROW_______________, GAME_TG, BRWS_TG, XXXXXXX, XXXXXXX,  XXXXXXX, \
    ______________HOME_ROW_GACS_L______________, XXXXXXX, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT,  \
    _______________DEAD_HALF_ROW_______________, _______________DEAD_HALF_ROW_______________,  \
                               _______, _______, KC_MPLY, KC_MUTE

#define LAYOUT_LAYER_SYSTEM                                                                   \
    QK_BOOT, EE_CLR, EC_TOGG, GERL_DF, ENGL_DF, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX , \
    _______________DEAD_HALF_ROW_______________, ______________HOME_ROW_GACS_R______________, \
   KC_BRID, KC_BRIU, XXXXXXX, KC_SLEP, KC_WAKE, _______________DEAD_HALF_ROW_______________,  \
                               KC_WBAK, KC_WFWD, _______, _______

#define LAYOUT_LAYER_GAME                                                           \
    KC_TAB,  KC_Q, KC_W, KC_E, KC_R, KC_BTN1,  KC_BTN2, XXXXXXX, KC_MNXT, KC_VOLU , \
    KC_LSFT, KC_A, KC_S, KC_D, KC_F, XXXXXXX,  KC_UP,   KC_BTN3, KC_MPLY, KC_MUTE , \
    KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_LEFT, KC_DOWN, KC_RGHT, KC_MPRV, KC_VOLD ,  \
                     KC_ESC, KC_SPC, _______, GAME_TG


#define TAB_FWD LCTL(KC_TAB)
#define TAB_BAK LSFT(TAB_FWD)

#define LAYOUT_LAYER_BROWSING                                                                               \
    KC_1,    KC_2,    KC_3,    KC_4,             KC_5,   KC_BTN1, KC_BTN2, XXXXXXX, XXXXXXX, TD(TD_PANIC) , \
    KC_BTN1, KC_BTN2, KC_UP,   TD(TD_F_GUIF), KC_WBAK,   KC_WFWD, XXXXXXX, KC_BTN3, TAB_BAK, TAB_FWD      , \
    XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT,        KC_ESC,   KC_MPLY, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT      , \
                                       KC_TAB, KC_SPC,   KC_ENT, BRWS_TG


#define _HOME_ROW_MOD_GACS(                                            \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
      LGUI_T(L10), LALT_T(L11), LCTL_T(L12), LSFT_T(L13),         L14,  \
             R15,          LSFT_T(R16), RCTL_T(R17), LALT_T(R18), RGUI_T(R19), \
      __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

#define _POINTER_MOD(                                                  \
    L00, L01, L02, L03, L04, R05, R06, R07, R08, R09,                  \
    L10, L11, L12, L13, L14, R15, R16, R17, R18, R19,                  \
    L20, L21, L22, L23, L24, R25, R26, R27, R28, R29,                  \
    ...)                                                               \
             L00,         L01,         L02,         L03,         L04,  \
             R05,         R06,         R07,         R08,         R09,  \
             L10,         L11,         L12,         L13,         L14,  \
             R15,         R16,         R17,         R18,         R19,  \
      _L_PTR(L20),        L21,         L22,         L23,         L24,  \
             R25,         R26,         R27,         R28,  _L_PTR(R29), \
      __VA_ARGS__
#define POINTER_MOD(...) _POINTER_MOD(__VA_ARGS__)

#define LAYOUT_wrapper(...) LAYOUT_split_3x5_2(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(
    POINTER_MOD(HOME_ROW_MOD_GACS(LAYOUT_LAYER_BASE))
  ),
  [LAYER_GERMAN] = LAYOUT_wrapper(
    POINTER_MOD(HOME_ROW_MOD_GACS(LAYOUT_LAYER_GERMAN))
  ),
  [LAYER_FUNCTION] = LAYOUT_wrapper(LAYOUT_LAYER_FUNCTION),
  [LAYER_NAVIGATION] = LAYOUT_wrapper(LAYOUT_LAYER_NAVIGATION),
  [LAYER_NUMERAL] = LAYOUT_wrapper(LAYOUT_LAYER_NUMERAL),
  [LAYER_POINTER] = LAYOUT_wrapper(LAYOUT_LAYER_POINTER),
  [LAYER_SYMBOLS] = LAYOUT_wrapper(LAYOUT_LAYER_SYMBOLS),
  [LAYER_MEDIA] = LAYOUT_wrapper(LAYOUT_LAYER_MEDIA),
  [LAYER_SYSTEM] = LAYOUT_wrapper(LAYOUT_LAYER_SYSTEM),
  [LAYER_GAME] = LAYOUT_wrapper(LAYOUT_LAYER_GAME),
  [LAYER_BROWSE] = LAYOUT_wrapper(POINTER_MOD(LAYOUT_LAYER_BROWSING))
};

const uint16_t PROGMEM esc_combo[] = {SPC_NAV, ENT_SYM, COMBO_END};
const uint16_t PROGMEM fun_combo[] = {TAB_SFT, BSP_NUM, COMBO_END};
const uint16_t PROGMEM medial_combo[] = {TAB_SFT, SPC_NAV, COMBO_END};
const uint16_t PROGMEM systeml_combo[] = {ENT_SYM, BSP_NUM, COMBO_END};

combo_t key_combos[] = {
    COMBO(esc_combo, TD(TD_ESC_CAPS)),
    COMBO(fun_combo, MO(LAYER_FUNCTION)),
    COMBO(medial_combo, MO(LAYER_MEDIA)),
    COMBO(systeml_combo, MO(LAYER_SYSTEM)),
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_O_OE] = ACTION_TAP_DANCE_DOUBLE(KC_O, DE_ODIA),
    [TD_U_UE] = ACTION_TAP_DANCE_DOUBLE(KC_U, DE_UDIA),
    [TD_PANIC] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_T), LGUI(KC_0)),
    [TD_F_GUIF] = ACTION_TAP_DANCE_DOUBLE(KC_F, LGUI(KC_F)),
    [TD_I_AE] = ACTION_TAP_DANCE_DOUBLE(KC_I, DE_ADIA),
    [TD_DOT_SS] = ACTION_TAP_DANCE_DOUBLE(DE_DOT, DE_SS),
    [TD_ESC_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
};

// clang-format on

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(LAYER_NUMERAL, KC_BSPC):
            return QUICK_TAP_TERM + 20;
        default:
            return 0;
    }
}

#ifdef POINTING_DEVICE_ENABLE
#    ifdef DILEMMA_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > DILEMMA_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > DILEMMA_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= DILEMMA_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
    }
}
#    endif // DILEMMA_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef DILEMMA_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    dilemma_set_pointer_sniping_enabled(layer_state_cmp(state, DILEMMA_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // DILEMMA_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE
