/*
 * DebuggerHexGui.cpp
 *
 *  Created on: 23.2.2012
 *      Author: mirek
 */

#include "DebuggerHexGui.h"

DebuggerHexGui::DebuggerHexGui(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
	HexEditorCtrl(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	debugger = NULL;

	offset_scroll->Enable(true);
	offset_ctrl->SetOffsetLimit(65536);
	offset_ctrl->offset_mode = 'X';

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
	if (debugger) {
		debugger->removeListener(this);
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

void DebuggerHexGui::OnOffsetScroll(wxScrollEvent & event)
{
	printf("DebuggerHexGui::OnOffsetScroll %d\n", event.GetPosition());
	LoadFromOffset(event.GetPosition() * BytePerLine());
}

void DebuggerHexGui::setDebugger(Debugger *debugger)
{
	this->debugger = debugger;
	debugger->addListener(this);
}

void DebuggerHexGui::debuggerEvent(DebuggerEvent event)
{
	if (event == DEBUGGER_EVENT_MEMORY_CHANGED ||
		event == DEBUGGER_EVENT_EMULATION_STEP ||
		event == DEBUGGER_EVENT_EMULATION_STOP)
	{
		uiUpdate();
	}
}

void DebuggerHexGui::uiUpdate()
{
	//int address = debugger->getCpuRegister(regPC);
	//LoadFromOffset(address, false, true);
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

		case (WXK_DELETE):
		case (WXK_NUMPAD_DELETE):
			/*if( myctrl->GetInsertionPoint() != myctrl->GetLastPosition() ){
			 int hex_loc = myctrl->GetInsertionPoint();
			 HexCharReplace(hex_loc,'0');
			 myctrl->SetInsertionPoint(hex_loc);
			 }
			 else
			 */
			wxBell();
			break;

		case (WXK_BACK):
			/*
			 if( myctrl->GetInsertionPoint()!=0 ){
			 HexCharReplace(myctrl->GetInsertionPoint()-1,'0');
			 myctrl->SetInsertionPoint(myctrl->GetInsertionPoint()-1);
			 }
			 else
			 if(page_offset == 0)
			 wxBell();
			 else{	//Has to be a line over up
			 // TODO (death#3#): if BytePerLine() changes, current offset gona be mad, like taking minus argument because it thinks a compleete line over up... spend caution about it.
			 page_offset -= myctrl->BytePerLine();
			 LoadFromOffset( page_offset );
			 HexCharReplace(myctrl->HexPerLine()-1,'0');
			 myctrl->SetInsertionPoint(myctrl->HexPerLine()-1);
			 }
			 */
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

	menu.Append(wxID_COPY, wxT("Copy"));
	//menu.Append(idCopyAs, _("CopyAs"));
	//menu.Append(idSaveAsDump, _("Save As Dump"));
	menu.Append(wxID_PASTE, wxT("Paste"));

	/*menu.AppendSeparator();
	menu.Append(idFillSelection, _("Fill Selecton"));
	if( BlockSelectOffset == -1 )
		menu.Append(idBlockSelect, _("Set Selection Block Start"));
	else
		menu.Append(idBlockSelect, _("Set Selection Block End"));
	menu.AppendSeparator();
	menu.Append(idTagQuick,			 _("Quick Tag"), _("Creates empty tag with Random Color."));
	menu.Append(idTagAddSelection, _("New Tag") );
	menu.Append(idTagEdit, 			 _("Tag Edit"));

	menu.Enable( idTagEdit, false );
	for( unsigned i = 0 ; i < MainTagArray.Count() ; i++ ) {
		TagElement *TAG = MainTagArray.Item(i);
		if( TAG->isCover(TagPosition) ) {
			menu.Enable( idTagEdit, true );
			break;
		}
	}

	menu.Enable( idTagQuick, select->GetState() );
	menu.Enable( idTagAddSelection, select->GetState() );
	menu.Enable( wxID_COPY, select->GetState() );
	menu.Enable( idCopyAs, select->GetState() );
	menu.Enable( idSaveAsDump, select->GetState() );
	menu.Enable( idFillSelection, select->GetState() );
	menu.Enable( wxID_PASTE, not select->GetState() );
	menu.Enable( wxID_DELETE, select->GetState());
	menu.Enable( idInjection, not select->GetState());
	menu.Enable( wxID_CUT, select->GetState());*/

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

void DebuggerHexGui::CopySelection()
{
	printf("copy %d %d %d\n", select->GetStart(), select->GetEnd(), select->GetState());

	if (select->GetState()) {
		if (wxTheClipboard->Open()) {
			int length = select->GetSize();
			unsigned char *buffer = new unsigned char[length];
			debugger->readMemory(buffer, select->GetStart(), length);
			wxTheClipboard->SetData(new wxTextDataObject(wxString::From8BitData((char *)buffer, length)));
			wxTheClipboard->Close();
		}
	}
}

void DebuggerHexGui::PasteFromClipboard()
{
	printf("paste %d %d\n", select->GetStart(), select->GetEnd());

	if (select->GetState()) {
		printf("paste1\n");
		if (wxTheClipboard->Open()) {
			printf("paste2\n");
			if (wxTheClipboard->IsSupported(wxDF_TEXT)) {
				printf("paste3\n");
				wxTextDataObject data;
				wxTheClipboard->GetData(data);

				int length = select->GetSize() < data.GetTextLength() ? select->GetSize() : data.GetTextLength();
				debugger->writeMemory((unsigned char*)data.GetText().mb_str().data(), select->GetStart(), length);
			}
			wxTheClipboard->Close();
		}
	}
}









