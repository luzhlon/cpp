#include "MyApp.h"
#include "LuaTextCtrl.h"

const char *LuaTextCtrl::s_type = "WX_TYPE_TEXTCTRL";
luaL_Reg LuaTextCtrlLib[];

LuaTextCtrl::LuaTextCtrl(lua_State *L)
{
	LuaAPI lua(L);
	lua.checktypeL(1, LUA_TTABLE);
	//Construct the control
	long style = lua.OptIntField(1, "Style", 0);
	int x = lua.OptIntField(1, "X", 0);
	int y = lua.OptIntField(1, "Y", 0);
	int width = lua.OptIntField(1, "Width", 0);
	int height = lua.OptIntField(1, "Height", 0);
	const char *hint = lua.OptStrField(1, "Hint", "");
	if (lua.IsFieldType("OnEnter", LUA_TFUNCTION, 1))
		style |= wxTE_PROCESS_ENTER;
	Create(wxGetApp().GetTopWindow(), lua.OptIntField(1, "ID", wxID_ANY),
		wxEmptyString, (x | y ? wxPoint(x, y) : wxDefaultPosition),
		(width && height ? wxSize(width, height) : wxDefaultSize), style);
	if (*hint) SetHint(hint);
	//Storage the callback table
	m_lua = lua.NewCallback(1);
	lua.pushvalue(1);
	LuaAPI::xmove(&lua, m_lua);
	//Event handler
	if (lua.IsFieldType("OnChar", LUA_TFUNCTION, 1))
		Connect(wxEVT_CHAR, wxKeyEventHandler(LuaTextCtrl::OnChar)); lua.pop(1);
	if (lua.IsFieldType("OnKeyDown", LUA_TFUNCTION, 1))
		Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(LuaTextCtrl::OnKeyDown));
	if (lua.IsFieldType("OnKeyUp", LUA_TFUNCTION, 1))
		Connect(wxEVT_KEY_UP, wxKeyEventHandler(LuaTextCtrl::OnKeyUp));
	if (lua.IsFieldType("OnChange", LUA_TFUNCTION, 1))
		Connect(wxEVT_TEXT, wxCommandEventHandler(LuaTextCtrl::OnChange));
	if (lua.IsFieldType("OnMaxLen", LUA_TFUNCTION, 1))
		Connect(wxEVT_TEXT_MAXLEN, wxCommandEventHandler(LuaTextCtrl::OnMaxLen));
	if (lua.IsFieldType("OnDropFiles", LUA_TFUNCTION, 1))
		Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(LuaTextCtrl::OnDropFiles));
	if (style & wxTE_PROCESS_ENTER)
		Connect(wxEVT_TEXT_ENTER, wxCommandEventHandler(LuaTextCtrl::OnEnter));
	//Userdata
	lua.NewUserPointer(this, s_type);
	lua.newlibL(LuaTextCtrlLib);
	lua.SetMetaField(-2, "__index");
}

LuaTextCtrl::~LuaTextCtrl()
{
	delete m_lua;
}

void LuaTextCtrl::OnEnter(wxCommandEvent& event)
{
	if (!m_lua->InitCallback("OnEnter"))
	{
		event.Skip();
		return;
	}
	m_lua->NewUserPointer(this, s_type);
	if (m_lua->BeginCallback(1))
	{
		wxLogMessage("OnEnter:\n%s", m_lua->tostring(-1));
		event.Skip(); return;
	}
	if (m_lua->toboolean(-1)) return;
	event.Skip();
}
void LuaTextCtrl::OnKeyDown(wxKeyEvent& event)
{
	if (!m_lua->InitCallback("OnKyeDown"))
	{
		event.Skip();
		return;
	}
	m_lua->NewUserPointer(this, s_type);
	m_lua->pushinteger(event.GetKeyCode());
	m_lua->pushboolean(event.ControlDown());
	m_lua->pushboolean(event.AltDown());
	m_lua->pushboolean(event.ShiftDown());
	if (m_lua->BeginCallback(1))
	{
		wxLogMessage("OnKeyDown:\n%s", m_lua->tostring(-1));
		event.Skip(); return;
	}
	if (m_lua->toboolean(-1)) return;
	event.Skip();
}
void LuaTextCtrl::OnKeyUp(wxKeyEvent& event)
{
	if (!m_lua->InitCallback("OnKyeUp"))
	{
		event.Skip();
		return;
	}
	m_lua->NewUserPointer(this, s_type);
	m_lua->pushinteger(event.GetKeyCode());
	m_lua->pushboolean(event.ControlDown());
	m_lua->pushboolean(event.AltDown());
	m_lua->pushboolean(event.ShiftDown());
	if (m_lua->BeginCallback(1))
	{
		wxLogMessage("OnKeyUp:\n%s", m_lua->tostring(-1));
		event.Skip(); return;
	}
	if (m_lua->toboolean(-1)) return;
	event.Skip();
}
void LuaTextCtrl::OnChange(wxCommandEvent& event)
{
	if (!m_lua->InitCallback("OnChange"))
	{
		event.Skip();
		return;
	}
	m_lua->NewUserPointer(this, s_type);
	if (m_lua->BeginCallback(1))
	{
		wxLogMessage("OnChange:\n%s", m_lua->tostring(-1));
		event.Skip(); return;
	}
	if (m_lua->toboolean(-1)) return;
	event.Skip();
}
void LuaTextCtrl::OnMaxLen(wxCommandEvent& event)
{
	if (!m_lua->InitCallback("OnMaxLen"))
	{
		event.Skip();
		return;
	}
	m_lua->NewUserPointer(this, s_type);
	if (m_lua->BeginCallback(1))
	{
		wxLogMessage("OnMaxLen:\n%s", m_lua->tostring(-1));
		event.Skip(); return;
	}
	if (m_lua->toboolean(-1)) return;
	event.Skip();
}
void LuaTextCtrl::OnChar(wxKeyEvent& event)
{
	if (!m_lua->InitCallback("OnChar"))
	{
		event.Skip();
		return;
	}
	char key[2] = { 0 };
	m_lua->NewUserPointer(this, s_type);
	m_lua->pushstring((key[0]=event.GetKeyCode(), key));
	m_lua->pushboolean(event.ControlDown());
	m_lua->pushboolean(event.AltDown());
	m_lua->pushboolean(event.ShiftDown());
	if (m_lua->BeginCallback(1))
	{
		wxLogMessage("OnChar:\n%s", m_lua->tostring(-1));
		event.Skip(); return;
	}
	if (m_lua->toboolean(-1)) return;
	event.Skip();
}

void LuaTextCtrl::OnDropFiles(wxDropFilesEvent &event)
{
	if (!m_lua->InitCallback("OnDropFiles"))
	{
		event.Skip();
		return;
	}
	m_lua->NewUserPointer(this, s_type);
	int nFiles = event.GetNumberOfFiles();
	wxString *files = event.GetFiles();
	wxPoint pos = event.GetPosition();
	m_lua->createtable(nFiles, 0);
	for (int i = 0; i < nFiles; i++)
	{
		m_lua->pushstring(files[i].mb_str());
		m_lua->seti(-2, i + 1);
	}
	m_lua->pushinteger(pos.x);
	m_lua->pushinteger(pos.y);
	if (m_lua->BeginCallback(1))
	{
		wxLogMessage("OnChar:\n%s", m_lua->tostring(-1));
		event.Skip(); return;
	}
	if (m_lua->toboolean(-1)) return;
	event.Skip();
}

int LuaAppendText(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);

	int top = lua.gettop();
	for (int i = 2; i <= top; i++)
		text->AppendText(lua.tostring(i));

	return 0;
}
int LuaWriteText(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	return 0;
}

int LuaValue(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	if (lua.isnoneornil(2))
		lua.Push((const char *)text->GetValue().mb_str());
	else
		text->SetValue(lua.tostring(2));
	return 1;
}

int LuaGetLineNumbers(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	lua.pushinteger(text->GetNumberOfLines());
	return 1;
}

int LuaGetLineText(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	lua.pushstring(text->GetLineText(lua.checkintegerL(2)));
	return 1;
}

int LuaGetLineLength(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	lua.pushinteger(text->GetLineLength(lua.checkintegerL(2)));
	return 1;
}

int LuaXY2Pos(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	lua.pushinteger(text->XYToPosition(lua.checkintegerL(2), lua.checkintegerL(3)));
	return 1;
}

int LuaPos2XY(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	long x, y;
	if (text->PositionToXY(lua.checkintegerL(2), &x, &y))
	{
		lua.pushinteger(x);
		lua.pushinteger(y);
		return 2;
	}
	lua.Push(false);
	return 1;
}

int LuaSetStyle(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	lua.checktypeL(2, LUA_TTABLE);
	wxTextAttr style = text->GetDefaultStyle();
	lua.getfield(2, "TextColor");
	if (lua.type(-1) == LUA_TSTRING)
		style.SetTextColour(wxColour(lua.tostring(-1)));
	else if (lua.type(-1) == LUA_TNUMBER)
		style.SetTextColour(wxColour(lua.tointeger(-1)));
	lua.pop(1);
	lua.getfield(2, "BackColor");
	if (lua.type(-1) == LUA_TSTRING)
		style.SetBackgroundColour(wxColour(lua.tostring(-1)));
	else if (lua.type(-1) == LUA_TNUMBER)
		style.SetBackgroundColour(wxColour(lua.tointeger(-1)));
	lua.pop(1);
	wxFont font;
	lua.getfield(2, "Font");
	if (lua.type(-1) == LUA_TSTRING)
		font.SetFaceName(lua.tostring(-1));
	lua.pop(1);
	lua.getfield(2, "FontSize");
	if (lua.type(-1) == LUA_TNUMBER)
		style.SetFontSize(lua.tointeger(-1));
	lua.pop(1);
	//////////////////////////////////
	bool b;
	if (lua.type(3) == LUA_TNUMBER && lua.type(4) == LUA_TNUMBER)
		b = text->SetStyle(lua.tointeger(3), lua.tointeger(4), style);
	else
		b = text->SetDefaultStyle(style);
	//lua.getfield(2, "TextColor");
	//lua.getfield(2, "TextColor");
	lua.Push(b);
	return 1;
}

int LuaHint(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	const char *hint = lua.tostring(3);
	if (hint) lua.Push(text->SetHint(hint));
	else lua.pushstring(text->GetHint());
	return 1;
}
int LuaSelect(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	if (lua.isnoneornil(2) && lua.isnoneornil(3))
	{
		text->SelectAll();
		return 0;
	}
	long from = lua.optinteger(2, 0),
		to = lua.optinteger(3, text->GetLastPosition());
	text->SetSelection(from, to);
	return 0;
}
int LuaInsertPos(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	if (lua.isnoneornil(2))
	{
		lua.pushinteger(text->GetInsertionPoint());
		return 1;
	}
	else text->SetInsertionPoint(lua.tointeger(2));
	return 0;
}
int LuaSelection(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	long from, to;
	if (lua.isnoneornil(2) && lua.isnoneornil(3))
	{
		text->GetSelection(&from, &to);
		lua.Push(text->GetStringSelection());
		lua.pushinteger(from);
		lua.pushinteger(to);
		return 3;
	}
	from = lua.optinteger(2, 0);
	to = lua.optinteger(3, text->GetLastPosition());
	if (lua.isnoneornil(4))
	{
		lua.Push(text->GetRange(from, to));
		return 1;
	}
	else
		text->Replace(from, to, lua.tostring(4));
	return 0;
}
/*
int LuaPos2XY(lua_State *L)
{
	LuaAPI lua(L);
	LuaTextCtrl *text = (LuaTextCtrl *)lua.CheckUserPointer(1, LuaTextCtrl::s_type);
	return 1;
}
*/

luaL_Reg LuaTextCtrlLib[] = 
{
	{ "print", LuaAppendText },
	{ "write", LuaWriteText },
	{ "lines", LuaGetLineNumbers },
	{ "linetext", LuaGetLineText },
	{ "linelen", LuaGetLineLength },
	{ "Pos2XY", LuaPos2XY },
	{ "XY2Pos", LuaXY2Pos },
	{ "value", LuaValue },
	{ "hint", LuaHint },
	{ "select", LuaSelect },
	{ "selection", LuaSelection },
	{ "setstyle", LuaSetStyle },
	{ "curpos", LuaInsertPos },
	{0,0}
};
