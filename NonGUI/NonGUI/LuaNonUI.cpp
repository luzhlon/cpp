#include "wx/wx.h"
#include "MyApp.h"
#include "LuaAPI.h"
#include "Constant.h"
#include "LuaNonUI.h"
#include "LuaTextCtrl.h"

int LuaAddPane(lua_State *L)
{
	LuaAPI lua(L);
	lua.checktypeL(1, LUA_TUSERDATA);

	MyFrame *frame = (MyFrame *)wxGetApp().GetTopWindow();
	wxWindow *win = (wxWindow *)*(void **)lua.touserdata(1);
	frame->AuiMgr->AddPane(win,
		lua.optinteger(2, wxLEFT), lua.optstring(3, "[Unkown-name]"));
	frame->AuiMgr->Update();

	return 1;
}

wxMenu* LuaMenuParse(LuaAPI &lua, int it)
{
	lua.checkstack(5);
	lua.pushvalue(it);
	wxMenu *menu = new wxMenu;
	MyFrame *frame = wxGetApp().Frame;
	int i = 1;
	int len = lua.rawlen(-1);
	while (i <= len)
	{
		lua.geti(-1, i);
		if (lua.type(-1) == LUA_TTABLE)
		{
			wxMenu *m= LuaMenuParse(lua, -1); 
			if (!m) return NULL;
			//menu->AppendSubMenu(m, 
		}
		const char *menustr = lua.tostring(-1);
		if (!menustr) return NULL; lua.pop(1);
		lua.geti(-1, i);
		if (lua.isfunction(-1)) frame->LuaSetMenuHandler(lua, -1);
		else return NULL; lua.pop(1);
		lua.geti(-1, i);
		const char *hintstr = lua.tostring(-1); lua.pop(1);
		i += 3;
	}
	return menu;
}

int LuaAddMenu(lua_State *L)
{
	LuaAPI lua(L);
	MyFrame *frame = wxGetApp().Frame;
	int ID = frame->NewControlId();
	//frame->MenuBar->Append();

	return 1;
}

int LuaNewPanel(lua_State *L)
{
	LuaAPI lua(L);
	lua.checktypeL(1, LUA_TTABLE);

	return 1;
}

int LuaNewText(lua_State *L)
{
	LuaTextCtrl* text = new LuaTextCtrl(L);
	return 1;
}

luaL_Reg LuaNonGUIlib[] = 
{
	{ "AddPane", LuaAddPane },
	{ "AddMenu", LuaAddMenu },
	{ "NewPaneInfo", LuaNewText },
	{ "NewPanel", LuaNewPanel },
	{ "NewText", LuaNewText },
	{0,0}
};

void InitNonGUIlib(lua_State *L)
{
	LuaAPI lua(L);

	lua.newlibL(LuaNonGUIlib);
	lua.NewConstantTable(wx_lua_constant);
	lua.setfield(-2, "CONSTANT");
	lua.setglobal("NonGUI");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Now non using
extern "C" int __declspec(dllexport) luaopen_NonGUI(lua_State *L)
{
	LuaAPI lua(L);
	wxGetApp().Lua = new LuaAPI(L);
	//lua.newlibL(lib);
	return 1;
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		//wxGetApp().m_hInstance = hInstance;
		break;
	case DLL_PROCESS_DETACH:
		//wxEntryCleanup();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return 1;
}
