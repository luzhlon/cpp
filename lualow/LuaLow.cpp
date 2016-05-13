#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "LuaAPI.h"

int LuaAlloc(lua_State *L)
{
	LuaAPI lua(L);

    void *p = malloc(lua.checkintegerL(1));
    if(p)
        lua.pushinteger((lua_Integer)p);
    else
        lua.Push(false);

    return 1;
}

int LuaSetMetatable(lua_State *L)
{
	LuaAPI lua(L);

	lua.Push((bool)lua.setmetatable(1));
    //return 1;
    return 0;
}

int LuaSetUservalue(lua_State *L)
{
	LuaAPI lua(L);

	lua.pushvalue(2);
	lua.setuservalue(1);

	return 0;
}

int LuaGetUservalue(lua_State *L)
{
	LuaAPI lua(L);

	lua.getuservalue(1);

	return 1;
}

int LuaNewUserdata(lua_State *L)
{
	LuaAPI lua(L);

    lua.newuserdata(lua.checkintegerL(1));
    if(lua.istable(2))
    {
        lua.pushvalue(2);
        lua.setmetatable(-2);
    }

    return 1;
}

int LuaFree(lua_State *L)
{
	LuaAPI lua(L);

    free(lua.ToBuffer(1));

    return 0;
}

int LuaGetAddr(lua_State *L)
{
	LuaAPI lua(L);

	lua.pushinteger((lua_Integer)lua.ToBuffer(1));

    return 1;
}

int LuaWrite(lua_State *L)
{
	LuaAPI lua(L);

    const char *buf = (const char *)lua.ToBuffer(1);
    const char *str = lua.checkstringL(2);
    unsigned long len = lua.rawlen(2);

    len = lua.isnoneornil(3) ? len : lua.tointeger(3);
    memcpy((void *)buf, str, len);

    return 0;
}

int LuaRead(lua_State *L)
{
	LuaAPI lua(L);

    const char *buf = (const char *)lua.ToBuffer(1);
    if(lua.isnoneornil(2))
        lua.Push(buf);
    else
        lua.pushlstring(buf, luaL_checkinteger(L, 2));

    return 1;
}

int LuaReadInt(lua_State *L)
{
	LuaAPI lua(L);

    union
    {
        long long *pi;
        long long  l;
    }ii;
    ii.pi = (long long *)lua.ToBuffer(1);
    ii.l = *ii.pi;
    int size = lua.isnoneornil(2) ? 4 : lua.tointeger(2);
    int isUnsigned = lua.toboolean(3);
    switch(size)
    {
        case 1:
            lua.pushinteger(isUnsigned ? (LuaAPI::uInt1)ii.l : (LuaAPI::Int1)ii.l);
            break;
        case 2:
            lua.pushinteger(isUnsigned ? (LuaAPI::uInt2)ii.l : (LuaAPI::Int2)ii.l);
            break;
        case 4:
            lua.pushinteger(isUnsigned ? (LuaAPI::uInt4)ii.l : (LuaAPI::Int4)ii.l);
            break;
        case 8:
            lua.pushinteger(isUnsigned ? (LuaAPI::uInt8)ii.l : (LuaAPI::Int8)ii.l);
            break;
        default:
            lua.Push(false);
    }

    return 1;
}

int LuaWriteInt(lua_State *L)
{
	LuaAPI lua(L);

    void *p = lua.ToBuffer(1);
    LuaAPI::uInt8 ul = lua.checkintegerL(2);
    int size = lua.isnoneornil(3) ? 4 : lua.tointeger(3);
    int isUnsigned = lua.toboolean(4);
    switch(size)
    {
        case 1:
			if (isUnsigned) *(LuaAPI::uInt1 *)p = (LuaAPI::uInt1)ul;
			else *(LuaAPI::Int1 *)p = (LuaAPI::Int1)ul; break;
        case 2:
			if (isUnsigned) *(LuaAPI::uInt2 *)p = (LuaAPI::uInt2)ul;
			else *(LuaAPI::Int2 *)p = (LuaAPI::Int2)ul; break;
        case 4:
			if (isUnsigned) *(LuaAPI::uInt4 *)p = (LuaAPI::uInt4)ul;
			else *(LuaAPI::Int4 *)p = (LuaAPI::Int4)ul; break;
        case 8:
			if (isUnsigned) *(LuaAPI::uInt8 *)p = (LuaAPI::uInt8)ul;
			else *(LuaAPI::Int8 *)p = (LuaAPI::Int8)ul; break;
        default:
            lua.Push(false);
    }

    return 0;
}

struct luaL_Reg LuaLow[] = 
{
    { "Alloc", LuaAlloc },
    { "Free", LuaFree },

    { "SetMetatable", LuaSetMetatable },
    { "NewUserdata", LuaNewUserdata },
    { "SetUserdata", LuaSetUservalue },
    { "GetUserdata", LuaGetUservalue },

    { "GetAddr", LuaGetAddr },

    { "Read", LuaRead },
    { "Write", LuaWrite },
    { "ReadInt", LuaReadInt },
    { "WriteInt", LuaWriteInt },
    {0, 0}
};
int __declspec(dllexport) luaopen_LuaLow(lua_State *L)
{
	LuaAPI lua(L);
    lua.newlibL(LuaLow);
    lua.pushvalue(-1);
    lua_setglobal(L, "LuaLow");
	return 1;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return 1;
}
