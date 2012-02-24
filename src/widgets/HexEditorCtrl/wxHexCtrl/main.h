#ifndef MAIN_H
#define MAIN_H

#include "app.h"
#include "wxHexCtrl.h"

class MyFrame: public wxFrame
{
	public:
		MyFrame(wxFrame *frame, const wxString& title);
		~MyFrame();
	private:
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		wxHexCtrl *hex_ctrl;
		DECLARE_EVENT_TABLE();
};

#endif // MAIN_H
