/////////////////////////////////////////////////////////////////////////////
// Name:        connectmanagerdlg.cpp
// Purpose:     
// Author:      CodeSoul
// Modified by: 
// Created:     01/06/2015 23:05:40
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 01/06/2015 23:05:40

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "maindlg.h"
#include "cloudapp.h"
#include "connectmanagerdlg.h"

////@begin XPM images
////@end XPM images


/*
 * ConnectManagerDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ConnectManagerDlg, wxDialog )


/*
 * ConnectManagerDlg event table definition
 */

BEGIN_EVENT_TABLE( ConnectManagerDlg, wxDialog )

////@begin ConnectManagerDlg event table entries
    EVT_BUTTON( ID_BTN_CLOSE, ConnectManagerDlg::OnBtnCloseClick )
////@end ConnectManagerDlg event table entries

END_EVENT_TABLE()


/*
 * ConnectManagerDlg constructors
 */

ConnectManagerDlg::ConnectManagerDlg()
{
    Init();
}

ConnectManagerDlg::ConnectManagerDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * ConnectManagerDlg creator
 */

bool ConnectManagerDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ConnectManagerDlg creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ConnectManagerDlg creation

	UpdateList();

    return true;
}


/*
 * ConnectManagerDlg destructor
 */

ConnectManagerDlg::~ConnectManagerDlg()
{
////@begin ConnectManagerDlg destruction
////@end ConnectManagerDlg destruction
}


/*
 * Member initialisation
 */

void ConnectManagerDlg::Init()
{
////@begin ConnectManagerDlg member initialisation
    m_listConnect = NULL;
    m_btnClose = NULL;
////@end ConnectManagerDlg member initialisation
}


/*
 * Control creation for ConnectManagerDlg
 */

void ConnectManagerDlg::CreateControls()
{    
////@begin ConnectManagerDlg content construction
    // Generated by DialogBlocks, 01/06/2015 23:36:45 (unregistered)

    ConnectManagerDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxArrayString m_listConnectStrings;
    m_listConnect = new wxListBox( itemDialog1, ID_LISTBOX, wxDefaultPosition, wxSize(500, 300), m_listConnectStrings, wxLB_SINGLE );
    itemBoxSizer2->Add(m_listConnect, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

    itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_btnClose = new wxButton( itemDialog1, ID_BTN_CLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(m_btnClose, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end ConnectManagerDlg content construction
}


/*
 * Should we show tooltips?
 */

bool ConnectManagerDlg::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap ConnectManagerDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ConnectManagerDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ConnectManagerDlg bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon ConnectManagerDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ConnectManagerDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ConnectManagerDlg icon retrieval
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_CLOSE
 */

void ConnectManagerDlg::OnBtnCloseClick( wxCommandEvent& event )
{
	int n = m_listConnect->GetSelection();
	if (n != wxNOT_FOUND)
	{
		auto node = wxGetApp().m_cntList.Item(n);
		ThreadConnect *thread = node->GetData();
		wxLogMessage("Terminate the connect %s", m_listConnect->GetString(n));
		thread->Kill();//This will delete the object
	}
	UpdateList();
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_CLOSE in ConnectManagerDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BTN_CLOSE in ConnectManagerDlg. 
}

void ConnectManagerDlg::UpdateList()
{
	auto &list = wxGetApp().m_cntList;
	wxIPV4address ipAddr;
	int i = 0;
	m_listConnect->Clear();
	for (auto iter = list.begin(); iter != list.end(); i++, iter++)
	{
		ThreadConnect *thread = *iter;
		wxSocketBase *connect = thread->m_connect;
		connect->GetPeer(ipAddr);
		wxString strItem = wxString::Format("%s %d", ipAddr.IPAddress(), ipAddr.Service());
		m_listConnect->InsertItems(1, &strItem, i);
	}
}
