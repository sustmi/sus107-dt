// -*- C++ -*- generated by wxGlade HG on Fri Feb  3 02:55:31 2012

#include <wx/wx.h>
#include <wx/image.h>

#ifndef DEBUGGERVIEW_H
#define DEBUGGERVIEW_H

#include "Debugger.h"

// begin wxGlade: ::dependencies
#include <wx/notebook.h>
#include <wx/grid.h>
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade

class Debugger;

class DebuggerView: public wxFrame {
public:
	// begin wxGlade: DebuggerView::ids
	// end wxGlade

	DebuggerView(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);
	void uiUpdate();

private:
	// begin wxGlade: DebuggerView::methods
	void set_properties();
	void do_layout();
	// end wxGlade

	Debugger *debugger;
	int address;

protected:
	// begin wxGlade: DebuggerView::attributes
	wxMenuBar* debugger_menubar;
	wxPanel* notebook_pane_hex;
	wxGrid* code_grid;
	wxSlider* code_slider;
	wxPanel* notebook_pane_code;
	wxNotebook* notebook;
	wxButton* button_step;
	wxButton* button_continue;
	wxButton* button_break;
	// end wxGlade

	DECLARE_EVENT_TABLE();

public:
	virtual void OnDebuggerStep(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnDebuggerContinue(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnDebuggerBreak(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnViewRegisters(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnCodeSliderEndScroll(wxScrollEvent &event); // wxGlade: <event_handler>
    void setDebugger(Debugger *debugger);
}; // wxGlade: end class


#endif // DEBUGGERVIEW_H