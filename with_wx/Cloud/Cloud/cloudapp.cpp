/////////////////////////////////////////////////////////////////////////////
// Name:        cloudapp.cpp
// Purpose:     
// Author:      CodeSoul
// Modified by: 
// Created:     28/05/2015 02:18:50
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "maindlg.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "cloudapp.h"
////@end includes
#include "wx/socket.h"
#include "wx/listimpl.cpp"


////@begin XPM images

////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( CloudApp )
////@end implement app

WX_DEFINE_LIST(ConnectList); 

/*
 * CloudApp type definition
 */

IMPLEMENT_CLASS( CloudApp, wxApp )


/*
 * CloudApp event table definition
 */

BEGIN_EVENT_TABLE( CloudApp, wxApp )

////@begin CloudApp event table entries
////@end CloudApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for CloudApp
 */

CloudApp::CloudApp()
{
    Init();
}


/*
 * Member initialisation
 */

void CloudApp::Init()
{
////@begin CloudApp member initialisation
////@end CloudApp member initialisation
}

/*
 * Initialisation for CloudApp
 */

bool CloudApp::OnInit()
{    
////@begin CloudApp initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
////@end CloudApp initialisation

	wxSocketBase::Initialize();

	MainDlg *dlg = new MainDlg(NULL, wxID_ANY, "Main Dialog.");
	dlg->ShowModal();

    return false;
}


/*
 * Cleanup for CloudApp
 */

int CloudApp::OnExit()
{    
////@begin CloudApp cleanup
    return wxApp::OnExit();
////@end CloudApp cleanup
}

