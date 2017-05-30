// StgDocStruct.h : main header file for the StgDocStruct application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CStgDocStructApp:
// See StgDocStruct.cpp for the implementation of this class
//

class CStgDocStructApp : public CWinApp
{
public:
	CStgDocStructApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenShowSubStorage();
};

extern CStgDocStructApp theApp;