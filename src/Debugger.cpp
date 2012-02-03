/*
 * Debugger.cpp
 *
 *  Created on: 3.2.2012
 *      Author: mirek
 */

#include "Debugger.h"

Debugger::Debugger() {
	// TODO Auto-generated constructor stub
	debuggerView = NULL;
	registersView = NULL;
}

Debugger::~Debugger() {
	// TODO Auto-generated destructor stub
}

void Debugger::uiShowMain()
{
	if (!debuggerView) {
		debuggerView = new DebuggerView(emulator, wxID_ANY, wxEmptyString);
	}
	debuggerView->setDebugger(this);
	debuggerView->Show(true);
	debuggerView->uiUpdate();
}

void Debugger::uiShowRegisters()
{
	if (!debuggerView) {
		return;
	}

	if (!registersView) {
		registersView = new RegistersView(debuggerView, wxID_ANY, wxEmptyString);
	}
	registersView->setDebugger(this);
	registersView->Show(true);
	registersView->uiUpdate();
}

void Debugger::uiUpdate()
{
	if (debuggerView) {
		debuggerView->uiUpdate();
	}
	if (registersView) {
		registersView->uiUpdate();
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



