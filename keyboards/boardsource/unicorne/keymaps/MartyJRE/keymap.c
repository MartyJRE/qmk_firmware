#include QMK_KEYBOARD_H

enum {
    _BASE,
    _NUMBERS,
    _SYMBOLS,
    _SYSTEM
};

enum {
    TD_Y,
    TD_ESC,
    TD_D,
    TD_F,
    TD_G,
    TD_H,
    TD_J,
    TD_K,
    TD_N,
    TD_T_C,
    TD_E_S
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(CT_CLN):  // list all tap dance keycodes with tap-hold configurations
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

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

tap_dance_action_t tap_dance_actions[] = {
    // Y/Meh
    [TD_Y] = ACTION_TAP_DANCE_DOUBLE(KC_Y, KC_MEH),
    // Esc/Ctrl
    [TD_ESC] = ACTION_TAP_DANCE_TAP_HOLD(KC_ESC, KC_LCTL),
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
    [TD_K] = ACTION_TAP_DANCE_TAP_HOLD(KC_K, S(KC_RBRC)),k
    // N/Win
    [TD_N] = ACTION_TAP_DANCE_TAP_HOLD(KC_N, KC_LGUI),
    // Tab/Ctrl
    [TD_T_C] = ACTION_TAP_DANCE_TAP_HOLD(KC_TAB, KC_LCTL),
    // Enter/Shift
    [TD_E_S] = ACTION_TAP_DANCE_TAP_HOLD(KC_ENT, KC_RSFT)
};

// @see https://github.com/MartyJRE/zmk-config-corne/blob/main/config/corne.keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
        KC_DEL, KC_Q, LCTL_T(KC_W), KC_E, LCTL_T(R), LCTL_T(KC_T), TD(TD_Y), KC_U, KC_I, KC_O, KC_P, KC_BSLS,
        TD(TD_ESC), LCTL_T(KC_A), LCTL_T(KC_S), TD(TD_D), TD(TD_F), TD(TD_G), TD(TD_H), TD(TD_J), TD(TD_K), KC_L, KC_SCLN, KC_QUOT,
        KC_LSFT, LCTL_T(KC_Z), LCTL_T(KC_X), LCTL_T(KC_C), LCTL_T(KC_V), LCTL_T(KC_B), TD(TD_N), LCTL_T(KC_M), KC_COMM, KC_DOT, LCTL_T(KC_SLSH), KC_RSFT,
        MO(_NUMBERS), KC_SPC, KC_BSPC, TD(TD_T_C), TD(TD_E_S), MO(_SYMBOLS)
    ),
    [_NUMBERS] = LAYOUT_split_3x6_3(
        _______, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, _______,
        _______, _______, _______, KC_GRAVE, KC_EQUAL, _______, _______, KC_MINUS, KC_UP, _______, _______, _______,
        _______, _______, _______, _______, _______, LCA_T(KC_B), _______, KC_LEFT, KC_DOWN, KC_RIGHT, _______, _______,
        _______, _______, _______, _______, _______, _______
    ),
    [_SYMBOLS] = LAYOUT_split_3x6_3(
        _______, S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0), _______,
        _______, _______, _______, _______, KC_LGUI, _______, _______, KC_MEH, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______
    ),
    [_SYSTEM] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, KC_RALT, LCA_T(KC_LEFT), LCA_T(KC_RIGHT), KC_TAB, _______, LALT_T(KC_ENTER), LSA_T(KC_ENTER), _______,
        _______, _______, _______, QK_CAPS_WORD_TOGGLE, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______
    )
};
