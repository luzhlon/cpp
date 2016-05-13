#include "MyApp.h"
#include "MyFrame.h"
#include "LuaNonUI.h"

wxIMPLEMENT_APP(MyApp);
//IMPLEMENT_APP_NO_MAIN(MyApp);

bool MyApp::OnInit()
{
	Lua = new LuaAPI(LuaAPI::newstateL());
	Lua->openlibsL();

	Frame = new MyFrame(wxString("NonGUI"));
	SetTopWindow(Frame);
	Frame->Show();

	SetExitOnFrameDelete(true);

	return true;
}

int MyApp::OnExit()
{
	Lua->close();
	return 0;
}