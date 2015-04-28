#include "app.h"

DECLARE_APP(App)
IMPLEMENT_APP(App)

bool App::OnInit()
{
	Frame *frame = new Frame(wxT("wxCPUID"),wxSize(600,400));

	frame->Show(true);

	return true;
}

Frame::Frame(const wxString &title,const wxSize &size):wxFrame(NULL,wxID_ANY,title,wxDefaultPosition,size,wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxSYSTEM_MENU)
{
	CreateUI();
}

void Frame::CreateUI()
{
	cpuid.GetName(reg[0]);
	wxMenu *file = new wxMenu;
	this->SetTitle(wxString(reg[0].byte,wxConvUTF8));

	file->Append(ENUM_FEATURE,wxT("F&eature\tAlt-e"),wxT("Feature"));
	file->Append(wxID_EXIT,wxT("E&xit\tAlt-q"),wxT("Exit"));

	wxMenuBar *bar = new wxMenuBar;
	bar->Append(file,wxT("file"));

	msg = new MSGList(this);

	SetMenuBar(bar);
	CreateStatusBar(2);
}

void Frame::EnumFeature(wxCommandEvent &event)
{
	cpuid.GetFeature(reg[0]);

	for(int i = 0,j = 1;i < 32;++i){
		wxString msg_str;
		msg_str.Append(wxString(ecx_msg[i],wxConvUTF8));
		if(msg_str.empty()){j <<= 1;continue;}
		msg_str.Append(wxString(YES_NO[(bool)(reg[0].dword[2] & j)],wxConvUTF8));
		j <<= 1;
		msg->AddMessage(msg_str);
	}

	for(int i = 0,j = 1;i < 32;++i){
		wxString msg_str;
		msg_str.Append(wxString(edx_msg[i],wxConvUTF8));
		if(msg_str.empty()){j <<= 1;continue;}
		msg_str.Append(wxString(YES_NO[(bool)(reg[0].dword[3] & j)],wxConvUTF8));
		j <<= 1;
		msg->AddMessage(msg_str);
	}
}

void Frame::OnExit(wxCommandEvent &event)
{
	Close();
}

MSGList::MSGList(wxWindow *parent):wxListCtrl(parent,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLC_REPORT | wxLC_SINGLE_SEL)
{
	item_num = 0;

	wxListItem item;
	item.SetText(wxT("Message"));
	InsertColumn(COLUMN_0,item);
	SetColumnWidth(COLUMN_0,600);
}

void MSGList::AddMessage(const wxString &msg)
{
	InsertItem(item_num,msg,COLUMN_0);
	++item_num;
}