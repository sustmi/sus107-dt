/*
 * Debugger.h
 *
 *  Created on: 3.2.2012
 *      Author: mirek
 */

#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include <set>

#include "DebuggerListener.h"
#include "ui/DebuggerView.h"
#include "ui/DebuggerRegistersView.h"
#include "Emulator.h"

class DebuggerView;
class DebuggerRegistersView;
class Emulator;

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

    int dasm(char *output, int output_size, unsigned flags, int *t_states, int *t_states2, Z80EX_WORD addr);
    Z80EX_BYTE dasm_readbyte(Z80EX_WORD addr);

private:
    std::set<DebuggerListener*> listeners;
    std::set<uint16_t> breakpoints;

	Emulator *emulator;
};

#endif /* DEBUGGER_H_ */
