/*
 * TapeRecorder.h
 *
 *  Created on: 9.1.2012
 *      Author: mirek
 */

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
