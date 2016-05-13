#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include "wx/wx.h"
#include "wx/aui/aui.h"
#include "wxStdioPanel.h"

class LuaTextCtrl;

class MyFrame : public wxFrame
{
public:
	MyFrame(wxString&, const wxPoint& pos=wxDefaultPosition,
		const wxSize& size=wxDefaultSize);
	~MyFrame();

	void LuaSetMenuHandler(LuaAPI&, int);//0, -1

	wxAuiManager *AuiMgr;
	wxMenuBar    *MenuBar;
private:
	void InitLua();
	void InitMenuBar();
	void InitAuiMgr();
	void InitStatusBar();
	void InitStdIoPanel();
	void InitEventHandler();

	void OnMenuSelected(wxCommandEvent&);

	void OnOpen(wxCommandEvent&);
	void OnClose(wxCloseEvent&);
	void OnSize(wxSizeEvent&);
	void OnInputEnter(wxCommandEvent &);

	LuaCallback*  m_lua;
	wxStdioPanel* m_stdPanel;
	int           m_RefMenuCall;
};

enum
{
	ID_OPEN = 1
};

#endif
