/*
 * Keyboard.cpp
 *
 *  Created on: 18.1.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#include "Keyboard.h"

Keyboard::Keyboard() {
	KeyboardKeys keyMatrix[5][8] = {
		{ KEY_CAPS_SHIFT, KEY_A, KEY_Q, KEY_1, KEY_0, KEY_P, KEY_ENTER, KEY_SPACE },
		{ KEY_Z, KEY_S, KEY_W, KEY_2, KEY_9, KEY_O, KEY_L, KEY_SYMBOL_SHIFT },
		{ KEY_X, KEY_D, KEY_E, KEY_3, KEY_8, KEY_I, KEY_K, KEY_M },
		{ KEY_C, KEY_F, KEY_R, KEY_4, KEY_7, KEY_U, KEY_J, KEY_N },
		{ KEY_V, KEY_G, KEY_T, KEY_5, KEY_6, KEY_Y, KEY_H, KEY_B },
	};

	// initialize mapKeyToRowCol table
	struct RowCol rowCol;
	for (int row = 0; row < 5; row++) {
		rowCol.row = row;
		for (int col = 0; col < 8; col++) {
			rowCol.col = col;
			mapKeyToRowCol[keyMatrix[row][col]] = rowCol;
		}
	}

	for (int col = 0; col < 8; col++) {
		colData[col] = 0x1f; // no key pressed
	}
}

Keyboard::~Keyboard() {
	// TODO Auto-generated destructor stub
}

uint8_t Keyboard::getKbData(uint8_t addrCode) {
	uint8_t data = 0x1f; // D0-D4 pull-up
	for (int col = 0; col < 8; col++) {
		if (!(addrCode & (1 << col))) { // scanned column is pulled down
			data &= colData[col]; // pressed key pulls row down
		}
	}
	return data;
}

void Keyboard::keyPressed(KeyboardKeys key) {
	if (key >= 0 && key < KEY_LAST) {
		RowCol rowCol = mapKeyToRowCol[key];
		colData[rowCol.col] &= ~(1 << rowCol.row); // pressed key pulls line down
	}
}

void Keyboard::keyReleased(KeyboardKeys key) {
	if (key >= 0 && key < KEY_LAST) {
		RowCol rowCol = mapKeyToRowCol[key];
		colData[rowCol.col] |= (1 << rowCol.row); // released key releases line up
	}
}





