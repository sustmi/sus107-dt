/*
 * Debugger.cpp
 *
 *  Created on: 3.2.2012
 *      Author: mirek
 */

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
	notifyListeners(DEBUGGER_EVENT_EMULATION_STEP);
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
	notifyListeners(DEBUGGER_EVENT_REGISTERS_CHANGED);
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
	notifyListeners(DEBUGGER_EVENT_MEMORY_CHANGED);
}

void Debugger::writeMemory(unsigned char *buffer, int offset, int length)
{
	for (int i = 0; i < length; i++) {
		this->getEmulator()->getMachine()->getMemory()->rawWrite(offset + i, buffer[i]);
	}
	notifyListeners(DEBUGGER_EVENT_MEMORY_CHANGED);
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

int Debugger::dasm(char *output, int output_size, unsigned  flags, int *t_states, int *t_states2, Z80EX_WORD addr) {
	return z80ex_dasm(output, output_size, flags, t_states, t_states2, ::dasm_readbyte, addr, this);
}

Z80EX_BYTE Debugger::dasm_readbyte(Z80EX_WORD addr) {
	return this->getEmulator()->getMachine()->getMemory()->rawRead(addr);
}
Z80EX_BYTE dasm_readbyte(Z80EX_WORD addr, void *user_data) {
	Debugger *debugger = (Debugger *)user_data;
	return debugger->dasm_readbyte(addr);
}



