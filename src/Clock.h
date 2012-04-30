// Clock.h

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
