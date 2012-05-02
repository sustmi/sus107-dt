// TapeRecorder.cpp

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

#include "TapeRecorder.h"

TapeRecorder::TapeRecorder() {
	tape = libspectrum_tape_alloc();
	state = TAPE_RECORDER_STATE_EMPTY;
}

TapeRecorder::~TapeRecorder() {
	libspectrum_tape_free(tape);
}

void TapeRecorder::attach(Machine *machine) {
	this->machine = machine;
}

void TapeRecorder::load(const char *filename) {
	FILE *f = fopen(filename, "rb");
	fseek(f, 0 , SEEK_END);
	long fsize = ftell(f);
	::rewind(f);

	uint8_t *buffer = new uint8_t[fsize];
	fread(buffer, sizeof(char), fsize, f);
	fclose(f);

	libspectrum_tape_clear(tape);
	libspectrum_tape_read(tape, buffer, fsize, LIBSPECTRUM_ID_UNKNOWN, filename);

	delete buffer;

	lastPosition = 0;
	flags = 0;
	flagsAtPosition = 0;
	state = TAPE_RECORDER_STATE_STOPPED;
	signal = 0;
}

void TapeRecorder::play() {
	if (state == TAPE_RECORDER_STATE_STOPPED) {
		lastPlayAt = machine->getCurrentTime();
		state = TAPE_RECORDER_STATE_PLAYING;
	}
}

void TapeRecorder::stop() {
	if (state == TAPE_RECORDER_STATE_PLAYING) {
		lastPosition += machine->getCurrentTime() - lastPlayAt;
		state = TAPE_RECORDER_STATE_STOPPED;
	}
}

void TapeRecorder::rewind() {
	if (state != TAPE_RECORDER_STATE_EMPTY) {
		lastPosition = 0;
		flagsAtPosition = 0;
		lastPlayAt = machine->getCurrentTime();
		libspectrum_tape_nth_block(tape, 0);
	}
}

int TapeRecorder::getSignal() {
	uint64_t position = getPosition();

	while (position > flagsAtPosition && !(flags & LIBSPECTRUM_TAPE_FLAGS_TAPE)) {
		lastFlags = flags;
		lastFlagsAtPosition = flagsAtPosition;

		libspectrum_dword tstates = 0;
		int err = libspectrum_tape_get_next_edge(&tstates, &flags, tape);
		flagsAtPosition += tstates;

		if (!(flags & LIBSPECTRUM_TAPE_FLAGS_NO_EDGE)) {
			if (signal == 1) {
				signal = 0;
			} else {
				signal = 1;
			}
		}
	}

	if (lastFlags & LIBSPECTRUM_TAPE_FLAGS_LEVEL_LOW) {
		signal = 0;
	} else if (lastFlags & LIBSPECTRUM_TAPE_FLAGS_LEVEL_HIGH) {
		signal = 1;
	}

	return signal;
}

uint64_t TapeRecorder::getPosition() {
	if (state == TAPE_RECORDER_STATE_PLAYING) {
		return lastPosition + (machine->getCurrentTime() - lastPlayAt);
	}

	return lastPosition;
}

TapeRecorderState TapeRecorder::getState()
{
	return state;
}




