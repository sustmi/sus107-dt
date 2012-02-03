/*
 * Debugger.h
 *
 *  Created on: 3.2.2012
 *      Author: mirek
 */

#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include "DebuggerView.h"
#include "RegistersView.h"
#include "Emulator.h"

class DebuggerView;
class RegistersView;
class Emulator;

class Debugger {
public:
	Debugger();
	virtual ~Debugger();

	void uiShowMain();
	void uiShowRegisters();
	void uiUpdate();

    Emulator *getEmulator() const;
    void setEmulator(Emulator *emulator);

private:
	DebuggerView *debuggerView;
	RegistersView *registersView;

	Emulator *emulator;
};

#endif /* DEBUGGER_H_ */
