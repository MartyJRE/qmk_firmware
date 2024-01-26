#include QMK_KEYBOARD_H

enum layer_names {
    _BASE,
    _NUMBERS,
    _SYMBOLS,
    _SYSTEM
};

#undef TRI_LAYER_LOWER_LAYER
#define TRI_LAYER_LOWER_LAYER _NUMBERS
#undef TRI_LAYER_RAISE_LAYER
#define TRI_LAYER_RAISE_LAYER _SYMBOLS
#undef TRI_LAYER_ADJUST_LAYER
#define TRI_LAYER_ADJUST_LAYER _SYSTEM

enum tap_dance_codes {
    TD_W,
    TD_R,
    TD_T,
    TD_Y,
    TD_ESC,
    TD_A,
    TD_S,
    TD_D,
    TD_F,
    TD_G,
    TD_H,
    TD_J,
    TD_K,
    TD_Z,
    TD_X,
    TD_C,
    TD_V,
    TD_B,
    TD_B_2,
    TD_N,
    TD_M,
    TD_T_C,
    TD_E_S,
    TD_SLSH
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(TD_W):
        case TD(TD_R):
        case TD(TD_T):
        case TD(TD_Y):
        case TD(TD_ESC):
        case TD(TD_A):
        case TD(TD_S):
        case TD(TD_D):
        case TD(TD_F):
        case TD(TD_G):
        case TD(TD_H):
        case TD(TD_J):
        case TD(TD_K):
        case TD(TD_Z):
        case TD(TD_X):
        case TD(TD_C):
        case TD(TD_V):
        case TD(TD_B):
        case TD(TD_N):
        case TD(TD_M):
        case TD(TD_T_C):
        case TD(TD_E_S):
        case TD(TD_SLSH):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
    }
    return true;
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

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(TD_T_C):
        case TD(TD_E_S):
            // Immediately select the hold action when another key is tapped.
            return true;
        default:
            // Do not select the hold action when another key is tapped.
            return false;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

tap_dance_action_t tap_dance_actions[] = {
    // W/Ctrl W
    [TD_W] = ACTION_TAP_DANCE_TAP_HOLD(KC_W, LCTL(KC_W)),
    // R/Ctrl R
    [TD_R] = ACTION_TAP_DANCE_TAP_HOLD(KC_R, LCTL(KC_R)),
    // T/Ctrl T
    [TD_T] = ACTION_TAP_DANCE_TAP_HOLD(KC_T, LCTL(KC_T)),
    // Y/Meh
    [TD_Y] = ACTION_TAP_DANCE_TAP_HOLD(KC_Y, KC_MEH),
    // Esc/Ctrl
    [TD_ESC] = ACTION_TAP_DANCE_TAP_HOLD(KC_ESC, KC_LCTL),
    // A/Ctrl A
    [TD_A] = ACTION_TAP_DANCE_TAP_HOLD(KC_A, LCTL(KC_A)),
    // S/Ctrl S
    [TD_S] = ACTION_TAP_DANCE_TAP_HOLD(KC_S, LCTL(KC_S)),
    // Z/Ctrl Z
    [TD_Z] = ACTION_TAP_DANCE_TAP_HOLD(KC_Z, LCTL(KC_Z)),
    // X/Ctrl X
    [TD_X] = ACTION_TAP_DANCE_TAP_HOLD(KC_X, LCTL(KC_X)),
    // C/Ctrl C
    [TD_C] = ACTION_TAP_DANCE_TAP_HOLD(KC_C, LCTL(KC_C)),
    // V/Ctrl V
    [TD_V] = ACTION_TAP_DANCE_TAP_HOLD(KC_V, LCTL(KC_V)),
    // B/Ctrl B
    [TD_B] = ACTION_TAP_DANCE_TAP_HOLD(KC_B, LCTL(KC_B)),
    // D/[
    [TD_D] = ACTION_TAP_DANCE_TAP_HOLD(KC_D, KC_LBRC),
    // F/(
    [TD_F] = ACTION_TAP_DANCE_TAP_HOLD(KC_F, S(KC_9)),
    // G/{
    [TD_G] = ACTION_TAP_DANCE_TAP_HOLD(KC_G, S(KC_LBRC)),
    // H/}
    [TD_H] = ACTION_TAP_DANCE_TAP_HOLD(KC_H, S(KC_RBRC)),
    // J/)
    [TD_J] = ACTION_TAP_DANCE_TAP_HOLD(KC_J, S(KC_0)),
    // K/]
    [TD_K] = ACTION_TAP_DANCE_TAP_HOLD(KC_K, S(KC_RBRC)),
    // N/Win
    [TD_N] = ACTION_TAP_DANCE_TAP_HOLD(KC_N, KC_LGUI),
    // M/Ctrl M
    [TD_M] = ACTION_TAP_DANCE_TAP_HOLD(KC_M, LCTL(KC_M)),
    // Tab/Ctrl
    [TD_T_C] = ACTION_TAP_DANCE_TAP_HOLD(KC_TAB, KC_LCTL),
    // Enter/Shift
    [TD_E_S] = ACTION_TAP_DANCE_TAP_HOLD(KC_ENTER, KC_RSFT),
    // / / Ctrl /
    [TD_SLSH] = ACTION_TAP_DANCE_TAP_HOLD(KC_SLSH, LCTL(KC_SLSH)),
};

// @see https://github.com/MartyJRE/zmk-config-corne/blob/main/config/corne.keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
        KC_DEL, KC_Q, TD(TD_W), KC_E, TD(TD_R), TD(TD_T), TD(TD_Y), KC_U, KC_I, KC_O, KC_P, KC_BSLS,
        TD(TD_ESC), TD(TD_A), TD(TD_S), TD(TD_D), TD(TD_F), TD(TD_G), TD(TD_H), TD(TD_J), TD(TD_K), KC_L, KC_SCLN, KC_QUOT,
        KC_LSFT, TD(TD_Z), TD(TD_X), TD(TD_C), TD(TD_V), TD(TD_B), TD(TD_N), TD(TD_M), KC_COMM, KC_DOT, TD(TD_SLSH), KC_RSFT,
        TL_LOWR, KC_SPC, KC_BSPC, TD(TD_T_C), TD(TD_E_S), TL_UPPR
    ),
    [_NUMBERS] = LAYOUT_split_3x6_3(
        KC_TRNS, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_GRAVE, KC_EQUAL, KC_TRNS, KC_TRNS, KC_MINUS, KC_UP, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LCTL(LALT(KC_B)), KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_SYMBOLS] = LAYOUT_split_3x6_3(
        KC_TRNS, S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0), KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LGUI, KC_TRNS, KC_TRNS, KC_MEH, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_SYSTEM] = LAYOUT_split_3x6_3(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LALT, LALT(LCTL(KC_LEFT)), LALT(LCTL(KC_RIGHT)), KC_TAB, KC_TRNS, LALT(KC_ENTER), LALT(S(KC_ENTER)), KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, QK_CAPS_WORD_TOGGLE, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};
