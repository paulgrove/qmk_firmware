#include QMK_KEYBOARD_H
#include "transactions.h"

#ifdef ENABLE_FLUTTERSHY

typedef struct _master_to_slave_t {
	uint8_t data;
} master_to_slave_t;

master_to_slave_t flutterstate;

#define FLUT_HEAD_UP flutterstate.data & 1
#define FLUT_FLAP_UP flutterstate.data & 2

#ifdef ENABLE_FLUTTERSHY_DISPLAY
#define FLUT_FLOOR 10

static const char PROGMEM raw_sunshine[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2, 34, 50, 18, 26,  8,128,135, 15, 31, 63,127,127,127,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  6,  3,  1,  0, 48, 28,  6,  0,  0, 62,
};

void write_chars(char character, uint8_t len) {
	uint8_t i;
	for (i = 0; i < len; i++) {
		oled_write_char(character + i, false); // flut head 1
	}
}

int8_t lastElev = 0;
void render_fluttershy(uint8_t current_wpm) {
	oled_write_raw_P(raw_sunshine, sizeof(raw_sunshine));
	uint8_t s = 2;
	uint8_t i;
	if (current_wpm >= 10)
		s = 1;
	if (current_wpm >= 100)
		s = 0;
	oled_write(&(get_u8_str(current_wpm, ' ')[s]), false);
	int8_t elev = FLUT_FLOOR - (current_wpm / 15);
	if (elev < 0)
		elev = 0;
	if (elev != lastElev) {
		flutterstate.data |= 1;
		flutterstate.data ^= 2;
		lastElev = elev;
	}
	oled_set_cursor(0, 2);
	for (i = 0; i <= FLUT_FLOOR; i++) {
		if (i != elev) {
			oled_advance_page(true);
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
			write_chars(i == FLUT_FLOOR ? 17 : 20, 3);
		} else {
			write_chars(6, 3);
			write_chars(i == FLUT_FLOOR ? 18 : 21, 2);
		}

	}
}
#endif

#ifdef ENABLE_FLUTTERSHY_MASTER
void housekeeping_task_user_fluttershy(void) {
	if (flutterstate.data == 0)
		return;
	if (is_keyboard_master()) {
			if(transaction_rpc_send(USER_SYNC_A, sizeof(flutterstate), &flutterstate)) {
				flutterstate.data = 0;
			}
	}
}
#endif

#ifdef ENABLE_FLUTTERSHY_SLAVE
void user_sync_fluttershy_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
	const master_to_slave_t *m2s = (const master_to_slave_t*)in_data;
	if (m2s->data & 1)
		flutterstate.data ^= 1;
	if (m2s->data & 2)
		flutterstate.data ^= 2;
}
#endif

void flapwing_fluttershy(void) {
	flutterstate.data ^= 2;
}

void bobhead_fluttershy(void) {
	flutterstate.data ^= 1;
}

#endif
