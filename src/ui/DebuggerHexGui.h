/*
 * DebuggerHexGui.h
 *
 *  Created on: 23.2.2012
 *      Author: Miroslav Sustek <sus107@vsb.cz>
 */

#ifndef DEBUGGERHEXGUI_H_
#define DEBUGGERHEXGUI_H_

#include "widgets/HexEditorCtrl/HexEditorCtrl.h"
#include "../Debugger.h"
#include "../DebuggerListener.h"

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
	void OnMouseWheel(wxMouseEvent& event);
	void OnMouseRight(wxMouseEvent& event);
	void OnKeyboardSelector(wxKeyEvent& event);
	void OnMenuEvent(wxCommandEvent& event);

	void UpdateOffsetScroll();
	void LoadFromOffset(int64_t position, bool cursor_reset = false, bool paint = true);

	void CopySelection();
	void PasteFromClipboard();

private:
	Debugger *debugger;
};

#endif /* DEBUGGERHEXGUI_H_ */
