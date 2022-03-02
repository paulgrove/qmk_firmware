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
