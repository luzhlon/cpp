#ifndef _MYIOPANEL_H_
#define _MYIOPANEL_H_

#include <wx/wx.h>
//#include <wx/richtext/richtextctrl.h>
#include <wx/splitter.h>
#include <wx/stc/stc.h>

class MyStyledText : public wxStyledTextCtrl
{
public:
	MyStyledText(wxWindow *, wxWindowID, wxPoint, wxSize);
	//MyStyledText();
	//~MyStyledText();

private:
	void OnStcKey(wxStyledTextEvent& event);

	wxDECLARE_EVENT_TABLE();
};

class MyIOPanel : public wxPanel
{
public:
	MyIOPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize(500, 300), long style = wxTAB_TRAVERSAL ); 
	~MyIOPanel();
	
	int SetSashPos(int);
	int Print(wxString&);
	int Print(char *);
	int GetInput();

	void m_splitterOnIdle( wxIdleEvent& )
	{
		m_splitter->SetSashPosition( 0 );
		m_splitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MyIOPanel::m_splitterOnIdle ), NULL, this );
	}

protected:
	wxSplitterWindow* m_splitter;
	wxTextCtrl* m_textOut;
	//Input Panel
	wxPanel* m_inPanel;
	MyStyledText* m_sciIn;
	wxButton	*m_btn;
	wxStaticText* m_status;
	wxCheckBox  * m_chkEnter;

	void OnBtnClick(wxCommandEvent& event);
	void InitInputPanel();
	void InitOutput();

private:
	void OnSize(wxSizeEvent&);

	void InitSciEdit();

	wxDECLARE_EVENT_TABLE();
};

#endif
