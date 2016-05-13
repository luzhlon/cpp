/////////////////////////////////////////////////////////////////////////////
// Name:        cloudclientapp.cpp
// Purpose:     
// Author:      CodeSoul
// Modified by: 
// Created:     28/05/2015 17:31:18
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
#include "wx/socket.h"
#include "cloudclientapp.h"
////@end includes


////@begin XPM images

////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( CloudClientApp )
////@end implement app


/*
 * CloudClientApp type definition
 */

IMPLEMENT_CLASS( CloudClientApp, wxApp )


/*
 * CloudClientApp event table definition
 */

BEGIN_EVENT_TABLE( CloudClientApp, wxApp )

////@begin CloudClientApp event table entries
////@end CloudClientApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for CloudClientApp
 */

CloudClientApp::CloudClientApp()
{
    Init();
}


/*
 * Member initialisation
 */

void CloudClientApp::Init()
{
////@begin CloudClientApp member initialisation
////@end CloudClientApp member initialisation
}

/*
 * Initialisation for CloudClientApp
 */

bool CloudClientApp::OnInit()
{    
////@begin CloudClientApp initialisation
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
////@end CloudClientApp initialisation

	wxSocketBase::Initialize();
	MainDlg *dlg = new MainDlg(NULL);
	dlg->ShowModal();
	dlg->Destroy();

    return false;
}


/*
 * Cleanup for CloudClientApp
 */

int CloudClientApp::OnExit()
{    
////@begin CloudClientApp cleanup
    return wxApp::OnExit();
////@end CloudClientApp cleanup
}

