/*
 * Clock.h
 *
 *  Created on: 30.4.2012
 *      Author: mirek
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include <wx/timer.h>
#include <wx/stopwatch.h>

#include "Emulator.h"

class Emulator;

class Clock : wxEvtHandler {
public:
	Clock();
	virtual ~Clock();

	void attach(Emulator *emulator);

	void start(uint64_t period_ms, uint64_t time_ms);
	void resume();
	void stop();

	uint64_t getTime();

	void OnTimer(wxTimerEvent &event);

private:
	Emulator *emulator;

	wxTimer *timer;
	wxStopWatch *stopWatch;
};

#endif /* CLOCK_H_ */
