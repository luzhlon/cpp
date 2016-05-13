#ifndef __LUANONUI_H_
#define __LUANONUI_H_

#include "lua.hpp"
#include "MyApp.h"
#include "MyFrame.h"
#include "wx/hashmap.h"

void InitNonGUIlib(lua_State *L);

luaL_Reg LuaNonGUIlib[];

#endif
