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

#include "sofle.h"

#ifdef RGB_MATRIX_ENABLE
  // Physical Layout
  // Columns
  // 0  1  2  3  4  5  6  7  8  9  10 11 12 13
  //                                           ROWS
  // 12 13 22 23 32 33       33 32 23 22 13 12  0
  //    02    03    04       04    03    02
  // 11 14 21 24 31 34       34 31 24 21 14 11  1
  //                01       01
  // 10 15 20 25 30 35       35 30 25 20 15 10  2
  //
  // 09 16 19 26 29 36       36 29 26 19 16 09  3
  //
  //     08 17 18 27 28     28 27 18 17 08      4
  //    07    06    05       05    06    07

led_config_t g_led_config = {
    {
        {  11,  12,  21,  22,  31,  32 },
        {  10,  13,  20,  23,  30,  33 },
        {  9,   14,  19,  24,  29,  34},
        {  8,   15,  18,  25,  28,  35},
        {  7,   16,  17,  26,  27,  NO_LED },
        {  47,  48,  57,  58,  67,  68},
        {  46,  49,  56,  59,  66,  69},
        {  45,  50,  55,  60,  65,  70},
        {  44,  51,  54,  61,  64,  71},
        {  43,  52,  53,  62,  63,  NO_LED }
    },
    {
//		1
        {90, 30},
    	// Left side underglow
//		2         3         4
		{36, 15}, {59, 11}, {82, 14}, // left to right
//		5         6         7
		{89, 52}, {65, 49}, {43, 50}, // right to left
        // Left side Matrix
//		8         9         10        11        12        13
        {48, 49}, {25, 40}, {25, 29}, {25, 18}, {25,  6}, {37,  6},
//		14        15        16        17        18        19
		{37, 17}, {37, 29}, {37, 40}, {59, 47}, {48, 49}, {48, 37},
//      20        21        22        23        24        25
		{48, 26}, {48, 15}, {48,  3}, {59,  2}, {59, 13}, {59, 24},
//      26        27        28        29        30        31
		{59, 36}, {84, 51}, {93, 56}, {70, 37}, {70, 26}, {70, 15},
//                          32        33        34        35        36
		                    {70,  4}, {82,  5}, {82, 16}, {82, 34}, {82, 39},

	/*
        {96, 40},
       // Left side underglow
		{16, 20}, {48, 10}, {80, 18}, {88, 60}, {56, 57}, {24,60},
        // Left side Matrix
        {32, 57}, { 0, 48}, { 0, 36}, { 0, 24}, { 0, 12}, {16, 12},
		{16, 24}, {16, 36}, {16, 48}, {48, 55}, {64, 57}, {32, 45},
		{32, 33}, {32, 21}, {32,  9}, {48,  7}, {48, 19}, {48, 31},
		{48, 43}, {80, 59}, {96, 64}, {64, 45}, {64, 33}, {64, 21},
		                    {64,  9}, {80, 10}, {80, 22}, {80, 34}, {80, 47},
	*/

        // Right side underglow
//		1
        {104, 30},
//		2          3          4
		{188, 15}, {165, 10}, {142, 14},
//		5          6          7
		{135, 52}, {159, 48}, {180, 50},
        // Right side Matrix
//		8          9          10         11         12         13
        {176, 48}, {199, 40}, {199, 29}, {199, 17}, {199,  6}, {187,  6},
//		14         15         16         17         18         19
		{187, 17}, {187, 29}, {187, 40}, {187, 49}, {154, 48}, {176, 37},
//      20         21         22         23         24         25
		{176, 26}, {176, 15}, {176,  3}, {165,  2}, {165, 13}, {165, 25},
//      26         27         28         29         30         31
		{165, 36}, {140, 52}, {131, 56}, {154, 37}, {154, 26}, {154, 15},
//                            32         33         34         35         36
		           			  {154, 45}, {142,  5}, {142, 16}, {142, 28}, {142, 39},
    },
    {
        LED_FLAG_INDICATOR, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_INDICATOR, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW, LED_FLAG_UNDERGLOW,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
        LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT
    }
};
#endif
