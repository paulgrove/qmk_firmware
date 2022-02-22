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
#include "transactions.h"

/* KEYBOARD PET START */

/* settings */
#define MIN_WALK_SPEED	  10
#define MIN_RUN_SPEED	   40

/* advanced settings */
#define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
#define ANIM_SIZE		   96   // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024

/* timers */
uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;

/* current frame */
uint8_t current_frame = 0;

/* status variables */
int   current_wpm = 0;
led_t led_usb_state;

bool isSneaking = false;
bool isJumping  = false;
bool showedJump = true;

uint8_t _i = 0;

/* logic */
static void render_luna(int LUNA_X, int LUNA_Y) {
	/* Sit */
	static const char PROGMEM sit[2][ANIM_SIZE] = {/* 'sit1', 32x22px */
												   {
													   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
												   },

												   /* 'sit2', 32x22px */
												   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

	/* Walk */
	static const char PROGMEM walk[2][ANIM_SIZE] = {/* 'walk1', 32x22px */
													{
														0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x90, 0x90, 0x90, 0xa0, 0xc0, 0x80, 0x80, 0x80, 0x70, 0x08, 0x14, 0x08, 0x90, 0x10, 0x10, 0x08, 0xa4, 0x78, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0xea, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x03, 0x06, 0x18, 0x20, 0x20, 0x3c, 0x0c, 0x12, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
													},

													/* 'walk2', 32x22px */
													{
														0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x28, 0x10, 0x20, 0x20, 0x20, 0x10, 0x48, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0xf8, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x30, 0xd5, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x02, 0x1c, 0x14, 0x08, 0x10, 0x20, 0x2c, 0x32, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
													}};

	/* Run */
	static const char PROGMEM run[2][ANIM_SIZE] = {/* 'run1', 32x22px */
												   {
													   0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x08, 0xc8, 0xb0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xc4, 0xa4, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x58, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x09, 0x04, 0x04, 0x04, 0x04, 0x02, 0x03, 0x02, 0x01, 0x01, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
												   },

												   /* 'run2', 32x22px */
												   {
													   0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x78, 0x28, 0x08, 0x10, 0x20, 0x30, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0xb0, 0x50, 0x55, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x1e, 0x20, 0x20, 0x18, 0x0c, 0x14, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
												   }};

	/* Bark */
	static const char PROGMEM bark[2][ANIM_SIZE] = {/* 'bark1', 32x22px */
													{
														0x00, 0xc0, 0x20, 0x10, 0xd0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
													},

													/* 'bark2', 32x22px */
													{
														0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x2c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x20, 0x4a, 0x09, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
													}};

	/* Sneak */
	static const char PROGMEM sneak[2][ANIM_SIZE] = {/* 'sneak1', 32x22px */
													 {
														 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x80, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x21, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x04, 0x04, 0x04, 0x03, 0x01, 0x00, 0x00, 0x09, 0x01, 0x80, 0x80, 0xab, 0x04, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x02, 0x06, 0x18, 0x20, 0x20, 0x38, 0x08, 0x10, 0x18, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
													 },

													 /* 'sneak2', 32x22px */
													 {
														 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xa0, 0x20, 0x40, 0x80, 0xc0, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x41, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x40, 0x55, 0x82, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x04, 0x18, 0x10, 0x08, 0x10, 0x20, 0x28, 0x34, 0x06, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
													 }};

	/* animation */
	void animate_luna(void) {
		/* jump */
		if (isJumping || !showedJump) {
			/* clear */
			oled_set_cursor(LUNA_X, LUNA_Y + 2);
			oled_write_P(PSTR("     "), false);

			oled_set_cursor(LUNA_X, LUNA_Y - 1);

			showedJump = true;
		} else {
			/* clear */
			oled_set_cursor(LUNA_X, LUNA_Y - 1);
			oled_write_P(PSTR("     "), false);

			oled_set_cursor(LUNA_X, LUNA_Y);
		}

		/* switch frame */
		current_frame = (current_frame + 1) % 2;

		/* current status */
		if (led_usb_state.caps_lock) {
			oled_write_raw_P(bark[abs(1 - current_frame)], ANIM_SIZE);

		} else if (isSneaking) {
			oled_write_raw_P(sneak[abs(1 - current_frame)], ANIM_SIZE);

		} else if (current_wpm <= MIN_WALK_SPEED) {
			oled_write_raw_P(sit[abs(1 - current_frame)], ANIM_SIZE);

		} else if (current_wpm <= MIN_RUN_SPEED) {
			oled_write_raw_P(walk[abs(1 - current_frame)], ANIM_SIZE);

		} else {
			oled_write_raw_P(run[abs(1 - current_frame)], ANIM_SIZE);
		}
	}

	/* animation timer */
	if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
		anim_timer = timer_read32();
		animate_luna();
	}

	/* this fixes the screen on and off bug */
	/*
	if (current_wpm > 0) {
		oled_on();
		anim_sleep = timer_read32();
	} else if (timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
		oled_off();
	}
	*/
}

/* KEYBOARD PET END */


enum sofle_layers {
	/* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
	_QWERTY,
	_LOWER,
	_RAISE,
	_ADJUST,
};

enum custom_keycodes {
	KC_QWERTY = SAFE_RANGE,
	KC_LOWER,
	KC_RAISE,
	KC_LLOCK,
	KC_ULCK,
	KC_ADJUST,
	KC_PRVWD,
	KC_NXTWD,
	KC_LSTRT,
	KC_LEND,
	KC_DLINE
};

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
				 KC_LGUI,KC_LALT,KC_LCTRL, KC_LOWER, KC_SPC,	  KC_ENT,  KC_RAISE, KC_RCTRL, KC_RALT, KC_RGUI
),
*/
[_QWERTY] = LAYOUT(
		 KC_GESC,  KC_1,   KC_2,	KC_3,	KC_4,	KC_5,					  KC_6,	KC_7,	KC_8,	KC_9,	KC_0,  KC_BSPC,
		 KC_TAB,   KC_Q,   KC_W,	KC_E,	KC_R,	KC_T,					  KC_Y,	KC_U,	KC_I,	KC_O,	KC_P,  KC_NUHS,
		 KC_LSFT,  KC_A,   KC_S,	KC_D,	KC_F,	KC_G,					  KC_H,	KC_J,	KC_K,	KC_L, KC_SCLN,  KC_RSHIFT,
 LCTL_T(KC_NUBS),  KC_Z,   KC_X,	KC_C,	KC_V,	KC_B, KC_MUTE,	 KC_MPLY,KC_N,	KC_M, KC_COMM,  KC_DOT, KC_QUOT, RCTL_T(KC_SLSH),
	   				 KC_LGUI,KC_LALT,KC_LBRC, KC_LOWER, KC_SPC,	  KC_ENT,  KC_RAISE, KC_RBRC,   KC_MINS, KC_EQUAL
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
[_LOWER] = LAYOUT(
	 KC_ULCK, _______,  _______, _______, KC_END , _______,						 KC_HOME, _______,  _______, _______, _______, _______,
	 _______, _______,	_______, _______, KC_INS , _______,						 _______, _______, KC_LLOCK, _______, _______, _______,
	 _______, _______,	_______, _______, KC_PGDN, _______,						 KC_LEFT, KC_DOWN,  KC_UP  , KC_RGHT, _______, _______,
	 _______, _______,	KC_DEL , _______, _______, KC_PGUP, _______,	   _______, _______, _______,  _______, _______, _______, _______,
						_______, _______, _______, _______, _______,	   _______, _______, _______,  _______, _______
),
/*
[_LOWER] = LAYOUT(
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
[_RAISE] = LAYOUT(

  KC_ULCK,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,						   KC_F6,   KC_F7,	KC_F8,   KC_F9,  KC_F10,  KC_F11,
  _______, _______,	  KC_UP, _______, _______, _______,						 _______, KC_PAUS, KC_LLOCK, _______, KC_PSCR,  KC_F12,
  _______, KC_LEFT,	KC_DOWN, KC_RGHT, _______, _______,						 _______, _______,  KC_CAPS, KC_SCRL, _______, _______,
  _______, _______,	_______, _______, _______, _______, _______,	   _______, _______, _______,  _______, _______, _______, _______,
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
  [_ADJUST] = LAYOUT(
  KC_ULCK, _______, _______, _______, _______, _______,						    _______, _______, _______, _______, _______, _______,
  _______, _______,	_______, _______, RGB_MOD, RGB_TOG,						    _______, _______, _______, _______, _______, _______,
  KC_CAPS, KC_PAUS,	KC_SCRL, _______, _______, _______,						    _______, _______, _______, _______, _______, _______,
  _______, _______,	_______, _______, _______, _______, _______,	   _______, KC_NUM , _______, _______, _______, _______, _______,
					_______, _______, _______, _______, _______,	   _______, _______, _______, _______, _______
  )
};

#ifdef OLED_ENABLE

static const char PROGMEM raw_vim_logo[] = { 0,  0, 28, 18,226,  2,  2,  2,  2,  2,  2,  2,242,242,252,254,254,252,248,248,114, 50,  2,  2,  2,  2,  2,  2,130, 66, 34, 28,  0,128,192,224,255,  0,  0,  0,  0,  0,  0,  0,127, 63, 31, 15,  7,  3,  1,  0,128,192,224,240,248,252,250,241,224,192,128,  0,  0,  1,  3,  7,255,  0,  0,  0,  0,  0,  0,  0,128,192, 96, 16, 16,240,254, 63, 63, 63, 63, 63, 63, 63, 63, 47, 39,195,  1,  0,  0,  0,  0,  0, 15, 16, 16, 16,  8,  4,  2,  7, 15, 19, 48,112,126, 63, 19, 16, 24, 15, 19, 16, 24, 15, 19, 16, 24, 15,  0,  0};
static const char PROGMEM raw_gaming_logo[] = {0,  0,  0,  0,  0,  0,  0,128,192,224,224,192,128,128,192,240,248,220,140,142,198,230,230,198,134,  6, 14, 28, 56,112,224,192,  0,  0,  0,248,252,254,207,207,  3,  3,207,207,255,255,246,249,249,246,255,255,207,207, 51, 51,207,207,254,252,248,  0,  0,  1,  0,224,252,255,255,255,255,255,255,243, 97, 97,115, 63, 31, 31, 31, 31, 63,115, 97, 97,243,255,255,255,255,255,255,252,224,  0,  0,  7, 15, 31, 31, 15,  7,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  7, 15, 31, 31, 15,  7,  0,};
static const char PROGMEM raw_pokemon_logo[] = {0,  0,  0, 24, 32,224,224,192,192,128,192,192,192,224,224,224,224,224,224,224,224,224,224,192,128,128,192,224,224, 32,  0,  0,  0,  0, 32, 16, 48,176,185,255,255,255,255,189, 49,113,249,113,115,247,127,127,247,123,115,243,113, 61,255,251,112, 96, 64,  0,128,192,224,240,113, 48,161,131,135,  7, 31, 63,255,254,124,254,252,252,254,252,252,254,252, 14,  2,243,251,185, 92,252,252, 60, 25, 12, 15,  7,  4,  1,  3,  3,  1,  0,  0,  0,  0,  3,  0,  3, 15,  1, 63, 15,  3,  0,  1,  0,  0,  0,  1,  1,  0,  1,  0,  0};

typedef struct _master_to_slave_t {
	uint8_t data;
} master_to_slave_t;

master_to_slave_t flutterstate;

#define FLUT_HEAD_UP flutterstate.data & 1
#define FLUT_FLAP_UP flutterstate.data & 2

void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
	const master_to_slave_t *m2s = (const master_to_slave_t*)in_data;
	if (m2s->data & 1)
		flutterstate.data ^= 1;
	if (m2s->data & 2)
		flutterstate.data ^= 2;
}

void keyboard_post_init_user(void) {
	if (!is_keyboard_master()) {
		transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
	}
}

void housekeeping_task_user(void) {
	if (flutterstate.data == 0)
		return;
	if (is_keyboard_master()) {
			if(transaction_rpc_send(USER_SYNC_A, sizeof(flutterstate), &flutterstate)) {
				flutterstate.data = 0;
			}
	}
}

#define FLUT_FLOOR 11

void write_chars(char character, uint8_t len) {
	uint8_t i;
	for (i = 0; i < len; i++) {
		oled_write_char(character + i, false); // flut head 1
	}
}

int8_t lastElev = 0;
static void render_fluttershy(void) {
	int8_t elev = FLUT_FLOOR - (get_current_wpm() / 15);
	if (elev < 0)
		elev = 0;
	if (elev != lastElev) {
		flutterstate.data |= 1;
		flutterstate.data ^= 2;
		lastElev = elev;
	}
	oled_set_cursor(1, 0);
	for (_i = 0; _i <= FLUT_FLOOR; _i++) {
		if (_i != elev) {
			if (_i == 0) {
				oled_write_P(PSTR("    *"), false);
			} else {
				oled_advance_page(true);
			}
			continue;
		}

		/* LINE 1 */
		if (FLUT_HEAD_UP) {
			write_chars(0, 3);
		} else {
			oled_write_P(PSTR("   "), false);
		}
		if (FLUT_FLAP_UP) {
			oled_write_char(9, false); // flut wing up 1
		}
		oled_advance_page(true);

		/* LINE 2 */
		write_chars(FLUT_HEAD_UP ? 3 : 0, 3);
		oled_write_char(FLUT_FLAP_UP ? 11 : 23, false); // flut wing up 2
		oled_write_char(15, false); // flut wing tail 1

		/* LINE 3 */
		write_chars(FLUT_HEAD_UP ? 6 : 3, 3);
		oled_write_char(FLUT_FLAP_UP ? 12 : 14, false); // flut wing up 3
		oled_write_char(16, false); // flut wing tail 2

		/* LINE 4 */
		if (FLUT_HEAD_UP) {
			oled_write_P(PSTR("  "), false);
			write_chars(_i == FLUT_FLOOR ? 17 : 20, 3);
		} else {
			write_chars(6, 3);
			write_chars(_i == FLUT_FLOOR ? 18 : 21, 2);
		}

	}
}

static void print_status_left(uint8_t layer) {
	switch (layer) {
		case _QWERTY:
			oled_write_raw_P(raw_pokemon_logo, sizeof(raw_pokemon_logo));
			oled_set_cursor(0, 4);
			for (_i = 0; _i < 4; _i++) {
				oled_write_P(PSTR("     "), false);
			}
			oled_write("CPSLK", led_usb_state.caps_lock);

			render_luna(0, 13);
			break;
		case _RAISE:
			oled_write_raw_P(raw_gaming_logo, sizeof(raw_gaming_logo));
			oled_set_cursor(0, 4);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("w UP "), false);
			oled_write_P(PSTR("s DWN"), false);
			oled_write_P(PSTR("a LFT"), false);
			oled_write_P(PSTR("d RGT"), false);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("1-0 ="), false);
			oled_write_P(PSTR("F1-10"), false);
			break;
		case _LOWER:
			oled_write_raw_P(raw_vim_logo, sizeof(raw_vim_logo));
			oled_set_cursor(0, 4);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("$ END"), false);
			oled_write_P(PSTR("r INS"), false);
			oled_write_P(PSTR("x DEL"), false);
			oled_write_P(PSTR("f PGD"), false);
			oled_write_P(PSTR("b PGU"), false);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("ESC `"), false);
			break;
		case _ADJUST:
			oled_write_raw_P(raw_pokemon_logo, sizeof(raw_pokemon_logo));
			oled_set_cursor(0, 4);

			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("     "), false);
			break;
		default:
			oled_write_ln_P(PSTR("Undef"), false);
	}

	current_wpm = get_current_wpm();
	led_usb_state = host_keyboard_led_state();

}

static void print_wpm(void) {
	/* wpm counter */
	oled_set_cursor(0, 15);
	oled_write(get_u8_str(get_current_wpm(), ' '), false);
}

static void print_status_right(uint8_t layer) {
	switch (layer) {
		case _QWERTY:
			render_fluttershy();
			print_wpm();
			break;
		case _RAISE:
			oled_write_raw_P(raw_gaming_logo, sizeof(raw_gaming_logo));
			oled_set_cursor(0, 4);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("6-0 ="), false);
			oled_write_P(PSTR("F6-10"), false);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("DEL ="), false);
			oled_write_P(PSTR("F11  "), false);
			oled_write_P(PSTR("# F12"), false);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("K CAP"), false);
			break;
		case _LOWER:
			oled_write_raw_P(raw_vim_logo, sizeof(raw_vim_logo));
			oled_set_cursor(0, 4);
			oled_write_P(PSTR("     "), false);
			oled_write_P(PSTR("^ BGN"), false);
			oled_write_P(PSTR("h LFT"), false);
			oled_write_P(PSTR("j DWN"), false);
			oled_write_P(PSTR("k UP "), false);
			oled_write_P(PSTR("l RHT"), false);
			break;
		case _ADJUST:
			oled_write_raw_P(raw_pokemon_logo, sizeof(raw_pokemon_logo));
			oled_set_cursor(0, 4);
			break;
		default:
			oled_write_P(STR("Undef"), false);
	}
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	if (is_keyboard_master()) {
		return OLED_ROTATION_270;
	}
	else {
		return OLED_ROTATION_270;
	}
	return rotation;
}

uint8_t last_layer = 100;
bool oled_task_user(void) {
	uint8_t layer = get_highest_layer(layer_state);
	if (last_layer != layer) {
		last_layer = layer;
		oled_clear();
	} else {
		oled_set_cursor(0, 0);
	}
	if (is_keyboard_master()) {
		print_status_left(layer);
	} else {
		print_status_right(layer);
		//render_logo();
	}
	return false;
}



#endif // OLED_ENABLE

bool llock = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		flutterstate.data ^= 2;
		if (keycode == KC_SPC || keycode == KC_ENT) {
			flutterstate.data ^= 1;
		}
	}
	switch (keycode) {
		case KC_QWERTY:
			if (record->event.pressed) {
				set_single_persistent_default_layer(_QWERTY);
			}
			return false;
		case KC_LOWER:
			if (record->event.pressed) {
				layer_on(_LOWER);
				update_tri_layer(_LOWER, _RAISE, _ADJUST);
			} else {
				if (!llock) {
					layer_off(_LOWER);
					update_tri_layer(_LOWER, _RAISE, _ADJUST);
				}
			}
			return false;
		case KC_RAISE:
			if (record->event.pressed) {
				layer_on(_RAISE);
				update_tri_layer(_LOWER, _RAISE, _ADJUST);
			} else {
				if (!llock) {
					layer_off(_RAISE);
					update_tri_layer(_LOWER, _RAISE, _ADJUST);
				}
			}
			return false;
		case KC_ADJUST:
			if (record->event.pressed) {
				layer_on(_ADJUST);
			} else {
				layer_off(_ADJUST);
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
				layer_off(_LOWER);
				layer_off(_RAISE);
				layer_off(_ADJUST);
				update_tri_layer(_LOWER, _RAISE, _ADJUST);
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

bool encoder_update_user(uint8_t index, bool clockwise) {
	uint8_t temp_mod = get_mods();
	uint8_t temp_osm = get_oneshot_mods();
	bool	is_ctrl  = (temp_mod | temp_osm) & MOD_MASK_CTRL;
	bool	is_shift = (temp_mod | temp_osm) & MOD_MASK_SHIFT;

	if (is_shift) {
		if (index == 0) { /* First encoder */
			if (clockwise) {
				rgb_matrix_increase_hue();
			} else {
				rgb_matrix_decrease_hue();
			}
		} else if (index == 1) { /* Second encoder */
			if (clockwise) {
				rgb_matrix_decrease_sat();
			} else {
				rgb_matrix_increase_sat();
			}
		}
	} else if (is_ctrl) {
		if (index == 0) { /* First encoder */
			if (clockwise) {
				rgb_matrix_increase_val();
			} else {
				rgb_matrix_decrease_val();
			}
		} else if (index == 1) { /* Second encoder */
			if (clockwise) {
				rgb_matrix_increase_speed();
			} else {
				rgb_matrix_decrease_speed();
			}
		}
	} else {
		if (index == 1) { /* First encoder */
			if (clockwise) {
				tap_code(KC_PGUP);
				// tap_code(KC_MS_WH_UP);
			} else {
				tap_code(KC_PGDOWN);
				// tap_code(KC_MS_WH_DOWN);
			}
		} else if (index == 0) { /* Second encoder */
			uint16_t mapped_code = 0;
			if (clockwise) {
				mapped_code = KC_VOLD;
			} else {
				mapped_code = KC_VOLU;
			}
			tap_code_delay(mapped_code, MEDIA_KEY_DELAY);
		}
	}
	return true;
}

#endif

