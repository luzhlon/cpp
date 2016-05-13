#include <stdio.h>
#include <LuaAPI.h>

int main()
{
	LuaAPI lua(LuaAPI::newstateL());
	lua.openlibsL();

	if (lua.dofileL("E:\\Github\\LuaDbgEng\\LuaDbgEng.lua"))
	//if (lua.dofileL("lua.lua"))
	{
		puts(lua.tostring(-1));
	}

	printf("dddddddddddddddddddddddddd");
	if (lua.dostringL("print 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa'"))
	{
		puts(lua.tostring(-1));
	}

	system("pause");
	return 0;
}