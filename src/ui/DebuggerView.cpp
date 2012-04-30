// DebuggerView.cpp

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

// -*- C++ -*- generated by wxGlade HG on Fri Feb  3 02:55:31 2012

#include "DebuggerView.h"

// begin wxGlade: ::extracode

// end wxGlade


DebuggerView::DebuggerView(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
	wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
	// begin wxGlade: DebuggerView::DebuggerView
	notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	notebook_pane_hex = new wxPanel(notebook, wxID_ANY);
	debugger_menubar = new wxMenuBar();
	wxMenu* wxglade_tmp_menu_1 = new wxMenu();
	wxglade_tmp_menu_1->Append(301, _("Registers"), wxEmptyString, wxITEM_NORMAL);
	debugger_menubar->Append(wxglade_tmp_menu_1, _("View"));
	SetMenuBar(debugger_menubar);
	hex_view = new DebuggerHexGui(notebook_pane_hex, wxID_ANY);
	debugger_code_view = new DebuggerCodeGui(notebook, wxID_ANY);
	button_step = new wxButton(this, 201, _("Step"));
	button_continue = new wxButton(this, 202, _("Continue"));
	button_break = new wxButton(this, 203, _("Break"));

	set_properties();
	do_layout();
	// end wxGlade

	debugger = NULL;
	//hex_view->Connect(wxEVT_STC_UPDATEUI, (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxStyledTextEventFunction, &DebuggerView::OnHexViewModified) , NULL, this);
}

DebuggerView::~DebuggerView()
{
	if (debugger) {
		debugger->removeListener(this);
	}
}

void DebuggerView::debuggerEvent(DebuggerEvent event)
{
	if (event == DEBUGGER_EVENT_EMULATION_START ||
		event == DEBUGGER_EVENT_EMULATION_STOP ||
		event == DEBUGGER_EVENT_EMULATION_STEP)
	{
		uiUpdate();
	}
}

void DebuggerView::setDebugger(Debugger *debugger)
{
    this->debugger = debugger;
    debugger_code_view->setDebugger(debugger);
    hex_view->setDebugger(debugger);

    debugger->addListener(this);
}

BEGIN_EVENT_TABLE(DebuggerView, wxFrame)
	// begin wxGlade: DebuggerView::event_table
	EVT_MENU(301, DebuggerView::OnViewRegisters)
	EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, DebuggerView::OnDebuggerNotebookPageChanged)
	EVT_BUTTON(201, DebuggerView::OnDebuggerStep)
	EVT_BUTTON(202, DebuggerView::OnDebuggerContinue)
	EVT_BUTTON(203, DebuggerView::OnDebuggerBreak)
	// end wxGlade
END_EVENT_TABLE();

void DebuggerView::uiUpdate()
{
	if (debugger->getEmulator()->isRunning()) {
		button_step->Disable();
		button_continue->Disable();
		button_break->Enable();
	} else {
		button_step->Enable();
		button_continue->Enable();
		button_break->Disable();
	}
}

void DebuggerView::OnDebuggerStep(wxCommandEvent &event)
{
	debugger->stepInstruction();
}

void DebuggerView::OnDebuggerContinue(wxCommandEvent &event)
{
	debugger->emulationContinue();
}

void DebuggerView::OnDebuggerBreak(wxCommandEvent &event)
{
	debugger->emulationBreak();
}

void DebuggerView::OnViewRegisters(wxCommandEvent &event)
{
	DebuggerRegistersView *registersView = new DebuggerRegistersView(this, wxID_ANY, wxEmptyString);
	registersView->setDebugger(debugger);
	registersView->Show(true);
	registersView->uiUpdate();
	debugger->addListener(registersView);
}

void DebuggerView::OnHexViewModified(wxStyledTextEvent &event)
{
	printf("store: %d -%s-\n", event.GetPosition(), event.GetText().mb_str().data());

	/*if (hex_view->GetCurrentPos() % 3 == 2) {
		hex_view->CharRight();
		//hex_view->Get
	}*/
}

void DebuggerView::OnDebuggerNotebookPageChanged(wxNotebookEvent & event)
{
	if (event.GetSelection() == 1) {
		debugger_code_view->uiUpdate();
	}
}

// wxGlade: add DebuggerView event handlers


void DebuggerView::set_properties()
{
	// begin wxGlade: DebuggerView::set_properties
	SetTitle(_("Debugger"));
	SetSize(wxSize(430, 480));
	// end wxGlade

	char buf[65536];
	for (int i = 0; i < 65536; i++) {
		buf[i] = (i % ('z' - 'a')) + 'a';
	}

	hex_view->ReadFromBuffer(0, 65536, buf);
	//hex_view->ReadFromBuffer(16, 62, "qwertzuioplkjhgfdsayxcvbnm0123456789QWERTZUIOPLKJHGFDSAYXCVBNM");
	//hex_view->Select(5, 8); // data selection
	printf("%d\n", hex_view->GetByteCount());
	printf("%d\n", hex_view->GetLastPosition());

}

void DebuggerView::do_layout()
{
	// begin wxGlade: DebuggerView::do_layout
	wxBoxSizer* debugger_panes = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* buttons = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
	sizer_1->Add(hex_view, 1, wxEXPAND, 0);
	notebook_pane_hex->SetSizer(sizer_1);
	notebook->AddPage(notebook_pane_hex, _("Hex"));
	notebook->AddPage(debugger_code_view, _("Code"));
	debugger_panes->Add(notebook, 1, wxEXPAND, 0);
	buttons->Add(button_step, 0, wxADJUST_MINSIZE, 0);
	buttons->Add(button_continue, 0, wxADJUST_MINSIZE, 0);
	buttons->Add(button_break, 0, wxADJUST_MINSIZE, 0);
	debugger_panes->Add(buttons, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxADJUST_MINSIZE, 0);
	SetSizer(debugger_panes);
	Layout();
	// end wxGlade
}

