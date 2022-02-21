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
#pragma once

#define ENABLE_LUNA
#define OLED_FONT_H "keyboards/sofle/keymaps/paulgrove/flutter-font.c"
#define OLED_FONT_START 0
#define OLED_FONT_END 127
#define OLED_TIMEOUT 120000
#define OLED_BRIGHTNESS 120

#define WPM_SAMPLE_SECONDS 2
#define WPM_SAMPLE_PERIODS 30

#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_A

/* The way how "handedness" is decided (which half is which),
see https://docs.qmk.fm/#/feature_split_keyboard?id=setting-handedness
for more options.
*/

#define RGB_DI_PIN D3

#ifdef RGB_MATRIX_ENABLE

#define RGBLED_NUM 72
#define DRIVER_LED_TOTAL RGBLED_NUM
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120 // limits maximum brightness of LEDs to 150 out of 255. Higher may cause the controller to crash.
#define RGB_MATRIX_HUE_STEP 8
#define RGB_MATRIX_SAT_STEP 8
#define RGB_MATRIX_VAL_STEP 8
#define RGB_MATRIX_SPD_STEP 10
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_SPLIT {36,36}
#define SPLIT_TRANSPORT_MIRROR
//#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#define ENABLE_RGB_MATRIX_DIGITAL_RAIN
//#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#define ENABLE_RGB_MATRIX_MULTISPLASH
#define ENABLE_RGB_MATRIX_CYCLE_ALL
//#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS

#endif

#ifdef RGBLIGHT_ENABLE
#define RGBLIGHT_SPLIT
#define RGBLED_NUM 70
#define RGB_SPLIT {36,36}
#define RGBLIGHT_LIMIT_VAL 120
//   #define RGBLIGHT_SLEEP  /* If defined, the RGB lighting will be switched off when the host goes to sleep */
// /*== all animations enable ==*/
//   #define RGBLIGHT_ANIMATIONS
// /*== or choose animations ==*/
//   #define RGBLIGHT_EFFECT_BREATHING
   //#define RGBLIGHT_EFFECT_RAINBOW_MOOD
   //#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
//   #define RGBLIGHT_EFFECT_SNAKE
//   #define RGBLIGHT_EFFECT_KNIGHT
//   #define RGBLIGHT_EFFECT_CHRISTMAS
//  #define RGB_MATRIX_RAINBOW_MOVING_CHEVRON
//   #define RGBLIGHT_EFFECT_STATIC_GRADIENT
//   #define RGBLIGHT_EFFECT_RGB_TEST
//   #define RGBLIGHT_EFFECT_ALTERNATING
#endif

#define MEDIA_KEY_DELAY 2

#define USB_POLLING_INTERVAL_MS 1
#define QMK_KEYS_PER_SCAN 12

#define SPLIT_MODS_ENABLE
#define SPLIT_OLED_ENABLE
#define SPLIT_WPM_ENABLE
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_TRANSPORT_MIRROR

#define LAYER_STATE_8BIT

