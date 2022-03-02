/* Copyright 2021 Carlos Martins
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
#include <stdio.h>
#include "./luna.h"
#include "./enums.h"
#ifdef OLED_ENABLE
#include "./render-oled.h"
#ifdef ENABLE_FLUTTERSHY
#include "transactions.h"
#include "./flutter-animate.h"
#endif
#endif

/* status variables */
uint8_t current_wpm = 0;
uint32_t standby_timer = 0;

/*
[_BLANK] = LAYOUT(
  _______, _______, _______, _______, _______, _______,						 _______, _______, _______, _______, _______, _______,
  _______, _______,	_______, _______, _______, _______,						 _______, _______, _______, _______, _______, _______,
  _______, _______,	_______, _______, _______, _______,						 _______, _______, _______, _______, _______, _______,
  _______, _______,	_______, _______, _______, _______, _______,	   _______, _______, _______, _______, _______, _______, _______,
					_______, _______, _______, _______, _______,	   _______, _______, _______, _______, _______
),
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * ,-----------------------------------------.					,-----------------------------------------.
 * | GrEsc|   1  |   2  |   3  |   4  |   5  |					|   6  |   7  |   8  |   9  |   0  | Bspc |
 * |------+------+------+------+------+------|					|------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |					|   Y  |   U  |   I  |   O  |   P  |  #~  |
 * |------+------+------+------+------+------|					|------+------+------+------+------+------|
 * |LShift|   A  |   S  |   D  |   F  |   G  |-------.	,-------|   H  |   J  |   K  |   L  |   ;  |RShift|
 * |------+------+------+------+------+------|	   |	|		|------+------+------+------+------+------|
 * | LCTR |   Z  |   X  |   C  |   V  |   B  |-------|	|-------|   N  |   M  |   ,  |   .  |   /? |  '@  |
 * `-----------------------------------------/	   /	 \	  \-----------------------------------------'
 *			|  \|  | LAlt |  [{  |LOWER | /Space  /		  \Enter \  |RAISE |  ]}  |  -_  |  =+  |
 *			|	   |	  |		 |		|/	   /		   \	  \ |	   |	  |	  |		|
 *			`----------------------------------'			'------''---------------------------'
 */

/*
[_QWERTY] = LAYOUT(
  KC_GRV,   RGB_TOG,   RGB_MOD,	RGB_RMOD,	KC_4,	KC_5,					 KC_6,	KC_7,	KC_8,	KC_9,	KC_0,  KC_MINS,
  KC_ESC,   KC_Q,   KC_W,	KC_E,	KC_R,	KC_T,					 KC_Y,	KC_U,	KC_I,	KC_O,	KC_P,  KC_BSPC,
  KC_TAB,   KC_A,   KC_S,	KC_D,	KC_F,	KC_G,					 KC_H,	KC_J,	KC_K,	KC_L, KC_SCLN,  KC_QUOT,
  KC_LSPO,  KC_Z,   KC_X,	KC_C,	KC_V,	KC_B, KC_MUTE,	 KC_MPLY,KC_N,	KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSPC,
				 KC_LGUI,KC_LALT,KC_LCTRL, KC_VIMOD, KC_SPC,	  KC_ENT,  KC_MEDIA, KC_RCTRL, KC_RALT, KC_RGUI
),
*/
[_QWERTY] = LAYOUT(
		 KC_ESC,  KC_1,   KC_2,	KC_3,	KC_4,	KC_5,					  KC_6,	KC_7,	KC_8,	KC_9,	KC_0,  KC_BSPC,
		 KC_TAB,   KC_Q,   KC_W,	KC_E,	KC_R,	KC_T,					  KC_Y,	KC_U,	KC_I,	KC_O,	KC_P,  KC_NUHS,
		 KC_LSFT,  KC_A,   KC_S,	KC_D,	KC_F,	KC_G,					  KC_H,	KC_J,	KC_K,	KC_L, KC_SCLN,  KC_RSHIFT,
 LCTL_T(KC_NUBS),  KC_Z,   KC_X,	KC_C,	KC_V,	KC_B, C(G(KC_D)),	 KC_SAPP,KC_N,	KC_M, KC_COMM,  KC_DOT, KC_SLSH, RCTL_T(KC_QUOT),
	   				 KC_LGUI,KC_LALT,KC_LBRC, KC_VIMOD, KC_SPC,	  KC_ENT,  KC_MEDIA, KC_RBRC,   KC_MINS, KC_EQUAL
),
/* LOWER
 * ,-----------------------------------------.					,-----------------------------------------.
 * |	  |  F1  |  F2  |  F3  |  F4  |  F5  |					|  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|					|------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   5  |					|   6  |   7  |   8  |   9  |   0  | F12  |
 * |------+------+------+------+------+------|					|------+------+------+------+------+------|
 * | Tab  |   !  |   @  |   #  |   $  |   %  |-------.	,-------|   ^  |   &  |   *  |   (  |   )  |   |  |
 * |------+------+------+------+------+------|  MUTE |	|	   |------+------+------+------+------+------|
 * | Shift|  =   |  -   |  +   |   {  |   }  |-------|	|-------|   [  |   ]  |   ;  |   :  |   \  | Shift|
 * `-----------------------------------------/	   /	 \	  \-----------------------------------------'
 *			| LGUI | LAlt | LCTR |LOWER | /Enter  /	   \Space \  |RAISE | RCTR | RAlt | RGUI |
 *			|	  |	  |	  |	  |/	   /		 \	  \ |	  |	  |	  |	  |
 *			`----------------------------------'		   '------''---------------------------'
 */
[_VIMOD] = LAYOUT(
	 KC_ULCK, _______,  _______, _______, KC_END , _______,						 KC_HOME, _______,  _______, _______, _______, _______,
	  KC_GRV, _______, KC_NXTWD, _______, KC_INS , _______,						 _______, KC_PGUP, KC_LLOCK, _______, _______, _______,
	 _______, _______,	_______, KC_PGDN, _______, _______,						 KC_LEFT, KC_DOWN,  KC_UP  , KC_RGHT, _______, _______,
	 _______, _______,	KC_DEL , _______, _______,KC_PRVWD, _______,	   _______, _______, _______,  _______, _______, _______, _______,
						_______, _______, _______, _______, _______,	   _______, _______, _______,  _______, _______
),
/*
[_VIMOD] = LAYOUT(
  _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,					   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  KC_GRV,	KC_1,	KC_2,	KC_3,	KC_4,	KC_5,					   KC_6,	KC_7,	KC_8,	KC_9,	KC_0,  KC_F12,
  _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,					   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
  _______,  KC_EQL, KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR, _______,	   _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
					   _______, _______, _______, _______, _______,	   _______, _______, _______, _______, _______
),
	*/
/* RAISE
 * ,----------------------------------------.					,-----------------------------------------.
 * |	  |	  |	  |	  |	  |	  |					|	  |	  |	  |	  |	  |	  |
 * |------+------+------+------+------+------|					|------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |	  |RGBTog|					|	  | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|					|------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|	  | Caps |-------.	,-------|	  | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE  |	|	   |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|	  |-------|	|-------|	  | LStr |	  | LEnd |	  | Shift|
 * `-----------------------------------------/	   /	 \	  \-----------------------------------------'
 *			| LGUI | LAlt | LCTR |LOWER | /Enter  /	   \Space \  |RAISE | RCTR | RAlt | RGUI |
 *			|	  |	  |	  |	  |/	   /		 \	  \ |	  |	  |	  |	  |
 *			`----------------------------------'		   '------''---------------------------'
 */
[_MEDIA] = LAYOUT(

  KC_ULCK,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,						   KC_F6,   KC_F7,	KC_F8,   KC_F9,  KC_F10,  KC_F11,
   KC_GRV, _______,	  KC_UP, _______, _______, _______,						 _______, KC_PAUS, KC_LLOCK, _______, KC_PSCR,  KC_F12,
  _______, KC_LEFT,	KC_DOWN, KC_RGHT, _______, _______,						 _______, _______,  KC_CAPS, KC_SCRL, _______, _______,
  _______, _______,	_______, _______, _______, _______, KC_MUTE,	   KC_MPLY, _______, _______,  _______, _______, _______, _______,
					_______, _______, _______, _______, _______,	   _______, _______, _______,  _______, _______
	/*
  KC_GRV , _______ , _______ , _______ , RGB_RMOD , RGB_MOD,						   _______,  _______  , _______,  _______ ,  _______ ,_______,
  _______,  KC_INS,  KC_PSCR,   KC_APP,  XXXXXXX, RGB_TOG,						KC_PGUP, KC_PRVWD,   KC_UP, KC_NXTWD,KC_DLINE, KC_BSPC,
  _______, KC_LALT,  KC_LCTL,  KC_LSFT,  XXXXXXX, KC_CAPS,					   KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL, KC_BSPC,
  _______,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  _______,	   _______,  XXXXXXX, KC_LSTRT, XXXXXXX, KC_LEND,   XXXXXXX, _______,
						 _______, _______, _______, _______, _______,	   _______, _______, _______, _______, _______
		*/
),
/* ADJUST */
  [_ADMIN] = LAYOUT(
  KC_ULCK, _______, _______, _______, _______, _______,						    _______, _______, _______, _______, _______, _______,
  _______, _______,	_______, _______, RGB_MOD, RGB_TOG,						    _______, _______, _______, _______, _______, _______,
  KC_CAPS, KC_PAUS,	KC_SCRL, _______, _______, _______,						    _______, _______, _______, _______, _______, _______,
  _______, _______,	_______, _______, _______, _______, _______,	   _______, KC_NUM , _______, _______, _______, _______, _______,
					_______, _______, _______, _______, _______,	   _______, _______, _______, _______, _______
  )
};

#ifdef OLED_ENABLE

#ifdef ENABLE_FLUTTERSHY
#ifdef ENABLE_FLUTTERSHY_SLAVE
void keyboard_post_init_user(void) {
	transaction_register_rpc(USER_SYNC_A, user_sync_fluttershy_slave_handler);
}
#endif

#ifdef ENABLE_FLUTTERSHY_MASTER
void housekeeping_task_user(void) {
	housekeeping_task_user_fluttershy();
}
#endif
#endif

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
#ifdef LEFT_FIRMWARE
	return OLED_ROTATION_270;
#else
	return OLED_ROTATION_270;
#endif
}

uint8_t last_layer = 100;
bool screenOn = true;
bool oled_task_user(void) {
	uint8_t layer = get_highest_layer(layer_state);

	/* this fixes the screen on and off bug */
	current_wpm = get_current_wpm();

	if (current_wpm > 0) {
		screenOn = true;
#ifdef LEFT_FIRMWARE
		oled_on();
		rgb_matrix_enable();
#endif
		standby_timer = timer_read32();
	} else if (timer_elapsed32(standby_timer) > OLED_TIMEOUT_USER) {
#ifdef LEFT_FIRMWARE
		rgb_matrix_disable();
		oled_off();
#endif
		screenOn = false;
	}
	if (!screenOn)
		return false;

	if (last_layer != layer) {
		last_layer = layer;
		oled_clear();
		oled_set_cursor(0, 0);
	}

	render_oled(layer, current_wpm);

	return false;
}

#endif // OLED_ENABLE

bool llock = false;
bool is_alt_tab_active = false;
bool tick_esc_was_shifted = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		flapwing_fluttershy();
		if (keycode == KC_SPC || keycode == KC_ENT) {
			bobhead_fluttershy();
		}
		screenOn = true;
		if (is_keyboard_master()) {
			rgb_matrix_enable();
			oled_on();
		}
		standby_timer = timer_read32();
	}
	switch (keycode) {
		case KC_QWERTY:
			if (record->event.pressed) {
				set_single_persistent_default_layer(_QWERTY);
			}
			return false;
		case KC_VIMOD:
			if (record->event.pressed) {
				layer_on(_VIMOD);
				update_tri_layer(_VIMOD, _MEDIA, _ADMIN);
			} else {
				if (!llock) {
					layer_off(_VIMOD);
					update_tri_layer(_VIMOD, _MEDIA, _ADMIN);
				}
			}
			return false;
		case KC_MEDIA:
			if (record->event.pressed) {
				layer_on(_MEDIA);
				update_tri_layer(_VIMOD, _MEDIA, _ADMIN);
			} else {
				if (!llock) {
					layer_off(_MEDIA);
					update_tri_layer(_VIMOD, _MEDIA, _ADMIN);
				}
			}
			return false;
		case KC_ADMIN:
			if (record->event.pressed) {
				layer_on(_ADMIN);
			} else {
				layer_off(_ADMIN);
			}
			return false;
		case KC_LLOCK:
			if (record->event.pressed) {
				llock = true;
			}
			return false;
		case KC_ULCK:
			if (record->event.pressed) {
				if (!llock) {
					register_code(KC_GRV);
					return false;
				}
				llock = false;
				layer_off(_VIMOD);
				layer_off(_MEDIA);
				layer_off(_ADMIN);
				update_tri_layer(_VIMOD, _MEDIA, _ADMIN);
			} else {
				unregister_code(KC_GRV);
			}
			return false;
		case KC_PRVWD:
			if (record->event.pressed) {
				if (keymap_config.swap_lctl_lgui) {
					register_mods(mod_config(MOD_LALT));
					register_code(KC_LEFT);
				} else {
					register_mods(mod_config(MOD_LCTL));
					register_code(KC_LEFT);
				}
			} else {
				if (keymap_config.swap_lctl_lgui) {
					unregister_mods(mod_config(MOD_LALT));
					unregister_code(KC_LEFT);
				} else {
					unregister_mods(mod_config(MOD_LCTL));
					unregister_code(KC_LEFT);
				}
			}
			break;
		case KC_NXTWD:
			 if (record->event.pressed) {
				if (keymap_config.swap_lctl_lgui) {
					register_mods(mod_config(MOD_LALT));
					register_code(KC_RIGHT);
				} else {
					register_mods(mod_config(MOD_LCTL));
					register_code(KC_RIGHT);
				}
			} else {
				if (keymap_config.swap_lctl_lgui) {
					unregister_mods(mod_config(MOD_LALT));
					unregister_code(KC_RIGHT);
				} else {
					unregister_mods(mod_config(MOD_LCTL));
					unregister_code(KC_RIGHT);
				}
			}
			break;
		case KC_SAPP:
			if (is_alt_tab_active) {
				is_alt_tab_active = false;
				unregister_code(KC_LALT);
			}
			break;
		/*
		case KC_LSTRT:
			if (record->event.pressed) {
				if (keymap_config.swap_lctl_lgui) {
					 //CMD-arrow on Mac, but we have CTL and GUI swapped
					register_mods(mod_config(MOD_LCTL));
					register_code(KC_LEFT);
				} else {
					register_code(KC_HOME);
				}
			} else {
				if (keymap_config.swap_lctl_lgui) {
					unregister_mods(mod_config(MOD_LCTL));
					unregister_code(KC_LEFT);
				} else {
					unregister_code(KC_HOME);
				}
			}
			break;
		case KC_LEND:
			if (record->event.pressed) {
				if (keymap_config.swap_lctl_lgui) {
					//CMD-arrow on Mac, but we have CTL and GUI swapped
					register_mods(mod_config(MOD_LCTL));
					register_code(KC_RIGHT);
				} else {
					register_code(KC_END);
				}
			} else {
				if (keymap_config.swap_lctl_lgui) {
					unregister_mods(mod_config(MOD_LCTL));
					unregister_code(KC_RIGHT);
				} else {
					unregister_code(KC_END);
				}
			}
			break;
		case KC_DLINE:
			if (record->event.pressed) {
				register_mods(mod_config(MOD_LCTL));
				register_code(KC_BSPC);
			} else {
				unregister_mods(mod_config(MOD_LCTL));
				unregister_code(KC_BSPC);
			}
			break;
		*/
			/* KEYBOARD PET STATUS START */

		case KC_LCTL:
		case KC_RCTL:
			if (record->event.pressed) {
				isSneaking = true;
			} else {
				isSneaking = false;
			}
			break;
		case KC_SPC:
			if (record->event.pressed) {
				isJumping  = true;
				showedJump = false;
			} else {
				isJumping = false;
			}
			break;

			/* KEYBOARD PET STATUS END */
	}
	return true;
}

#ifdef ENCODER_ENABLE

uint16_t alt_tab_timer = 0;
bool encoder_update_user(uint8_t index, bool clockwise) {
	uint8_t layer = get_highest_layer(layer_state);
	if (layer == _MEDIA) {
		uint16_t mapped_code = 0;
		if (index == 0) { /* First encoder */
			if (clockwise) {
				mapped_code = KC_VOLD;
			} else {
				mapped_code = KC_VOLU;
			}
		}  else {
			if (clockwise) {
				mapped_code = KC_MNXT;
			} else {
				mapped_code = KC_MPRV;
			}
		}
		tap_code_delay(mapped_code, MEDIA_KEY_DELAY);
	} else if (layer == _VIMOD) {
		if (index == 0) { /* First encoder */
			if (clockwise) {

			} else {

			}
		}  else {
			if (clockwise) {

			} else {

			}
		}
	} else if (layer == _ADMIN) {
		if (index == 0) { /* First encoder */
			if (clockwise) {

			} else {

			}
		}  else {
			if (clockwise) {

			} else {

			}
		}
	} else {
		if (timer_elapsed(alt_tab_timer) > 100) {
			if (index == 0) { /* First encoder */
				if (clockwise) {
					tap_code16(C(G(KC_LEFT)));
				} else {
					tap_code16(C(G(KC_RGHT)));
				}
			}  else {
				if (!is_alt_tab_active) {
					is_alt_tab_active = true;
					register_code(KC_LALT);
				}
				if (clockwise) {
					tap_code16(S(KC_TAB));
				} else {
					tap_code16(KC_TAB);
				}
			}
		}
	}
	alt_tab_timer = timer_read();

	return true;
}

void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 5000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}

#endif

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    for (uint8_t i = led_min; i <= led_max; i++) {
		if (!HAS_FLAGS(g_led_config.flags[i], 0x08))
			 continue;
        switch(get_highest_layer(layer_state|default_layer_state)) {
            case _VIMOD:
                rgb_matrix_set_color(i, 0, 32, 0);
                break;
            case _MEDIA:
                rgb_matrix_set_color(i, 0, 0, 32);
                break;
            case _ADMIN:
                rgb_matrix_set_color(i, 32, 32, 0);
                break;
            default:
                rgb_matrix_set_color(i, 32, 0, 0);
                break;
        }
    }
}
