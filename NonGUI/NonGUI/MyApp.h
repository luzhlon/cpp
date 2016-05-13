#ifndef __MYAPP_H_
#define __MYAPP_H_

#include "wx/app.h"
#include "wx/wx.h"
#include "LuaAPI.h"

class MyFrame;

class MyApp : public wxApp
{
public:
	bool OnInit() wxOVERRIDE;
	int  OnExit() wxOVERRIDE;

	LuaAPI    *Lua;
	MyFrame   *Frame;
	//HINSTANCE  m_hInstance;
};

wxDECLARE_APP(MyApp);

#endif //#ifndef __MYAPP_H_
