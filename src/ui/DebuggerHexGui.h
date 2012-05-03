// DebuggerHexGui.h

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

#ifndef DEBUGGERHEXGUI_H_
#define DEBUGGERHEXGUI_H_

#include <iostream>
#include <fstream>
#include <wx/filedlg.h>
#include <wx/wfstream.h>

#include "widgets/HexEditorCtrl/HexEditorCtrl.h"
#include "../Emulator.h"
#include "../EmulatorListener.h"
#include "../Debugger.h"
#include "../DebuggerListener.h"

class Emulator;
class EmulatorListener;
class Debugger;
class DebuggerListener;

const int DEBUGGER_SHOW_IN_CODEVIEW = wxID_HIGHEST + 5002;

class DebuggerHexGui: public HexEditorCtrl, public EmulatorListener, public DebuggerListener {
public:
	DebuggerHexGui( wxWindow* parent, wxWindowID id = ID_DEFAULT, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
	~DebuggerHexGui();

	void uiUpdate();
	void attach(Emulator *emulator, Debugger *debugger);
	void emulatorEvent(EmulatorEvent event);
	void debuggerEvent(DebuggerEvent event);

	void OnOffsetScroll(wxScrollEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnKeyboardChar(wxKeyEvent& event);
	void OnKeyboardInput(wxKeyEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnMouseRight(wxMouseEvent& event);
	void OnKeyboardSelector(wxKeyEvent& event);
	void OnMenuEvent(wxCommandEvent& event);
	void OnSaveAsFile(wxCommandEvent& event);

	void UpdateOffsetScroll();
	void LoadFromOffset(int64_t position, bool cursor_reset = false, bool paint = true);
	void MakeAddressVisible(int64_t position);

	void CopySelection();
	void PasteFromClipboard();

private:
	Emulator *emulator;
	Debugger *debugger;

	wxFileDialog* saveFileDialog;

protected:
	DECLARE_EVENT_TABLE()
};

#endif /* DEBUGGERHEXGUI_H_ */
