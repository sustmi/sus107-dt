// -*- C++ -*- generated by wxGlade HG on Tue Jan 10 21:38:21 2012

#include <wx/wx.h>
#include <wx/image.h>
#include "Emulator.h"


class Application: public wxApp {
public:
	bool OnInit();
};

IMPLEMENT_APP(Application)

bool Application::OnInit()
{
	wxInitAllImageHandlers();
	Emulator* emulator = new Emulator(NULL, wxID_ANY, wxEmptyString);
	emulator->init();
	SetTopWindow(emulator);
	emulator->Show();
	return true;
}
