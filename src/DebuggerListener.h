// DebuggerListener.h

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

#ifndef DEBUGGERLISTENER_H_
#define DEBUGGERLISTENER_H_

enum DebuggerEvent {
	DEBUGGER_EVENT_REGISTERS_CHANGED,
	DEBUGGER_EVENT_MEMORY_CHANGED,
	DEBUGGER_EVENT_EMULATION_START,
	DEBUGGER_EVENT_EMULATION_STOP,
	DEBUGGER_EVENT_EMULATION_STEP,
	DEBUGGER_EVENT_BREAKPOINTS_CHANGED,
};

class DebuggerListener {
public:
	DebuggerListener();
	virtual ~DebuggerListener();

	virtual void debuggerEvent(DebuggerEvent event) = 0;
};

#endif /* DEBUGGERLISTENER_H_ */
