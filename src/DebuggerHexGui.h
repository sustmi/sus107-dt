/*
 * DebuggerHexGui.h
 *
 *  Created on: 23.2.2012
 *      Author: mirek
 */

#ifndef DEBUGGERHEXGUI_H_
#define DEBUGGERHEXGUI_H_

#include "widgets/HexEditorCtrl/HexEditorCtrl.h"
#include "Debugger.h"
#include "DebuggerListener.h"

class Debugger;
class DebuggerListener;

class DebuggerHexGui: public HexEditorCtrl, public DebuggerListener {
public:
	DebuggerHexGui( wxWindow* parent, wxWindowID id = ID_DEFAULT, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
	~DebuggerHexGui();

	void uiUpdate();
	void setDebugger(Debugger *debugger);
	void debuggerEvent(DebuggerEvent event);

	void OnOffsetScroll(wxScrollEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnKeyboardChar(wxKeyEvent& event);
	void OnKeyboardInput(wxKeyEvent& event);

	void UpdateOffsetScroll();
	void LoadFromOffset(int64_t position, bool cursor_reset = false, bool paint = true);

private:
	Debugger *debugger;
};

#endif /* DEBUGGERHEXGUI_H_ */
