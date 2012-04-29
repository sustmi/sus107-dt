// Joystick.h

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

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "PortDevice.h"

enum JoystickKeys {
	JOY_LEFT = 0, JOY_RIGHT, JOY_DOWN, JOY_UP, JOY_FIRE,
	JOY_LAST
};

class Joystick: public PortDevice {
public:
	Joystick();
	virtual ~Joystick();

	void keyPressed(JoystickKeys key);
	void keyReleased(JoystickKeys key);

	// PortDevice methods
	void pread(uint16_t port, uint8_t *value);
	void pwrite(uint16_t port, uint8_t value);

private:
	uint8_t pullDownData;
};

#endif /* JOYSTICK_H_ */
