// Debugger.h

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

#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include <set>

#include "DebuggerListener.h"
#include "Emulator.h"

class Emulator;
class DebuggerListener;

class Debugger {
public:
	Debugger();
	virtual ~Debugger();

	void addListener(DebuggerListener *listener);
	void removeListener(DebuggerListener *listener);
	void notifyListeners(DebuggerEvent event);

	void addBreakpoint(uint16_t address);
	void removeBreakpoint(uint16_t address);
	bool isBreakpoint(uint16_t address);

	// emulator
	Emulator *getEmulator() const;
	void setEmulator(Emulator *emulator);

	void stepInstruction();
	void emulationContinue();
	void emulationBreak();

	// cpu
	void setCpuRegister(Z80_REG_T reg, Z80EX_WORD value);
	Z80EX_WORD getCpuRegister(Z80_REG_T reg);

	// mem
	void readMemory(unsigned char *buffer, int offset, int length);
	void writeMemory(unsigned char *buffer, int offset, int length);

	unsigned char readMemory(int offset);
	void writeMemory(int offset, unsigned char value);

	int disassembly(char *output, int output_size, uint16_t addr);
	Z80EX_BYTE dasm_readbyte(Z80EX_WORD addr);

private:
	std::set<DebuggerListener*> listeners;
	std::set<uint16_t> breakpoints;

	Emulator *emulator;
};

Z80EX_BYTE dasm_readbyte(Z80EX_WORD addr, void *user_data);

#endif /* DEBUGGER_H_ */
