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





