// -*- C++ -*- generated by wxGlade HG on Fri Feb  3 02:55:31 2012

#include "DebuggerView.h"

// begin wxGlade: ::extracode

// end wxGlade


DebuggerView::DebuggerView(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
	wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
	// begin wxGlade: DebuggerView::DebuggerView
	notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	notebook_pane_code = new wxPanel(notebook, wxID_ANY);
	debugger_menubar = new wxMenuBar();
	wxMenu* wxglade_tmp_menu_1 = new wxMenu();
	wxglade_tmp_menu_1->Append(wxID_ANY, wxT("Registers"), wxEmptyString, wxITEM_NORMAL);
	debugger_menubar->Append(wxglade_tmp_menu_1, wxT("View"));
	SetMenuBar(debugger_menubar);
	notebook_pane_hex = new wxPanel(notebook, wxID_ANY);
	code_grid = new wxGrid(notebook_pane_code, wxID_ANY);
	code_slider = new wxSlider(notebook_pane_code, wxID_ANY, 0, 0, 65535, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL|wxSL_LEFT|wxSL_TOP);
	button_step = new wxButton(this, 201, wxT("Step"));
	button_continue = new wxButton(this, 202, wxT("Continue"));
	button_break = new wxButton(this, 203, wxT("Break"));

	set_properties();
	do_layout();
	// end wxGlade

	address = 0;
}

void DebuggerView::setDebugger(Debugger *debugger)
{
    this->debugger = debugger;
}

BEGIN_EVENT_TABLE(DebuggerView, wxFrame)
	// begin wxGlade: DebuggerView::event_table
	EVT_MENU(wxID_ANY, DebuggerView::OnViewRegisters)
	EVT_COMMAND_SCROLL_ENDSCROLL(wxID_ANY, DebuggerView::OnCodeSliderEndScroll)
	EVT_COMMAND_SCROLL_THUMBTRACK(wxID_ANY, DebuggerView::OnCodeSliderEndScroll)
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

	code_slider->SetValue(address);

	char buf[1024];
	int dummy;
	int addr = address;

	for (int i = 0; i < code_grid->GetRows(); i++) {
		int len = debugger->dasm(buf, 1024, 0, &dummy, &dummy, addr);

		code_grid->SetCellValue(i, 0, wxString::Format(wxT("%04x"), addr));
		code_grid->SetCellValue(i, 1, wxString::Format(wxT("%s"), wxString::From8BitData(buf).c_str()));

		addr += len;
	}
}

void DebuggerView::OnDebuggerStep(wxCommandEvent &event)
{
	debugger->getEmulator()->getMachine()->stepInstruction();
	address = debugger->getEmulator()->getMachine()->getPC();
	debugger->uiUpdate();
}


void DebuggerView::OnDebuggerContinue(wxCommandEvent &event)
{
	debugger->getEmulator()->start();
}


void DebuggerView::OnDebuggerBreak(wxCommandEvent &event)
{
	debugger->getEmulator()->stop();
	address = debugger->getEmulator()->getMachine()->getCpu()->getRegister(regPC);
	debugger->uiUpdate();
}

void DebuggerView::OnViewRegisters(wxCommandEvent &event)
{
	debugger->uiShowRegisters();
}

void DebuggerView::OnCodeSliderEndScroll(wxScrollEvent & event)
{
	address = code_slider->GetValue();
	uiUpdate();
}


// wxGlade: add DebuggerView event handlers


void DebuggerView::set_properties()
{
	// begin wxGlade: DebuggerView::set_properties
	SetTitle(wxT("Debugger"));
	code_grid->CreateGrid(16, 2);
	code_grid->SetRowLabelSize(0);
	code_grid->EnableEditing(false);
	code_grid->EnableGridLines(false);
	code_grid->EnableDragColSize(false);
	code_grid->EnableDragRowSize(false);
	code_grid->EnableDragGridSize(false);
	code_grid->SetColLabelValue(0, wxT("Address"));
	code_grid->SetColSize(0, 70);
	code_grid->SetColLabelValue(1, wxT("Instruction"));
	code_grid->SetColSize(1, 200);
	// end wxGlade
}


void DebuggerView::do_layout()
{
	// begin wxGlade: DebuggerView::do_layout
	wxBoxSizer* debugger_panes = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* buttons = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* code_sizer = new wxBoxSizer(wxHORIZONTAL);
	code_sizer->Add(code_grid, 2, wxEXPAND, 0);
	code_sizer->Add(code_slider, 0, wxEXPAND, 0);
	notebook_pane_code->SetSizer(code_sizer);
	notebook->AddPage(notebook_pane_hex, wxT("Hex"));
	notebook->AddPage(notebook_pane_code, wxT("Code"));
	debugger_panes->Add(notebook, 1, wxEXPAND, 0);
	buttons->Add(button_step, 0, wxADJUST_MINSIZE, 0);
	buttons->Add(button_continue, 0, wxADJUST_MINSIZE, 0);
	buttons->Add(button_break, 0, wxADJUST_MINSIZE, 0);
	debugger_panes->Add(buttons, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL|wxADJUST_MINSIZE, 0);
	SetSizer(debugger_panes);
	debugger_panes->Fit(this);
	Layout();
	// end wxGlade
}
