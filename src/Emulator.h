// Emulator.h

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

#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "Machine.h"
#include "Cpu.h"
#include "Ula.h"
#include "TapeRecorder.h"
#include "Keyboard.h"
#include "Joystick.h"
#include "Speaker.h"

#include "Debugger.h"

class Debugger;

class Emulator {
public:
	Emulator();
	virtual ~Emulator();

	void init();
	void start();
	void stop();

	void runUntil(uint64_t time);
	bool isRunning();

	Debugger *getDebugger() const;
	Machine *getMachine() const;
	Keyboard *getKeyboard() const;
	Joystick *getJoystick() const;
	TapeRecorder *getTapeRecorder() const;
	Speaker *getSpeaker() const;
	void renderScreen(uint32_t *buffer);

private:
	bool running;

	Debugger *debugger;

	Machine *machine;
	Cpu *cpu;
	Ula *ula;
	TapeRecorder *tapeRecorder;
	Speaker *speaker;
	Keyboard *keyboard;
	Joystick *joystick;
};

#endif /* EMULATOR_H_ */
