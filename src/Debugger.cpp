// Debugger.cpp

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

#include "Debugger.h"

Debugger::Debugger() {
	// TODO Auto-generated constructor stub
}

Debugger::~Debugger() {
	// TODO Auto-generated destructor stub
}

void Debugger::addListener(DebuggerListener *listener)
{
	listeners.insert(listener);
}

void Debugger::removeListener(DebuggerListener *listener)
{
	listeners.erase(listener);
}

void Debugger::notifyListeners(DebuggerEvent event)
{
	std::set<DebuggerListener*>::iterator it;
	for (it = listeners.begin(); it != listeners.end(); it++) {
		(*it)->debuggerEvent(event);
	}
}

Emulator *Debugger::getEmulator() const
{
	return emulator;
}

void Debugger::setEmulator(Emulator *emulator)
{
	this->emulator = emulator;
}

void Debugger::stepInstruction()
{
	emulator->getMachine()->stepInstruction();
	emulator->notifyListeners(EMULATOR_EVENT_EMULATION_STEP);
}

void Debugger::emulationContinue()
{
	emulator->start();
}

void Debugger::emulationBreak()
{
	emulator->stop();
}

void Debugger::setCpuRegister(Z80_REG_T reg, Z80EX_WORD value)
{
	emulator->getMachine()->getCpu()->setRegister(reg, value);
	emulator->notifyListeners(EMULATOR_EVENT_REGISTERS_CHANGED);
}

Z80EX_WORD Debugger::getCpuRegister(Z80_REG_T reg)
{
	return emulator->getMachine()->getCpu()->getRegister(reg);
}

void Debugger::readMemory(unsigned char *buffer, int offset, int length)
{
	for (int i = 0; i < length; i++) {
		buffer[i] = this->getEmulator()->getMachine()->getMemory()->rawRead(offset + i);
	}
}

unsigned char Debugger::readMemory(int offset)
{
	return this->getEmulator()->getMachine()->getMemory()->rawRead(offset);
}

void Debugger::writeMemory(int offset, unsigned char value)
{
	this->getEmulator()->getMachine()->getMemory()->rawWrite(offset, value);
	emulator->notifyListeners(EMULATOR_EVENT_MEMORY_CHANGED);
}

void Debugger::writeMemory(unsigned char *buffer, int offset, int length)
{
	for (int i = 0; i < length; i++) {
		this->getEmulator()->getMachine()->getMemory()->rawWrite(offset + i, buffer[i]);
	}
	emulator->notifyListeners(EMULATOR_EVENT_MEMORY_CHANGED);
}

void Debugger::addBreakpoint(uint16_t address)
{
	if (breakpoints.insert(address).second) {
		notifyListeners(DEBUGGER_EVENT_BREAKPOINTS_CHANGED);
	}
}
void Debugger::removeBreakpoint(uint16_t address)
{
	if (breakpoints.erase(address)) {
		notifyListeners(DEBUGGER_EVENT_BREAKPOINTS_CHANGED);
	}
}
bool Debugger::isBreakpoint(uint16_t address)
{
	return breakpoints.count(address) > 0;
}

int Debugger::disassembly(char *output, int output_size, uint16_t addr) {
	int dummy;
	return z80ex_dasm(output, output_size, 0, &dummy, &dummy, ::dasm_readbyte, addr, this);
}

Z80EX_BYTE Debugger::dasm_readbyte(Z80EX_WORD addr) {
	return this->getEmulator()->getMachine()->getMemory()->rawRead(addr);
}
Z80EX_BYTE dasm_readbyte(Z80EX_WORD addr, void *user_data) {
	Debugger *debugger = (Debugger *)user_data;
	return debugger->dasm_readbyte(addr);
}



