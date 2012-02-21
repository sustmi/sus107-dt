/*
 * DebuggerListener.h
 *
 *  Created on: 21.2.2012
 *      Author: mirek
 */

#ifndef DEBUGGERLISTENER_H_
#define DEBUGGERLISTENER_H_

enum DebuggerEvent {
	DEBUGGER_EVENT_REGISTERS_CHANGED,
	DEBUGGER_EVENT_MEMORY_CHANGED,
	DEBUGGER_EVENT_EMULATION_START,
	DEBUGGER_EVENT_EMULATION_STOP,
	DEBUGGER_EVENT_EMULATION_STEP,
	DEBUGGER_EVENT_BREAKPOINTS_CHANGED,
};

class DebuggerListener {
public:
	DebuggerListener();
	virtual ~DebuggerListener();

	virtual void debuggerEvent(DebuggerEvent event) = 0;
};

#endif /* DEBUGGERLISTENER_H_ */
