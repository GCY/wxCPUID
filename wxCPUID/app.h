#ifndef __DRIVER_LOADER__
#define __DRIVER_LOADER__

#include <wx/wx.h>
#include <wx/listctrl.h>

#include "cpuid.h"
#include "msg.h"

enum{
	COLUMN_0 = 0,
	ENUM_FEATURE = 100
};

class App:public wxApp
{
public:

	bool OnInit();
};

class MSGList:public wxListCtrl
{
public:

	MSGList(wxWindow*);

	void AddMessage(const wxString&);

private:

	int item_num;
};

class Frame:public wxFrame
{
public:

	Frame(const wxString&,const wxSize&);

	void EnumFeature(wxCommandEvent&);
	void OnExit(wxCommandEvent&);
private:

	void CreateUI();

	CPUID cpuid;

	MSGList *msg;
	Register reg[3];

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(Frame,wxFrame)
	EVT_MENU(ENUM_FEATURE,Frame::EnumFeature)
	EVT_MENU(wxID_EXIT,Frame::OnExit)
END_EVENT_TABLE()

#endif