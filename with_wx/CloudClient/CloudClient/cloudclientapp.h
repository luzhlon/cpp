/////////////////////////////////////////////////////////////////////////////
// Name:        cloudclientapp.h
// Purpose:     
// Author:      CodeSoul
// Modified by: 
// Created:     28/05/2015 17:31:18
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CLOUDCLIENTAPP_H_
#define _CLOUDCLIENTAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
////@end includes

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

/*!
 * CloudClientApp class declaration
 */

class CloudClientApp: public wxApp
{    
    DECLARE_CLASS( CloudClientApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    CloudClientApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();
	
////@begin CloudClientApp event handler declarations
////@end CloudClientApp event handler declarations

////@begin CloudClientApp member function declarations
////@end CloudClientApp member function declarations

////@begin CloudClientApp member variables
////@end CloudClientApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(CloudClientApp)
////@end declare app

#endif
    // _CLOUDCLIENTAPP_H_
