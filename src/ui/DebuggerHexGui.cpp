// DebuggerHexGui.cpp

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

#include "DebuggerHexGui.h"

DebuggerHexGui::DebuggerHexGui(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
	HexEditorCtrl(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	emulator = NULL;
	debugger = NULL;

	saveFileDialog = NULL;

	offset_scroll->Enable(true);
	offset_ctrl->SetOffsetLimit(65536);
	offset_ctrl->offset_mode = 'X';
	offset_scroll_real->SetMinSize( wxSize( 20, 0 ));

	hex_ctrl->Connect(wxEVT_CHAR, wxKeyEventHandler(DebuggerHexGui::OnKeyboardChar), NULL, this);
	text_ctrl->Connect(wxEVT_CHAR, wxKeyEventHandler(DebuggerHexGui::OnKeyboardChar), NULL, this);
	hex_ctrl->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(DebuggerHexGui::OnKeyboardInput), NULL, this);
	text_ctrl->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(DebuggerHexGui::OnKeyboardInput), NULL, this);
	hex_ctrl->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(DebuggerHexGui::OnMouseWheel), NULL, this);
	text_ctrl->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(DebuggerHexGui::OnMouseWheel), NULL, this);
	hex_ctrl->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(DebuggerHexGui::OnMouseRight), NULL, this);
	text_ctrl->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(DebuggerHexGui::OnMouseRight), NULL, this);

	this->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(DebuggerHexGui::OnMenuEvent));
}

DebuggerHexGui::~DebuggerHexGui() {
	if (emulator) {
		emulator->removeListener(this);
	}
	if (debugger) {
		debugger->removeListener(this);
	}
	if (saveFileDialog) {
		delete saveFileDialog;
	}

	hex_ctrl->Disconnect(wxEVT_CHAR, wxKeyEventHandler(DebuggerHexGui::OnKeyboardChar), NULL, this);
	text_ctrl->Disconnect(wxEVT_CHAR, wxKeyEventHandler(DebuggerHexGui::OnKeyboardChar), NULL, this);
	hex_ctrl->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(DebuggerHexGui::OnKeyboardInput), NULL, this);
	text_ctrl->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(DebuggerHexGui::OnKeyboardInput), NULL, this);
	hex_ctrl->Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(DebuggerHexGui::OnMouseWheel), NULL, this);
	text_ctrl->Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(DebuggerHexGui::OnMouseWheel), NULL, this);
	hex_ctrl->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(DebuggerHexGui::OnMouseRight), NULL, this);
	text_ctrl->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(DebuggerHexGui::OnMouseRight), NULL, this);

	this->Disconnect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(DebuggerHexGui::OnMenuEvent));
}

BEGIN_EVENT_TABLE(DebuggerHexGui, HexEditorCtrl)
	EVT_MENU(wxID_SAVEAS, DebuggerHexGui::OnSaveAsFile)
END_EVENT_TABLE();

void DebuggerHexGui::OnOffsetScroll(wxScrollEvent & event)
{
	LoadFromOffset(event.GetPosition() * BytePerLine());
}

void DebuggerHexGui::attach(Emulator *emulator, Debugger *debugger)
{
	this->emulator = emulator;
	this->debugger = debugger;
	emulator->addListener(this);
	debugger->addListener(this);
}

void DebuggerHexGui::emulatorEvent(EmulatorEvent event)
{
	if (event == EMULATOR_EVENT_MEMORY_CHANGED ||
		event == EMULATOR_EVENT_EMULATION_STEP ||
		event == EMULATOR_EVENT_EMULATION_STOP)
	{
		uiUpdate();
	}
}

void DebuggerHexGui::debuggerEvent(DebuggerEvent event)
{
	// nothing?
}

void DebuggerHexGui::uiUpdate()
{
	LoadFromOffset(page_offset, false, true);
	UpdateOffsetScroll();
}

void DebuggerHexGui::OnResize(wxSizeEvent &event)
{
	HexEditorCtrl::OnResize(event);
	uiUpdate();
}

void DebuggerHexGui::UpdateOffsetScroll()
{
	if (BytePerLine() > 0) {
		if (offset_scroll->GetRange() != (65536 / BytePerLine()) ||
			offset_scroll->GetThumbPosition() != page_offset / BytePerLine())
		{
			// adjust slider to page size
			offset_scroll->SetScrollbar(page_offset / BytePerLine(), LineCount(), 65536 / BytePerLine(), LineCount());
		}
	}
}

void DebuggerHexGui::OnKeyboardChar(wxKeyEvent &event)
{
	wxChar chr = event.GetKeyCode();
	if (event.GetEventObject() == hex_ctrl) {
		if (isxdigit(chr)) {
			if (chr >= 'a' && chr <= 'f') {
				chr = chr - 'a' + 'A';
			}

			HexCharReplace(GetLocalHexInsertionPoint(), chr);

			char rdchr = hex_ctrl->ReadByte( GetLocalHexInsertionPoint()/2 ); // read from screen
			debugger->writeMemory(CursorOffset(), rdchr);

			if (hex_ctrl->GetInsertionPoint() >= hex_ctrl->GetLastPosition()) {
				if (CursorOffset() + ByteCapacity() <= 65536) {
					LoadFromOffset(page_offset + BytePerLine());
					hex_ctrl->LastLine();
					hex_ctrl->Home();
				}
			} else {
				hex_ctrl->NextChar();
			}
		}
	} else if (event.GetEventObject() == text_ctrl) {
		if (wxString(chr).IsAscii()) {
			TextCharReplace(GetLocalInsertionPoint(), chr);

			int GLIP = GetLocalInsertionPoint(); // this required because TextCharReplace() calls HexCtrl->Replace that alters entering point
			char rdchr = hex_ctrl->ReadByte(GLIP); // read from screen
			debugger->writeMemory(GLIP + page_offset, rdchr);

			if (text_ctrl->GetInsertionPoint() >= text_ctrl->GetLastPosition()) {
				if (CursorOffset() + ByteCapacity() <= 65536) {
					LoadFromOffset(page_offset + BytePerLine());
					text_ctrl->LastLine();
					text_ctrl->Home();
				}
			} else {
				text_ctrl->NextChar();
			}
		}
	}
}

void DebuggerHexGui::OnKeyboardInput(wxKeyEvent& event) {
	wxHexCtrl *myctrl = static_cast<wxHexCtrl*>(event.GetEventObject());

	OnKeyboardSelector(event); // Keyboard Selection Start

	switch (event.GetKeyCode()) {
		case (WXK_UP):
		case (WXK_NUMPAD_UP):
			if (ActiveLine() == 1) {	//If cursor at first line
				if( page_offset == 0 ) {
					wxBell();					// there is no line over up!
				} else {							// Illusion code
					LoadFromOffset( page_offset - BytePerLine() );	//offset decreasing one line & update text with new location, makes screen slide illusion
					//UpdateCursorLocation();
				}
			} else {
				SetLocalHexInsertionPoint( GetLocalHexInsertionPoint() - HexPerLine() );
			}
			break;

		case (WXK_DOWN):
		case (WXK_NUMPAD_DOWN):
			if (ActiveLine() == LineCount()) { //If cursor at bottom of screen
				if (page_offset + ByteCapacity() < 65536) { //detects if another line is present or not
					int temp = GetLocalHexInsertionPoint(); //preserving cursor location
					LoadFromOffset(page_offset + BytePerLine()); //offset increasing one line and update text with new location, makes screen slide illusion
					SetLocalHexInsertionPoint(temp); //restoring cursor location
				} else {
					wxBell(); //there is no line to slide bell
				}
			} else if (ActiveLine() * HexPerLine() > GetLastPosition()) {
				wxBell(); //If cursor at last line but not at bottom of screen
			} else {
				SetLocalHexInsertionPoint(GetLocalHexInsertionPoint() + HexPerLine());
			}
			break;
			// TODO (death#1#): bug in left & text ctrl
		case (WXK_LEFT):
		case (WXK_NUMPAD_LEFT):
			if (GetLocalHexInsertionPoint() == 0) {
				if (page_offset == 0) {
					wxBell();
				} else {
					LoadFromOffset(page_offset - BytePerLine());
					SetLocalHexInsertionPoint(HexPerLine() - 1);
				}
			} else {
				SetLocalHexInsertionPoint(GetLocalHexInsertionPoint() - ((myctrl == hex_ctrl) ? 1 : 2));
			}
			break;
		case (WXK_RIGHT):
		case (WXK_NUMPAD_RIGHT):
			if (myctrl->GetInsertionPoint() >= myctrl->GetLastPosition()) {
				if (page_offset + ByteCapacity() < 65536) { //Checks if its EOF or not
					LoadFromOffset(page_offset + BytePerLine());
					SetLocalHexInsertionPoint((LineCount() - 1) * HexPerLine());
				} else {
					wxBell();
				}
			} else {
				SetLocalHexInsertionPoint(GetLocalHexInsertionPoint() + ((myctrl == hex_ctrl) ? 1 : 2));
			}
			break;

		case (WXK_HOME):
		case (WXK_NUMPAD_HOME): {
			SetLocalHexInsertionPoint(GetLocalHexInsertionPoint() - GetLocalHexInsertionPoint() % HexPerLine());
			break;
		}
		case (WXK_END):
		case (WXK_NUMPAD_END): {
			SetLocalHexInsertionPoint(GetLocalHexInsertionPoint() + HexPerLine() - (GetLocalHexInsertionPoint() % HexPerLine()) - 1);
			break;
		}
		case (WXK_PAGEUP):
		case (WXK_NUMPAD_PAGEUP):
			if (page_offset - ByteCapacity() > 0) {
				//int temp = GetHexInsertionPoint();
				LoadFromOffset(page_offset - ByteCapacity());
				//UpdateCursorLocation();
				//SetLocalHexInsertionPoint(temp);
			} else {
				int temp = GetLocalHexInsertionPoint() % HexPerLine();
				LoadFromOffset(0);
				SetLocalHexInsertionPoint(temp);
				wxBell();
			}
			break;
			// TODO (death#5#): Add last byte problem. Also text ctrl has +1 issue
		case (WXK_PAGEDOWN):
		case (WXK_NUMPAD_PAGEDOWN):
			if (page_offset + ByteCapacity() * 2 < 65536) { //*2 for cosmetic
				int temp = GetLocalHexInsertionPoint();
				LoadFromOffset(page_offset + ByteCapacity());
				SetLocalHexInsertionPoint(temp);
			} else {
				int temp = (GetLocalHexInsertionPoint() % HexPerLine()) + (LineCount() - 1) * HexPerLine();
				page_offset = 65536 - ByteCapacity();
				page_offset += BytePerLine() - page_offset % BytePerLine(); //cosmetic
				//Reload();
				SetLocalHexInsertionPoint(temp);
				wxBell();
			}
			break;

		default: {
			if (event.ControlDown()) {
				switch (event.GetKeyCode()) {
					case (0x41): // CTRL+A = ALL
						Select(0, 65536);
						break;
					case (0x43): // CTRL+C = COPY
						CopySelection();
						break;
					case (0x56): // CTRL+V = PASTE
						PasteFromClipboard();
						break;
					default:
						event.Skip();
						break;
				}
			} else {
				event.Skip();
			}
			break;
		}

	}

	UpdateOffsetScroll();
	OnKeyboardSelector(event); // Keyboard Selection End
	PaintSelection();
}

void DebuggerHexGui::OnMouseWheel(wxMouseEvent & event)
{
	if (event.GetWheelRotation() > 0) { // Going to UP
		page_offset -= BytePerLine() * event.GetLinesPerAction(); //offset decreasing
		if (page_offset <= 0) {
			page_offset = 0;
		}
	} else if (event.GetWheelRotation() < 0) { // Going to BOTTOM
		page_offset += BytePerLine() * event.GetLinesPerAction(); //offset decreasing
		if (page_offset + ByteCapacity() >= 65536) {
			page_offset = 65536 - ByteCapacity();
		}
	}

	LoadFromOffset(page_offset, false, true);
	UpdateOffsetScroll();
}

void DebuggerHexGui::OnMouseRight(wxMouseEvent & event)
{
	wxMenu menu;

	menu.Append(wxID_COPY, _("Copy"));
	menu.Append(wxID_PASTE, _("Paste"));
	menu.Append(wxID_SAVEAS, _("Save as file..."));
	menu.AppendSeparator();
	menu.Append(DEBUGGER_SHOW_IN_CODEVIEW, _("Show in Code View"));

	PopupMenu(&menu);
}

void DebuggerHexGui::OnKeyboardSelector(wxKeyEvent & event)
{
	if (event.GetKeyCode()==WXK_UP || event.GetKeyCode()==WXK_NUMPAD_UP ||
		event.GetKeyCode()==WXK_DOWN || event.GetKeyCode()==WXK_NUMPAD_DOWN ||
		event.GetKeyCode()==WXK_LEFT || event.GetKeyCode()==WXK_NUMPAD_LEFT ||
		event.GetKeyCode()==WXK_RIGHT || event.GetKeyCode()==WXK_NUMPAD_RIGHT ||
		event.GetKeyCode()==WXK_HOME || event.GetKeyCode()==WXK_NUMPAD_HOME ||
		event.GetKeyCode()==WXK_END || event.GetKeyCode()==WXK_NUMPAD_END ||
		event.GetKeyCode()==WXK_PAGEUP || event.GetKeyCode()==WXK_NUMPAD_PAGEUP ||
		event.GetKeyCode()==WXK_PAGEDOWN || event.GetKeyCode()==WXK_NUMPAD_PAGEDOWN)
	{
		if (event.ShiftDown()) {
			Selector();
		} else if (select->GetState()) { // Only SetState if there is a selection to decrease OnUpdateUI event overhead!
			select->SetState(false);
		}
	}


}

void DebuggerHexGui::OnMenuEvent(wxCommandEvent & event)
{
	switch(event.GetId()) {
		case wxID_COPY:
			CopySelection();
			break;
		case wxID_PASTE:
			PasteFromClipboard();
			break;
		default:
			event.Skip();
			break;
	}
}

void DebuggerHexGui::LoadFromOffset(int64_t position, bool cursor_reset, bool paint)
{
	int len = ByteCapacity();
	if (len > 0) {
		char *buffer = new char[len];

		debugger->readMemory((unsigned char*)buffer, (int)position, len);

		ReadFromBuffer(position, len, buffer, cursor_reset, paint);
		delete [] buffer;
	}
}

void DebuggerHexGui::MakeAddressVisible(int64_t position)
{
	if (position < page_offset || position >= page_offset + ByteCapacity()) {
		uint64_t newPageOffset = (position / ByteCapacity()) * ByteCapacity();
		if (newPageOffset + ByteCapacity() >= 65536) {
			newPageOffset = 65535 - ByteCapacity();
		}
		page_offset = newPageOffset;

		LoadFromOffset(page_offset, false, true);
		UpdateOffsetScroll();
	}
}

void DebuggerHexGui::CopySelection()
{
	if (select->GetState()) {
		if (wxTheClipboard->Open()) {
			int length = select->GetSize();
			unsigned char *buffer = new unsigned char[length];
			
			debugger->readMemory(buffer, select->GetStart(), length);
			
			wxTextDataObject *data = new wxTextDataObject(wxString::From8BitData((char *)buffer, length));
			wxTheClipboard->SetData(data);
			wxTheClipboard->Close();
		}
	}
}

void DebuggerHexGui::OnSaveAsFile(wxCommandEvent & event)
{
	if (!saveFileDialog) {
		saveFileDialog = new wxFileDialog(this, _("Save as file"),
			wxEmptyString, wxEmptyString,
			wxT("All files (*.*)|*.*"),
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
	}

	if (saveFileDialog->ShowModal() == wxID_OK) {
		unsigned char *buffer = NULL;
		try {
			wxFileOutputStream output_stream(saveFileDialog->GetPath());

			if (!output_stream.IsOk()) {
				wxMessageBox(_("Failed to save selected data."), _("Save failed"), wxICON_ERROR);
				return;
			}

			int length = select->GetSize();
			unsigned char *buffer = new unsigned char[length];

			debugger->readMemory(buffer, select->GetStart(), length);

			output_stream.Write(buffer, length);

			delete buffer;

			wxMessageBox(wxString::Format(_("Successfuly saved %d bytes."), length), _("File saved"), wxICON_INFORMATION);
		} catch (...) {
			wxMessageBox(_("Failed to save selected data."), _("Save failed"), wxICON_ERROR);
			if (buffer) {
				delete buffer;
			}
		}
	}
}

void DebuggerHexGui::PasteFromClipboard()
{
	if (wxTheClipboard->Open()) {
		if (wxTheClipboard->IsSupported(wxDF_TEXT)) {
			wxTextDataObject data;
			wxTheClipboard->GetData(data);

			int start;
			int length;

			if (select->GetState()) { // into selection
				start = select->GetStart();
				length = select->GetSize() < data.GetTextLength() ? select->GetSize() : data.GetTextLength();
			} else { // rewrite from current position
				start = CursorOffset();
				length = data.GetTextLength();
			}

			debugger->writeMemory((unsigned char*)data.GetText().To8BitData().data(), start, length);
		}
		wxTheClipboard->Close();
	}
}














