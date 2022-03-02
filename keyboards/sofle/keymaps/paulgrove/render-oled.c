#include QMK_KEYBOARD_H
#include "./luna.h"
#include "./vim-logo.h"
#include "./gaming-logo.h"
#include "./enums.h"

#ifdef LEFT_FIRMWARE
#include "./pokemon-logo.h"

void render_oled(uint8_t layer, uint8_t current_wpm) {
	switch (layer) {
		case _QWERTY:
			oled_write_raw_P(raw_pokemon_logo, sizeof(raw_pokemon_logo));
			oled_set_cursor(0, 6);
			oled_write("abstu", false); // Desktop Switcher
			oled_write("cdvwx", false); //
			oled_advance_page(true);
			oled_write("CAP h", led_usb_state.caps_lock);
			oled_write("NUM h", led_usb_state.num_lock);
			oled_write("SCR h", led_usb_state.scroll_lock);
#ifdef ENABLE_LUNA
			render_luna(0, 13, current_wpm);
#endif
			break;
		case _MEDIA:
			oled_write_raw_P(raw_gaming_logo, sizeof(raw_gaming_logo));
			oled_set_cursor(0, 5);
			oled_write_ln_P(PSTR("W UP"), false);
			oled_write_P(PSTR("S DWN"), false);
			oled_write_P(PSTR("A LFT"), false);
			oled_write_P(PSTR("D RGT"), false);
			oled_advance_page(true);
			oled_write_P(PSTR("1-0 z"), false);
			oled_write_P(PSTR("F1-10"), false);
			oled_set_cursor(0, 14);
			oled_write("ab ef", false); // Audio indicator
			oled_write("cd eg", false); // Audio mute
			break;
		case _VIMOD:
			oled_write_raw_P(raw_vim_logo, sizeof(raw_vim_logo));
			oled_set_cursor(0, 5);
			oled_write_P(PSTR("$ END"), false);
			oled_write_P(PSTR("R INS"), false);
			oled_write_P(PSTR("X DEL"), false);
			oled_write_P(PSTR("F PGD"), false);
			oled_write_P(PSTR("B PGU"), false);
			oled_advance_page(true);
			oled_write_P(PSTR("ESC `"), false);
			break;
		case _ADMIN:
			oled_write_P(PSTR("SHFTz"), false);
			oled_write_P(PSTR("CAPSh"), led_usb_state.caps_lock);
			oled_advance_page(true);
			oled_write_ln_P(PSTR("Az"), false);
			oled_write_P(PSTR("PAUSE"), false);
			oled_advance_page(true);
			oled_write_ln_P(PSTR("Dz"), false);
			oled_write_P(PSTR("SCRLh"), led_usb_state.scroll_lock);
			oled_advance_page(true);
			oled_write_P(PSTR("*RGB*"), false);
			oled_write_P(PSTR("R=NXT"), false);
			oled_write_P(PSTR("T=OFF"), false);
			oled_advance_page(true);
			oled_advance_page(true);
			oled_write_P(PSTR("ab   "), false);
			oled_write_P(PSTR("cd   "), false);
			break;
		default:
			oled_write_ln_P(PSTR("Undef"), false);
	}

	led_usb_state = host_keyboard_led_state();
}

#else

#include "./flutter-animate.h"

void render_oled(uint8_t layer, uint8_t current_wpm) {
	switch (layer) {
		case _QWERTY:
#ifdef ENABLE_FLUTTERSHY_DISPLAY
			render_fluttershy(current_wpm);
#endif
			if (current_wpm < 15) {
				oled_set_cursor(0, 6);
				oled_write("abijk", false); // Desktop Switcher
				oled_write("cdlmn", false); //
			}
			break;
		case _MEDIA:
			oled_write_raw_P(raw_gaming_logo, sizeof(raw_gaming_logo));
			oled_set_cursor(0, 5);
			oled_write_ln_P(PSTR("6-0z"), false);
			oled_write_P   (PSTR("F6-10"), false);
			oled_advance_page(true);
			oled_write_ln_P(PSTR("DELz"), false);
			oled_write_ln_P(PSTR("F11"), false);
			oled_write_P   (PSTR("# F12"), false);
			oled_set_cursor(0, 14);
			oled_write("ab pr", false); // Audio skip track
			oled_write("cd op", false); // Audio pause play
			break;
		case _VIMOD:
			oled_write_raw_P(raw_vim_logo, sizeof(raw_vim_logo));
			oled_set_cursor(0, 5);
			oled_write_P(PSTR("^ BGN"), false);
			oled_write_P(PSTR("H LFT"), false);
			oled_write_P(PSTR("J DWN"), false);
			oled_write_P(PSTR("K UP "), false);
			oled_write_P(PSTR("L RHT"), false);
			break;
		case _ADMIN:
			oled_set_cursor(0, 0);
			oled_write_P(PSTR("N `  "), false);
			oled_write("NUMBh", led_usb_state.num_lock);
			break;
		default:
			oled_write_P(STR("Undef"), false);
	}
}

#endif

