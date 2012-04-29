// Joystick.cpp

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

#include "Joystick.h"

Joystick::Joystick() {
	pullDownData = 0xff; // no key pressed
}

Joystick::~Joystick() {
}

void Joystick::keyPressed(JoystickKeys key)
{
	if (key >= 0 && key < JOY_LAST) {
		pullDownData &= ~(1 << key); // pressed key pulls line down
	}
}

void Joystick::keyReleased(JoystickKeys key)
{
	if (key >= 0 && key < JOY_LAST) {
		pullDownData |= (1 << key); // released key releases line
	}
}

void Joystick::pread(uint16_t port, uint8_t *value)
{
	if (port == 0xf7fe) {
		*value = *value & pullDownData;
	}
}

void Joystick::pwrite(uint16_t port, uint8_t value)
{
	// joystick is read-only
}





