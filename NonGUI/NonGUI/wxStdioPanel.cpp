#include "wxStdioPanel.h"

wxStdioPanel::wxStdioPanel(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
	:wxPanel(parent, id, pos, size)
{
	m_ReDir = new StdRedirector;
	m_stdin = new wxTextCtrl(this, ID_STDIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	Connect(ID_STDIN, wxEVT_TEXT_ENTER, wxCommandEventHandler(wxStdioPanel::OnInputEnter));
	m_stdout = new wxTextCtrl(this, ID_STDOUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	wxFont font;
	font.SetFaceName("Courier New");
	m_stdout->SetFont(font);
	m_promt = new wxStaticText(this, wxID_ANY, "         ");
	wxStreamToTextRedirector redirector(m_stdout);

	//sizer
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
	hsizer->Add(m_promt, 0);
	hsizer->Add(m_stdin, 1, wxGROW | wxALL);
	sizer->Add(m_stdout, 1, wxGROW | wxALL);
	sizer->Add(hsizer, 0, wxGROW | wxALL);
	SetSizer(sizer);

	//thread
	m_threadin = new ThreadIn(this);
	ThreadOut *threadout = new ThreadOut(this);
	ThreadErr *threaderr = new ThreadErr(this);
	m_threadin->Create(); m_threadin->Run();
	threadout->Create();  threadout->Run();
	threaderr->Create();  threaderr->Run();
}

void wxStdioPanel::OnInputEnter(wxCommandEvent& event)
{
	m_threadin->m_semph->Post();
}

wxStdioPanel::~wxStdioPanel()
{
	delete m_ReDir;
	delete m_threadin;
}

wxThread::ExitCode ThreadIn::Entry()
{
	wxString str;
	while (true)
	{
		m_panel->m_promt->SetLabel("[NOINPUT]");
		m_panel->m_ReDir->WriteToStdInput((LPVOID)(const char *)str.ToAscii(), 0);
		m_panel->m_promt->SetLabel("INPUT:");
		m_semph->Wait();//µÈ´ýÊäÈë
		str = m_panel->m_stdin->GetValue();
		m_panel->m_ReDir->WriteToStdInput((LPVOID)(const char *)str.ToAscii(), str.length());
		m_panel->m_ReDir->WriteToStdInput("\r\n", 2);
		m_panel->m_stdin->Clear();
	}
	return (ExitCode)0;
}

wxThread::ExitCode ThreadOut::Entry()
{
	char str[10240] = { 0 };
	int size;
	while (true)
	{
		m_panel->m_ReDir->ReadFromStdOutput((LPVOID)str, 10240, (LPDWORD)&size);
		str[size] = 0;
		m_panel->m_stdout->AppendText(str);
	}
	return (ExitCode)0;
}

wxThread::ExitCode ThreadErr::Entry()
{
	char str[10240] = { 0 };
	int size;
	while (true)
	{
		m_panel->m_ReDir->ReadFromStdError((LPVOID)str, 10240, (LPDWORD)&size);
		str[size] = 0;
		m_panel->m_stdout->AppendText(str);
	}
	return (ExitCode)0;
}
