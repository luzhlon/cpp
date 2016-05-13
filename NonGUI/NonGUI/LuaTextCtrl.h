#ifndef __LUATEXTCTRL_H_
#define __LUATEXTCTRL_H_

#include "wx/textctrl.h"
#include "LuaAPI.h"

class LuaTextCtrl : public wxTextCtrl
{
public:
	static const char * s_type;
public:
	LuaTextCtrl(lua_State *L);
	~LuaTextCtrl();

private:
	LuaCallback *m_lua;

protected:
	void OnEnter(wxCommandEvent&);
	void OnKeyDown(wxKeyEvent&);
	void OnKeyUp(wxKeyEvent&);
	void OnChar(wxKeyEvent&);
	void OnChange(wxCommandEvent&);
	void OnMaxLen(wxCommandEvent&);
	void OnDropFiles(wxDropFilesEvent &);
	//wxDECLARE_EVENT_TABLE();
};

luaL_Reg LuaTextCtrlLib[];

#endif
