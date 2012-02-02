/*
 * Ula.h
 *
 *  Created on: 8.12.2011
 *      Author: mirek
 */

#ifndef ULA_H_
#define ULA_H_

#include "Machine.h"
#include "Memory.h"
#include "Cpu.h"
#include "PortDevice.h"
#include "TapeRecorder.h"
#include "Keyboard.h"

// forward declaration
class Machine;
class Memory;
class Cpu;
class TapeRecorder;
class Keyboard;

class Ula : public PortDevice {
public:
	Ula();
	virtual ~Ula();

	void attach(Machine *machine, Memory *memory, Cpu *cpu, TapeRecorder *tapeRecorder, Keyboard *keyboard);

	uint8_t mread(uint16_t addr);
	void mwrite(uint16_t addr, uint8_t value);

	bool getIntLineState();
    void setInterruptLength(uint64_t interruptLength);
    void setInterruptPeriod(uint64_t interruptPeriod);
    void setLastInterruptTime(uint64_t lastInterruptTime);

    // PortDevice methods
    void pread(uint16_t port, uint8_t *value);
    void pwrite(uint16_t port, uint8_t value);

    void renderScreen(uint32_t *buffer);

private:
    Machine *machine;
    Memory *memory;
    Cpu *cpu;

    TapeRecorder *tapeRecorder;
    Keyboard *keyboard;

    uint64_t lastInterruptTime;
    uint64_t interruptPeriod;
    uint64_t interruptLength;

    int borderColor;

    uint64_t getTimeSinceLastInterrupt();
};

#endif /* ULA_H_ */
