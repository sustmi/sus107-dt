// Clock.cpp

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

#include "Clock.h"

Clock::Clock() {
	timer = new wxTimer(this, wxID_ANY);
	stopWatch = new wxStopWatch;
	stopWatch->Pause();
	Connect(timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(Clock::OnTimer));
}

Clock::~Clock() {
	timer->Stop();
	stopWatch->Pause();
	Disconnect(timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(Clock::OnTimer));

	delete timer;
	delete stopWatch;
}

void Clock::attach(Emulator *emulator)
{
	this->emulator = emulator;
}

void Clock::start(uint64_t period_ms, uint64_t time_ms)
{
	timer->Start(period_ms);
	stopWatch->Start(time_ms);
}

void Clock::resume()
{
	timer->Start();
	stopWatch->Resume();
}

void Clock::stop()
{
	timer->Stop();
	stopWatch->Pause();
}

uint64_t Clock::getTime()
{
	return stopWatch->Time();
}

void Clock::OnTimer(wxTimerEvent &event)
{
	emulator->runUntil(stopWatch->Time());
}



