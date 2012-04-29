/*
 * Ula.cpp
 *
 *  Created on: 8.12.2011
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#include <stdio.h>
#include "Ula.h"
#include "Machine.h"
#include "Cpu.h"
#include "Memory.h"

Ula::Ula() {
	// TODO Auto-generated constructor stub
	borderColor = 0;
	lastInterruptTime = 0;
}

Ula::~Ula() {
	// TODO Auto-generated destructor stub
}

void Ula::attach(Machine *machine, Memory *memory, Cpu *cpu, Keyboard *keyboard,
		TapeRecorder *tapeRecorder, Speaker *speaker)
{
	this->machine = machine;
	this->memory = memory;
	this->cpu = cpu;
	this->keyboard = keyboard;
	this->tapeRecorder = tapeRecorder;
	this->speaker = speaker;
}

bool Ula::getIntLineState() {
	int currentTime = machine->getCurrentTime();

	// make sure that: lastInterruptTime <= currentTime < nextInterruptTime
	if (currentTime >= lastInterruptTime + interruptPeriod) {
		lastInterruptTime += interruptPeriod * ((currentTime - lastInterruptTime) / interruptPeriod);
	}

	if (currentTime < lastInterruptTime + interruptLength) {
		return true;
	} else {
		return false;
	}
}

void Ula::setInterruptLength(uint64_t interruptLength)
{
	this->interruptLength = interruptLength;
}

void Ula::setInterruptPeriod(uint64_t interruptPeriod)
{
	this->interruptPeriod = interruptPeriod;
}

void Ula::setLastInterruptTime(uint64_t lastInterruptTime)
{
	this->lastInterruptTime = lastInterruptTime;
}

uint8_t Ula::mread(uint16_t addr) {
	contendedMemoryDelay(addr);
	return memory->rawRead(addr);
}
void Ula::mwrite(uint16_t addr, uint8_t value) {
	contendedMemoryDelay(addr);
	memory->rawWrite(addr, value);
}

void Ula::contendedMemoryDelay(uint16_t addr)
{
	// delays apply only for addresses between 0x4000 and 0x7fff
	if (addr >= 0x4000 && addr <= 0x7fff) {
		uint64_t tstatesSinceInterrupt = (machine->getCurrentTime()) - lastInterruptTime;
		contendedDelay(tstatesSinceInterrupt);
	}
}

void Ula::contendedPortDelay(uint16_t port)
{
	uint64_t tstatesSinceInterrupt = (machine->getCurrentTime()) - lastInterruptTime -1;
	if (port >= 0x4000 && port <= 0x7fff) {
		if (port & 0x1) {
			tstatesSinceInterrupt += contendedDelay(tstatesSinceInterrupt);
			tstatesSinceInterrupt += contendedDelay(tstatesSinceInterrupt + 1);
			tstatesSinceInterrupt += contendedDelay(tstatesSinceInterrupt + 1);
			contendedDelay(tstatesSinceInterrupt + 1);
		} else {
			tstatesSinceInterrupt += contendedDelay(tstatesSinceInterrupt);
			contendedDelay(tstatesSinceInterrupt + 1);
		}
	} else {
		if (port & 0x1) {
			// no delay
		} else {
			contendedDelay(tstatesSinceInterrupt + 1);
		}
	}
}

int Ula::contendedDelay(uint64_t tstates)
{
	int ret = 0;
	// Delays start one T-state before first non-border pixel is drawn on the screen.
	// Upper border consists of 64 lines, each 224 T-states long.
	// Last line where delays apply is 256th (64 for upper border + 192 for picture data).
	if ((tstates + 1) / 224 >= 64 && (tstates + 1) / 224 < 64+192) {
		// Picture data are read in first 128 T-states of the line
		if ((tstates + 1) % 224 < 128) {
			int delay = 6 - ((tstates + 1) % 224) % 8;
			if (delay > 0) {
				cpu->wait(delay);
				ret = delay;
			}
			//printf("%d, %d\n", tstates, delay);
		}
	}
	return ret;
}

void Ula::pread(uint16_t port, uint8_t *value) {
	contendedPortDelay(port);

	if ((port & 0x00ff) == 0x00fe) {
		if (tapeRecorder != NULL) {
			*value = (*value & ~(1 << 6)) | ((tapeRecorder->getSignal() & 0x1) << 6);
		}
		if (keyboard != NULL) {
			*value = (*value & ~(0x1f)) | (keyboard->getKbData((port & 0xff00) >> 8) & 0x1f);
		}
	}
}
void Ula::pwrite(uint16_t port, uint8_t value) {
	contendedPortDelay(port);

	if ((port & 0x00ff) == 0x00fe) {
		borderColor = value & 0x07;

		// sound
		int ear = (value & (1 << 4)) != 0;
		if (speaker != NULL) {
			speaker->setSignal(ear ? 0.8 : -0.8);
		}
	}
}

void Ula::renderScreen(uint32_t *buffer)
{
	static uint32_t palette_rgb[16] = {
		0x000000, // black
		0x0000c0, // blue
		0xc00000, // red
		0xc000c0, // purple
		0x00c000, // green
		0x00c0c0, // cyan
		0xc0c000, // yellow
		0xc0c0c0, // white
		// bright
		0x000000, // black
		0x0000ff, // blue
		0xff0000, // red
		0xff00ff, // purple
		0x00ff00, // green
		0x00ffff, // cyan
		0xffff00, // yellow
		0xffffff, // white
	};

	for (int y = 0; y < 288; y++) {
		for (int x = 0; x < 352; x++) {
			buffer[y*352 + x] = palette_rgb[borderColor];
		}
	}

	// swap INK and PAPER every 16 half frames
	int flash_phase = ((machine->getCurrentTime() / this->interruptPeriod) / 16) % 2;

	for (int y = 0; y < 192; y++) {
		for (int x = 0; x < 256; x++) {
			int third = y / 64;
			int line = y % 8;
			int row_in_third = (y / 8) % 8;
			int row = y / 8;
			int column = x / 8;

			uint16_t painting_addr = 0x4000 + (third << 11) + (line << 8) + (row_in_third << 5) + column;
			uint16_t attribute_addr = 0x5800 + (row << 5) + column;
			
			uint8_t paint = memory->rawRead(painting_addr);
			uint8_t attr = memory->rawRead(attribute_addr);

			int ink = ((attr & 0x40) >> 3) | (attr & 0x07);
			int paper = (attr & 0x78) >> 3;
			bool flash = (attr & 0x80) && flash_phase == 1;

			int color = (((paint & (0x80 >> (x%8))) != 0) != flash) ? ink : paper;

			buffer[(y+48)*352 + x+48] = palette_rgb[color];
		}
	}
}

uint64_t Ula::getTimeSinceLastInterrupt() {
	return machine->getCurrentTime() - lastInterruptTime;
}
