EXTRAKEY_ENABLE = yes
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812
WPM_ENABLE = yes

LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
SPACE_CADET_ENABLE = no
MAGIC_ENABLE = no

SRC += flutter-animate.c \
	   luna.c \
	   vim-logo.c \
	   pokemon-logo.c \
	   gaming-logo.c \
	   render-oled.c \
	   enums.c \
	   layers.c
