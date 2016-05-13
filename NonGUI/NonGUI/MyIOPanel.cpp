#include "MyApp.h"
#include "MyIOPanel.h"
#include "LuaDbgEng.h"
#include "MyWorkThread.h"

enum
{
	ID_BUTTON = 1,
	ID_STCINPUT
};

wxBEGIN_EVENT_TABLE(MyIOPanel, wxPanel)
	EVT_SIZE(MyIOPanel::OnSize)
	EVT_BUTTON(ID_BUTTON, MyIOPanel::OnBtnClick)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(MyStyledText, wxStyledTextCtrl)
	EVT_STC_KEY(ID_STCINPUT, MyStyledText::OnStcKey)
wxEND_EVENT_TABLE()

MyStyledText::MyStyledText(wxWindow *parent, wxWindowID id, wxPoint pos, wxSize size)
	:wxStyledTextCtrl(parent, id, pos, size)
{
	Connect(ID_STCINPUT, wxEVT_STC_KEY, (wxObjectEventFunction)&MyStyledText::OnStcKey);
}

MyIOPanel::MyIOPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) : wxPanel(parent, id, pos, size, style)
{
	m_splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D);

	InitInputPanel();
	InitOutput();

	wxBoxSizer* m_MainSizer;
	m_MainSizer = new wxBoxSizer(wxVERTICAL);
	m_MainSizer->Add(m_splitter, 1, wxEXPAND, 5);

	m_splitter->Connect(wxEVT_IDLE, wxIdleEventHandler(MyIOPanel::m_splitterOnIdle), NULL, this);
	m_splitter->Initialize(m_textOut);
	m_splitter->SplitHorizontally(m_textOut, m_inPanel);

	SetSizer(m_MainSizer);
	Layout();
	Refresh();

	MyWorkThread *m_work = new MyWorkThread();
	m_work->Create();
	m_work->Run();
}

void MyIOPanel::OnBtnClick(wxCommandEvent& event)
{
	wxString& str = m_sciIn->GetText();
	LuaInterpreter *lua = MyApp::Lua;

	if (lua->dostringL((const char *)str.ToAscii()))//If Error
	{
		Print((char *)lua->tostring(-1));
		Print("\n");
	}

	//MyApp::Lua->WriteToStdInput((LPVOID)(const char *)str.c_str(), str.Length(), NULL);
	//MyApp::Lua->WriteNextLine();
}

void MyIOPanel::InitInputPanel()
{
	//Input Panel BEGIN
	m_inPanel = new wxPanel(m_splitter, wxID_ANY, wxDefaultPosition, wxSize(3000, 10), wxTAB_TRAVERSAL);

	wxBoxSizer *m_InPanelSizer, *m_hSizer;
	m_InPanelSizer = new wxBoxSizer(wxVERTICAL);
	m_hSizer = new wxBoxSizer(wxHORIZONTAL);

	InitSciEdit();

	m_btn = new wxButton(m_inPanel, ID_BUTTON, "È·¶¨", wxDefaultPosition, wxSize(50, 30));

	m_status = new wxStaticText(m_inPanel, wxID_ANY, wxT("Debugger Status......."), wxDefaultPosition, wxSize(3000, 20), 0);
	m_status->Wrap(-1);

	m_hSizer->Add(m_sciIn, wxEXPAND);
	m_hSizer->Add(m_btn);
	m_InPanelSizer->Add(m_hSizer, wxEXPAND);
	m_InPanelSizer->Add(m_status, 0);
	m_inPanel->SetSizer(m_InPanelSizer);
	m_inPanel->Layout();
	m_sciIn->SetFocus();
	//Input Panel END
}

void MyIOPanel::InitOutput()
{
	m_textOut = new wxTextCtrl(m_splitter, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	//wxStreamToTextRedirector *redirect = new wxStreamToTextRedirector(m_textOut, &std::cout);
	wxFont font;
	font.SetFaceName("Courier New");
	//font.SetPixelSize(wxSize(12, 12));
	m_textOut->SetFont(font);
	//m_textOut->SetForegroundColour(wxColour(*wxRED));
	m_textOut->Refresh();
}

void MyIOPanel::InitSciEdit()
{
	m_sciIn = new MyStyledText(m_inPanel, ID_STCINPUT, wxDefaultPosition, wxSize(3000, 1000));

	m_sciIn->SetUseTabs(true);
	m_sciIn->SetTabWidth(4);
	m_sciIn->SetIndent(4);
	m_sciIn->SetTabIndents(true);
	m_sciIn->SetBackSpaceUnIndents(true);
	m_sciIn->SetViewEOL(false);
	m_sciIn->SetViewWhiteSpace(false);
	m_sciIn->SetMarginWidth(2, 0);
	m_sciIn->SetIndentationGuides(true);
	m_sciIn->SetMarginType(1, wxSTC_MARGIN_SYMBOL);
	m_sciIn->SetMarginMask(1, wxSTC_MASK_FOLDERS);
	m_sciIn->SetMarginWidth(1, 16);
	m_sciIn->SetMarginSensitive(1, true);
	m_sciIn->SetProperty(wxT("fold"), wxT("1"));
	m_sciIn->SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);
	m_sciIn->SetMarginType(0, wxSTC_MARGIN_NUMBER);
	m_sciIn->SetMarginWidth(0, m_sciIn->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_99999")));
	m_sciIn->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS);
	m_sciIn->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("BLACK")));
	m_sciIn->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("WHITE")));
	m_sciIn->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS);
	m_sciIn->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(wxT("BLACK")));
	m_sciIn->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(wxT("WHITE")));
	m_sciIn->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
	m_sciIn->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS);
	m_sciIn->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, wxColour(wxT("BLACK")));
	m_sciIn->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, wxColour(wxT("WHITE")));
	m_sciIn->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS);
	m_sciIn->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(wxT("BLACK")));
	m_sciIn->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(wxT("WHITE")));
	m_sciIn->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
	m_sciIn->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);
	m_sciIn->SetSelBackground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
	m_sciIn->SetSelForeground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));
	m_sciIn->SetLexerLanguage("Lua");

}

void MyStyledText::OnStcKey(wxStyledTextEvent& event)
{
	wxMessageBox("Onstyleddddddddddddddddddddddddd");
	printf("key:%d\n", event.GetKey());
	event.Skip();
}

void MyIOPanel::OnSize(wxSizeEvent& event)
{
	SetSashPos(GetSize().y / 4 * 3);
	event.Skip();
}

int MyIOPanel::SetSashPos(int pos)
{
	m_splitter->SetSashPosition(pos);
	return 1;
}

int MyIOPanel::Print(wxString& str)
{
	m_textOut->AppendText(str);
	return 1;
}

int MyIOPanel::Print(char *s)
{
	m_textOut->AppendText(s);
	return 1;
}

MyIOPanel::~MyIOPanel()
{
}
