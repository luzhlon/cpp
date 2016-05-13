/////////////////////////////////////////////////////////////////////////////
// Name:        cloudapp.h
// Purpose:     
// Author:      CodeSoul
// Modified by: 
// Created:     28/05/2015 02:18:50
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CLOUDAPP_H_
#define _CLOUDAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
////@end includes

#include "wx/list.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

class ThreadConnect;

WX_DECLARE_LIST(ThreadConnect, ConnectList);

/*!
 * CloudApp class declaration
 */

class CloudApp: public wxApp
{    
    DECLARE_CLASS( CloudApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    CloudApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

	ConnectList m_cntList;

////@begin CloudApp event handler declarations
////@end CloudApp event handler declarations

////@begin CloudApp member function declarations
////@end CloudApp member function declarations

////@begin CloudApp member variables
////@end CloudApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(CloudApp)
////@end declare app

#endif
    // _CLOUDAPP_H_
