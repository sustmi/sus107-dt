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
}

DebuggerHexGui::~DebuggerHexGui() {
	if (debugger) {
		debugger->removeListener(this);
	}

	hex_ctrl->Disconnect(wxEVT_CHAR, wxKeyEventHandler(DebuggerHexGui::OnKeyboardChar), NULL, this);
	text_ctrl->Disconnect(wxEVT_CHAR, wxKeyEventHandler(DebuggerHexGui::OnKeyboardChar), NULL, this);
	hex_ctrl->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(DebuggerHexGui::OnKeyboardInput), NULL, this);
	text_ctrl->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(DebuggerHexGui::OnKeyboardInput), NULL, this);
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
	int address = debugger->getCpuRegister(regPC);
	LoadFromOffset(address, false, true);
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
	//Keyboard Selection Code
	if (event.GetKeyCode()==WXK_UP || event.GetKeyCode()==WXK_NUMPAD_UP ||
		event.GetKeyCode()==WXK_DOWN || event.GetKeyCode()==WXK_NUMPAD_DOWN ||
		event.GetKeyCode()==WXK_LEFT || event.GetKeyCode()==WXK_NUMPAD_LEFT ||
		event.GetKeyCode()==WXK_RIGHT || event.GetKeyCode()==WXK_NUMPAD_RIGHT ||
		event.GetKeyCode()==WXK_HOME || event.GetKeyCode()==WXK_NUMPAD_HOME ||
		event.GetKeyCode()==WXK_END || event.GetKeyCode()==WXK_NUMPAD_END ||
		event.GetKeyCode()==WXK_PAGEUP || event.GetKeyCode()==WXK_NUMPAD_PAGEUP ||
		event.GetKeyCode()==WXK_PAGEDOWN || event.GetKeyCode()==WXK_NUMPAD_PAGEDOWN)
	{
		//OnKeyboardSelector(event);	//Selection Starter call
	}


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
			event.Skip();
			/*if (event.ControlDown())
				switch (event.GetKeyCode()) {
				case (0x5a): // CTRL+Z = UNDO
					if (event.ShiftDown())
						DoRedo(); // UNDO with shift = REDO
					else
						DoUndo();
					break;
				case (0x59): // CTRL+Y = REDO
					DoRedo();
					break;
				case (0x53): { // CTRL+S = SAVE
					FileSave();
					// TODO (death#1#): File Name star'in * when file changed & saved
				}
					break;
				case (0x41): // CTRL+A = ALL
					Select(0, FileLength());
					break;
				case (0x58): // CTRL+X = CUT
					wxBell();
					break;
				case (0x43): // CTRL+C = COPY
					CopySelection();
					break;
				case (0x56): // CTRL+V = PASTE
					PasteFromClipboard();
					break;
				case (0x46): // CTRL+F = FIND
					//finddlg();
					break;
				case (0x4f): // CTRL+O = OPEN
					wxBell();
					break;
				default:
					event.Skip(); // ->OnKeyboardChar( event );
					break;
				}
			else
				event.Skip(); // ->OnKeyboardChar( event );*/
		}


	}//switch end
	UpdateOffsetScroll();
	//OnKeyboardSelector(event);
	PaintSelection( );
}

void DebuggerHexGui::LoadFromOffset(int64_t position, bool cursor_reset, bool paint)
{
	int len = ByteCapacity();
	char *buffer = new char[len];

	debugger->readMemory((unsigned char*)buffer, (int)position, len);

	ReadFromBuffer(position, len, buffer, cursor_reset, paint);
	delete [] buffer;
}





