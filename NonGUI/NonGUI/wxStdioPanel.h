#ifndef __WXSTDIOPANEL_H_
#define __WXSTDIOPANEL_H_

#include "wx/wx.h"
#include "StdRedirector.h"

class ThreadIn;

class wxStdioPanel : public wxPanel
{
public:
	wxStdioPanel(wxWindow *, wxWindowID id, const wxPoint&, const wxSize&);
	~wxStdioPanel();

	void SetPrompt(const wxString&);
	void Print(const wxString&);
	wxString &Read();
	StdRedirector *m_ReDir;
	wxTextCtrl *m_stdout;
	wxTextCtrl *m_stdin;
	wxStaticText *m_promt;

	ThreadIn *m_threadin;

protected:
	void OnInputEnter(wxCommandEvent& event);
};

class ThreadIn : public wxThread
{
public:
	ThreadIn(wxStdioPanel *panel) :m_panel(panel), wxThread(wxTHREAD_DETACHED)
	{
		m_semph = new wxSemaphore(0, 1);
	}
	~ThreadIn()
	{
		delete m_semph;
	}
	wxSemaphore *m_semph;

private:
	ExitCode Entry();
	wxStdioPanel *m_panel;
};

class ThreadOut : public wxThread
{
public:
	ThreadOut(wxStdioPanel *panel) :m_panel(panel), wxThread(wxTHREAD_DETACHED){}
	//~ThreadOut();

private:
	ExitCode Entry();
	wxStdioPanel *m_panel;
};

class ThreadErr : public wxThread
{
public:
	ThreadErr(wxStdioPanel *panel) :m_panel(panel), wxThread(wxTHREAD_DETACHED){}
	//~ThreadErr();

private:
	ExitCode Entry();
	wxStdioPanel *m_panel;
};

enum
{
	ID_STDIN = 1,
	ID_STDOUT
};

#endif
