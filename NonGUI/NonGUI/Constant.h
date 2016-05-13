
#include "wx/wx.h"
#include "LuaAPI.h"

LuaAPI::LUA_CONSTANT wx_lua_constant[] = 
{
	{ "wxID_ANY", wxID_ANY },
	{"wxLEFT",wxLEFT},
	{"wxRIGHT",wxRIGHT},
	{"wxTOP",wxTOP},
	{"wxBOTTOM",wxBOTTOM},
	{ "wxTE_PROCESS_ENTER", wxTE_PROCESS_ENTER },
	{"wxTE_PROCESS_TAB", wxTE_PROCESS_TAB },
	{"wxTE_MULTILINE", wxTE_MULTILINE },
	{"wxTE_PASSWORD", wxTE_PASSWORD },
	{"wxTE_READONLY", wxTE_READONLY },
	{"wxTE_RICH", wxTE_RICH },
	{"wxTE_RICH2", wxTE_RICH2 },
	{"wxTE_AUTO_URL", wxTE_AUTO_URL },
	{"wxTE_NOHIDESEL", wxTE_NOHIDESEL },
	{"wxHSCROLL", wxHSCROLL },
	{"wxTE_NO_VSCROLL", wxTE_NO_VSCROLL },
	{"wxTE_LEFT", wxTE_LEFT },
	{"wxTE_CENTRE", wxTE_CENTRE },
	{"wxTE_RIGHT", wxTE_RIGHT },
	{"wxTE_DONTWRAP", wxTE_DONTWRAP },
	{"wxTE_CHARWRAP", wxTE_CHARWRAP },
	{"wxTE_WORDWRAP", wxTE_WORDWRAP },
	{"wxTE_BESTWRAP", wxTE_BESTWRAP },
	{"wxTE_CAPITALIZE", wxTE_CAPITALIZE },
	{0,0}
};
