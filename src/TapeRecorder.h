// TapeRecorder.h

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

#ifndef TAPERECORDER_H_
#define TAPERECORDER_H_

#include <libspectrum.h>

#include "Machine.h"

// forward declaration
class Machine;

enum TapeRecorderState {
	TAPE_RECORDER_STATE_EMPTY = 0,
	TAPE_RECORDER_STATE_STOPPED,
	TAPE_RECORDER_STATE_PLAYING,
};

class TapeRecorder {
public:
	TapeRecorder();
	virtual ~TapeRecorder();

	void attach(Machine *machine);

	void load(const char *filename);
	void play();
	void stop();
	void rewind();

	int getSignal();

private:
	uint64_t getPosition();

	Machine *machine;

	uint64_t lastPosition;
	uint64_t lastPlayAt;

	int flags;
	uint64_t flagsAtPosition;

	int lastFlags;
	uint64_t lastFlagsAtPosition;

	TapeRecorderState state;

	int signal;

	libspectrum_tape *tape;


};

#endif /* TAPERECORDER_H_ */
