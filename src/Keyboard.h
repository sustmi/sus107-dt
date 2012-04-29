// Keyboard.h

// Copyright (C) 2012  Miroslav Sustek <sus107@vsb.cz>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "Machine.h"

enum KeyboardKeys {
	KEY_CAPS_SHIFT = 0, KEY_Z, KEY_X, KEY_C, KEY_V,
	KEY_A, KEY_S, KEY_D, KEY_F, KEY_G,
	KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T,
	KEY_1, KEY_2, KEY_3, KEY_4, KEY_5,
	KEY_0, KEY_9, KEY_8, KEY_7, KEY_6,
	KEY_P, KEY_O, KEY_I, KEY_U, KEY_Y,
	KEY_ENTER,  KEY_L, KEY_K, KEY_J, KEY_H,
	KEY_SPACE, KEY_SYMBOL_SHIFT, KEY_M, KEY_N, KEY_B,
	KEY_LAST
};

class Keyboard {
public:
	Keyboard();
	virtual ~Keyboard();

	void keyPressed(KeyboardKeys key);
	void keyReleased(KeyboardKeys key);
	uint8_t getKbData(uint8_t addrCode);

private:
	struct RowCol {
		uint8_t row;
		uint8_t col;
	};

	uint8_t colData[8]; // n-th bit = 0 (1): key in n-th column pressed (released)
	struct RowCol mapKeyToRowCol[KEY_LAST];
};

#endif /* KEYBOARD_H_ */
