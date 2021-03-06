 /* Copyright 2015-2017 Jack Humbert
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

#include "niu_mini.h"
#include "action_layer.h"

void activate_lower(int);
void activate_raise(int);

extern keymap_config_t keymap_config;

enum layers {
  _QWERTY,
  _COLEMAK,
  _WS,
  _F,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  WS
};

enum tapdance {
  TD_RSFT = 0,
  TD_LSFT,
  TD_CTL,
  TD_ALT,
  TD_LOWER
};

void dance_rsft_finished(qk_tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
  case 1:
    register_code(KC_RSFT);
    if (!state->pressed) {
      register_code(KC_0);
      unregister_code(KC_0);
    }
    break;
  case 3:
    register_code(KC_RSFT);
  case 2:
    register_code(KC_RBRC);
    unregister_code(KC_RBRC);
  } 
}

void dance_lsft_finished(qk_tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
  case 1:
    register_code(KC_RSFT);
    if (!state->pressed) {
      register_code(KC_9);
      unregister_code(KC_9);
    }
    break;
  case 3:
    register_code(KC_RSFT);
  case 2:
    register_code(KC_LBRC);
    unregister_code(KC_LBRC);
  } 
}

void dance_shift_reset(qk_tap_dance_state_t *state, void *user_data) {
  unregister_code(KC_RSFT);
}

void dance_alt_finished(qk_tap_dance_state_t *state, void *user_data) {
  switch (state->count){
  case 1:
    register_code(KC_LALT);
    break;
  case 3:
    register_code(KC_LALT);
  case 2:
    register_code(KC_LCTL);
    break;
  }
}

void dance_ctl_finished(qk_tap_dance_state_t *state, void *user_data) {
  switch (state->count){
  case 1:
    register_code(KC_LCTL);
    break;
  case 3:
    register_code(KC_LCTL);
  case 2:
    register_code(KC_LALT);
    break;
  }
}

void dance_ctl_alt_reset(qk_tap_dance_state_t *state, void *user_data) {
  unregister_code(KC_LALT);
  unregister_code(KC_LCTL);
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_rsft_finished, dance_shift_reset),
  [TD_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_lsft_finished, dance_shift_reset),
  [TD_ALT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_alt_finished, dance_ctl_alt_reset),
  [TD_CTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_ctl_finished, dance_ctl_alt_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] =
{
 {KC_GRV,  KC_Q,    KC_W, KC_E,  KC_R,       KC_T,   KC_Y,   KC_U,       KC_I,    KC_O,    KC_P,    KC_BSPC},         
 {KC_TAB,  KC_A,    KC_S, KC_D,  KC_F,       KC_G,   KC_H,   KC_J,       KC_K,    KC_L,    KC_SCLN, RALT_T(KC_QUOT)}, 
 {KC_LSFT, KC_Z,    KC_X, KC_C,  KC_V,       KC_B,   KC_N,   KC_M,       KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT},         
 {MO(_F),  _______, WS,   RAISE, TD(TD_ALT), KC_SPC, KC_SPC, TD(TD_CTL), LOWER,   KC_RALT, KC_LGUI, KC_ENT}
},

[_WS] =
{
 {RAISE,   _______, _______, _______, _______, _______,   _______,   _______, _______, _______, _______, _______}, 
 {KC_SPC,  _______, _______, _______, _______, _______,   _______,   _______, _______, _______, _______, _______}, 
 {_______, _______, _______, _______, _______, _______,   _______,   _______, _______, _______, _______, _______}, 
 {_______, _______, _______, _______, _______, S(KC_ENT), S(KC_ENT), _______, _______, _______, _______, _______}
},

[_LOWER] =
{
 {KC_ESC,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,    KC_ASTR,    KC_LPRN, KC_RPRN, KC_DEL},  
 {KC_ENT,  _______, _______, _______, _______, KC_LBRC, KC_RBRC, KC_MINS,    KC_EQL,     KC_LBRC, KC_RBRC, KC_BSLS},
 {_______, KC_HOME, KC_PGUP, KC_PGDN, KC_END,  KC_LCBR, KC_RCBR, S(KC_NUHS), S(KC_NUBS), _______, _______, _______}, 
 {_______, _______, _______, _______, _______, _______, _______, _______,    _______,    KC_VOLD, KC_VOLU, KC_MPLY}
},

[_RAISE] =
{
 {KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC}, 
 {KC_ENT,  _______, _______, _______, KC_ENT,   KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE}, 
 {_______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______, _______, KC_NUHS, KC_NUBS, _______, _______, _______},
 {_______, _______, _______, _______, _______,  _______, _______,  _______, _______, KC_VOLD, KC_VOLU, KC_MPLY}
},

[_F] =
{
 {_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______, _______, _______, _______, _______, _______, _______}, 
 {_______, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______, _______, KC_LCTL, KC_LSFT, KC_LALT, _______, _______}, 
 {_______, KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_ESC,  KC_SPC,  KC_ENT,  _______, _______, _______}, 
 {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
},

[_COLEMAK] =
{
 {_______, KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, _______},
 {_______, KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,    KC_O,    _______},
 {_______, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, _______},
 {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
},

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|      |      |      |      |      |      |      |      |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] =
{
 {RGB_MODE_PLAIN,   _______, DEBUG,   _______, _______, _______, _______, TERM_ON, TERM_OFF, _______, _______, RESET },       
 {RGB_MODE_FORWARD, _______, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  _______,  _______, _______, TG(_COLEMAK)}, 
 {RGB_MODE_REVERSE, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______,  _______, _______, QWERTY},       
 {_______,          RGB_SAD, RGB_HUD, _______, RGB_VAD, RGB_TOG, RGB_TOG, RGB_VAI, _______,  RGB_HUI, RGB_SAI, _______}
}

};

void matrix_init_user() {
  set_single_persistent_default_layer(_QWERTY);
  layer_on(_COLEMAK);
}

void activate_lower(int activate) {
  if (activate) {
    layer_on(_LOWER);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
  } else {
    layer_off(_LOWER);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
  }
}

void activate_raise(int activate) {
  if (activate) {
    layer_on(_RAISE);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
  } else {
    layer_off(_RAISE);
    update_tri_layer(_LOWER, _RAISE, _ADJUST);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case QWERTY:
    if (record->event.pressed) {
      print("mode just switched to qwerty and this is a huge string\n");
      set_single_persistent_default_layer(_QWERTY);
    }
    return false;
    break;
  case LOWER:
    activate_lower(record->event.pressed);
    return false;
    break;
  case RAISE:
    activate_raise(record->event.pressed);
    return false;
    break;
  case WS:
    if (record->event.pressed) {
      register_code(KC_LGUI);
      layer_on(_WS);
    } else {
      layer_off(_WS);
      unregister_code(KC_LGUI);
    }
    return false;
  }
  return true;
}
