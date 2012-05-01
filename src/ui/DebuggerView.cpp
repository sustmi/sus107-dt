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
	debugger_view_toolbar = new wxToolBar(this, -1);
	SetToolBar(debugger_view_toolbar);
	debugger_view_toolbar->SetToolBitmapSize(wxSize(24, 24));
	debugger_view_toolbar->AddTool(DEBUGGER_TOOL_CONTINUE, _("Continue"), (*_img_media_playback_start_4), wxNullBitmap, wxITEM_NORMAL, _("Continue program execution"), wxEmptyString);
	debugger_view_toolbar->AddTool(DEBUGGER_TOOL_BREAK, _("Break"), (*_img_media_playback_pause_4), wxNullBitmap, wxITEM_NORMAL, _("Break program"), wxEmptyString);
	debugger_view_toolbar->AddTool(DEBUGGER_TOOL_STEP, _("Step"), (*_img_debug_step_into_instruction), wxNullBitmap, wxITEM_NORMAL, _("Step instruction"), wxEmptyString);
	debugger_view_toolbar->Realize();
	debugger_code_view = new DebuggerCodeGui(notebook, wxID_ANY);
	hex_view = new DebuggerHexGui(notebook_pane_hex, wxID_ANY);

	set_properties();
	do_layout();
	// end wxGlade

	emulator = NULL;
	debugger = NULL;
	//hex_view->Connect(wxEVT_STC_UPDATEUI, (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxStyledTextEventFunction, &DebuggerView::OnHexViewModified) , NULL, this);
}

DebuggerView::~DebuggerView()
{
	if (emulator) {
		emulator->removeListener(this);
	}
	if (debugger) {
		debugger->removeListener(this);
	}
}

void DebuggerView::emulatorEvent(EmulatorEvent event)
{
	if (event == EMULATOR_EVENT_EMULATION_START ||
		event == EMULATOR_EVENT_EMULATION_STOP ||
		event == EMULATOR_EVENT_EMULATION_STEP)
	{
		uiUpdate();
	}
}

void DebuggerView::debuggerEvent(DebuggerEvent event)
{
	// nothing ?
}

void DebuggerView::attach(Emulator *emulator, Debugger *debugger)
{
    this->emulator = emulator;
    this->debugger = debugger;
    debugger_code_view->attach(emulator, debugger);
    hex_view->attach(emulator, debugger);

    emulator->addListener(this);
    debugger->addListener(this);
}

BEGIN_EVENT_TABLE(DebuggerView, wxFrame)
	// begin wxGlade: DebuggerView::event_table
	EVT_MENU(301, DebuggerView::OnViewRegisters)
	EVT_TOOL(DEBUGGER_TOOL_CONTINUE, DebuggerView::OnDebuggerContinue)
	EVT_TOOL(DEBUGGER_TOOL_BREAK, DebuggerView::OnDebuggerBreak)
	EVT_TOOL(DEBUGGER_TOOL_STEP, DebuggerView::OnDebuggerStep)
	EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, DebuggerView::OnDebuggerNotebookPageChanged)
	// end wxGlade
END_EVENT_TABLE();

void DebuggerView::uiUpdate()
{
	if (debugger->getEmulator()->isRunning()) {
		GetToolBar()->EnableTool(DEBUGGER_TOOL_CONTINUE, false);
		GetToolBar()->EnableTool(DEBUGGER_TOOL_BREAK, true);
		GetToolBar()->EnableTool(DEBUGGER_TOOL_STEP, false);
	} else {
		GetToolBar()->EnableTool(DEBUGGER_TOOL_CONTINUE, true);
		GetToolBar()->EnableTool(DEBUGGER_TOOL_BREAK, false);
		GetToolBar()->EnableTool(DEBUGGER_TOOL_STEP, true);
	}

	wxIcon();
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
	registersView->attach(emulator, debugger);
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
	if (event.GetSelection() == 0) {
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
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
	sizer_1->Add(hex_view, 1, wxEXPAND, 0);
	notebook_pane_hex->SetSizer(sizer_1);
	notebook->AddPage(debugger_code_view, _("Code"));
	notebook->AddPage(notebook_pane_hex, _("Hex"));
	debugger_panes->Add(notebook, 1, wxEXPAND, 0);
	SetSizer(debugger_panes);
	Layout();
	// end wxGlade
}

