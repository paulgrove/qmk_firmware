#include QMK_KEYBOARD_H

enum sofle_layers {
	/* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
	_QWERTY,
	_VIMOD,
	_MEDIA,
	_ADMIN,
};

enum custom_keycodes {
	KC_QWERTY = SAFE_RANGE,
	KC_VIMOD,
	KC_MEDIA,
	KC_LLOCK,
	KC_ULCK,
	KC_ADMIN,
	KC_PRVWD,
	KC_NXTWD,
	KC_SAPP,
	//TICK_ESC,
	//KC_LSTRT,
	//KC_LEND,
	//KC_DLINE
};

