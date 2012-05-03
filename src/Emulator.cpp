// Emulator.cpp

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

#include "Emulator.h"

Emulator::Emulator() {
	ula = NULL;
}

Emulator::~Emulator() {
	delete debugger;
	clock->stop();
	delete machine;
	delete tapeRecorder;
	delete keyboard;
	delete joystick;
	delete speaker;
	delete memory;
	delete cpu;
	delete ula;
	delete ports;
}

void Emulator::addListener(EmulatorListener *listener)
{
	listeners.insert(listener);
}

void Emulator::removeListener(EmulatorListener *listener)
{
	listeners.erase(listener);
}

void Emulator::notifyListeners(EmulatorEvent event)
{
	std::set<EmulatorListener*>::iterator it;
	for (it = listeners.begin(); it != listeners.end(); it++) {
		(*it)->emulatorEvent(event);
	}
}

void Emulator::init()
{
	running = false;

	memory = new Memory();
	cpu = new Cpu();
	ula = new Ula();
	ports = new Ports();

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

	// Spectrum 48k machine specifics
	ula->setInterruptLength(libspectrum_timings_interrupt_length(LIBSPECTRUM_MACHINE_48));
	ula->setInterruptPeriod(libspectrum_timings_tstates_per_frame(LIBSPECTRUM_MACHINE_48));
	machine->setCpuFreq(libspectrum_timings_processor_speed(LIBSPECTRUM_MACHINE_48));

	clock = new Clock();
	clock->attach(this);

	debugger = new Debugger();
	debugger->setEmulator(this);
}

void Emulator::start()
{
	if (!running) {
		running = true;
		notifyListeners(EMULATOR_EVENT_EMULATION_START);

		// make stopwatch start with current emulation time
		clock->start(20, 1000 * machine->getCurrentTime() / machine->getCpuFreq());
		speaker->start();
	}
}

void Emulator::stop()
{
	if (running) {
		clock->stop();
		running = false;
		notifyListeners(EMULATOR_EVENT_EMULATION_STOP);

		speaker->stop();
	}
}

bool Emulator::isRunning()
{
	return running;
}

void Emulator::runUntil(uint64_t time_ms)
{
	do {
		uint16_t pc = machine->getCpu()->getRegister(regPC);
		if (debugger->isBreakpoint(pc)) {
			stop();
			break;
		}

		machine->step();
	} while (machine->getCurrentTime() < ((time_ms * machine->getCpuFreq()) / 1000));
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


