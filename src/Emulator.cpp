/*
 * Emulator.cpp
 *
 *  Created on: 20.3.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#include "Emulator.h"

Emulator::Emulator() {
	ula = NULL;
}

Emulator::~Emulator() {
	// TODO Auto-generated destructor stub
}

void Emulator::init()
{
	running = false;

	Memory *memory = new Memory();
	cpu = new Cpu();
	ula = new Ula();
	Ports *ports = new Ports();

	tapeRecorder = new TapeRecorder();
	keyboard = new Keyboard();
	joystick = new Joystick();
	speaker = new Speaker();

	machine = new Machine();
	machine->attach(memory, cpu, ula);

	memory->attach(machine);
	cpu->attach(ula, ports);
	ula->attach(machine, memory, cpu, keyboard, tapeRecorder, speaker);
	tapeRecorder->attach(machine);
	speaker->attach(machine);

	ports->connectDevice(ula);
	ports->connectDevice(joystick);

	machine->loadRom("48.rom");

	// Spectrum 48k machine specifics
	ula->setInterruptLength(libspectrum_timings_interrupt_length(LIBSPECTRUM_MACHINE_48));
	ula->setInterruptPeriod(libspectrum_timings_tstates_per_frame(LIBSPECTRUM_MACHINE_48));
	machine->setCpuFreq(libspectrum_timings_processor_speed(LIBSPECTRUM_MACHINE_48));

	timer = new Timer();

	debugger = new Debugger();
	debugger->setEmulator(this);

	//speaker = new Speaker();
	//speaker->attach(machine, ula);
}

void Emulator::start()
{
	if (!running) {
		running = true;
		debugger->notifyListeners(DEBUGGER_EVENT_EMULATION_START);
		timer->start();

		speaker->start();
	}
}

void Emulator::stop()
{
	if (running) {
		timer->stop();
		running = false;
		debugger->notifyListeners(DEBUGGER_EVENT_EMULATION_STOP);

		speaker->stop();
	}
}

bool Emulator::isRunning()
{
	return running;
}

void Emulator::runUntil(uint64_t time)
{
	do {
		uint16_t pc = machine->getCpu()->getRegister(regPC);
		if (debugger->isBreakpoint(pc)) {
			stop();
			// TODO: zajistit, aby stopky neměly čas o jednu dávku v budoucnu
			// a po opětovném startu nedošlo k přeskočení určitého času
			break;
		}

		machine->step();
	} while (machine->getCurrentTime() < ((time * machine->getCpuFreq()) / 1000));
}

Debugger *Emulator::getDebugger() const
{
	return debugger;
}

Machine *Emulator::getMachine() const
{
	return machine;
}

Keyboard *Emulator::getKeyboard() const
{
	return keyboard;
}

Joystick *Emulator::getJoystick() const
{
	return joystick;
}

TapeRecorder *Emulator::getTapeRecorder() const
{
	return tapeRecorder;
}

Speaker *Emulator::getSpeaker() const
{
	return speaker;
}

void Emulator::renderScreen(uint32_t *buffer)
{
	if (ula != NULL) {
		ula->renderScreen(buffer);
	}
}


