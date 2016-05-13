#include "MyApp.h"
#include "MyFrame.h"
#include "wx/wfstream.h"
#include "wx/fileconf.h"
//#include "LuaTextCtrl.h"

MyFrame::MyFrame(wxString& title,
	const wxPoint& pos, const wxSize& size)
	:wxFrame(NULL, wxID_ANY, title, pos, size)
{
	InitEventHandler();
	InitAuiMgr();
	InitMenuBar();
	InitStatusBar();
	InitLua();
	SetSize(wxSize(720, 540));
	SetSizeHints(wxSize(720, 540));
	Refresh();
}

void MyFrame::InitAuiMgr()
{
	AuiMgr = new wxAuiManager(this);
	m_stdPanel = new wxStdioPanel(this, wxID_ANY, wxDefaultPosition, wxSize(800,800));
	wxAuiPaneInfo pane;
	AuiMgr->AddPane(m_stdPanel,
		pane.Left().MinSize(wxSize(500, 700)).Caption("Standard Input && Output"));
	AuiMgr->Update();

	/*
	wxFileInputStream file("Config.ini");
	if (!file.IsOk()) return;
	wxFileConfig conf(file);
	wxString pp = conf.Read("Perspective");
	AuiMgr->LoadPerspective(pp);//*/
}

void MyFrame::InitStatusBar()
{
	CreateStatusBar();   
	SetStatusText("Windows Debugger with wxWidgets&Lua53" );
}

void MyFrame::InitLua()
{
	LuaAPI lua = *wxGetApp().Lua;
	void InitNonGUIlib(lua_State*);
	InitNonGUIlib(lua.GetState());

	lua.newtable();
	m_lua = lua.NewCallback(-1); wxASSERT(m_lua);
	m_RefMenuCall = lua.RegGetRef();

	//创建UI的动作要在同一个线程里完成
	if (lua.dofileL("E:\\GitHub\\LuaDbgEng\\LuaDbgEng.lua"))
		wxLogMessage("LuaDbgEng.lua:\n%s", lua.tostring(-1));
}

void MyFrame::InitMenuBar()
{
	SetMenuBar(MenuBar = new wxMenuBar);
}

void MyFrame::InitEventHandler()
{
	Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyFrame::OnMenuSelected));
	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MyFrame::OnClose));
}

void MyFrame::LuaSetMenuHandler(LuaAPI &lua, int id)
{
	lua.checkstack(3);
	lua.RegGet(m_RefMenuCall);
	lua.pushvalue(-2);
	lua.seti(-2, id);
	lua.pop(2);
}

void MyFrame::OnMenuSelected(wxCommandEvent& event)
{
	if (!m_lua->InitCallback(event.GetId())) return;
	m_lua->BeginCallback(0);
}

void MyFrame::OnSize(wxSizeEvent& event)
{
	event.Skip();
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
	//wxLogMessage("OnOpen");
}

void MyFrame::OnClose(wxCloseEvent& event)
{
	exit(0);
}

MyFrame::~MyFrame()
{
}
