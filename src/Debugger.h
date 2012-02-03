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

    int dasm(char *output, int output_size, unsigned flags, int *t_states, int *t_states2, Z80EX_WORD addr);
    Z80EX_BYTE dasm_readbyte(Z80EX_WORD addr);

private:
	DebuggerView *debuggerView;
	RegistersView *registersView;

	Emulator *emulator;
};

#endif /* DEBUGGER_H_ */
