/*
 * Timer.h
 *
 *  Created on: 10.1.2012
 *      Author: mirek
 */

#ifndef TIMER_H_
#define TIMER_H_

class Timer {
public:
	Timer();
	virtual ~Timer();

	void setTimeout(long miliseconds);
	void setCallback();
	void start();
	void stop();

private:
	long timeout;
	bool running;

};

#endif /* TIMER_H_ */
