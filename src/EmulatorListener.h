// EmulatorListener.h

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

#ifndef EMULATORLISTENER_H_
#define EMULATORLISTENER_H_

enum EmulatorEvent {
	EMULATOR_EVENT_VIDEO_FRAME,
	EMULATOR_EVENT_EMULATION_START,
	EMULATOR_EVENT_EMULATION_STOP,
	EMULATOR_EVENT_EMULATION_STEP,
	EMULATOR_EVENT_REGISTERS_CHANGED,
	EMULATOR_EVENT_MEMORY_CHANGED,
};

class EmulatorListener {
public:
	EmulatorListener();
	virtual ~EmulatorListener();

	virtual void emulatorEvent(EmulatorEvent event) = 0;
};

#endif /* EMULATORLISTENER_H_ */
