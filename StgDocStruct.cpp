// StgDocStruct.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "StgDocStruct.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "StgDocStructDoc.h"
#include "StgDocStructView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT EnumStgDocStructure(CList<CComBSTR> &StgStructList, CTreeCtrl& objTreeCtrl, LPTSTR filename, BOOL bSubStorage = TRUE);

// CStgDocStructApp

BEGIN_MESSAGE_MAP(CStgDocStructApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CStgDocStructApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CStgDocStructApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_SUB_STORAGE, &CStgDocStructApp::OnFileOpenShowSubStorage)
END_MESSAGE_MAP()


// CStgDocStructApp construction

CStgDocStructApp::CStgDocStructApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CStgDocStructApp object

CStgDocStructApp theApp;


// CStgDocStructApp initialization

BOOL CStgDocStructApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_StgDocStructTYPE,
		RUNTIME_CLASS(CStgDocStructDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CStgDocStructView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//Allow drag/drop the file into the app. [11/8/2012 hcx]
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CStgDocStructApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CStgDocStructApp message handlers

void CStgDocStructApp::OnFileOpen()
{
	CWinApp::OnFileOpen();
	CWnd* pWnd = m_pMainWnd;
	if (pWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)))	 
	{
		BOOL max;
		CFrameWnd *pFrame = ((CMainFrame*)pWnd)->MDIGetActive(&max);
		if (pFrame) 
		{
			pWnd = pFrame->GetActiveView();
		}
	}

	CStgDocStructView * pView = DYNAMIC_DOWNCAST(CStgDocStructView, pWnd);
	CStgDocStructDoc* pDoc = pView ? pView->GetDocument() : NULL;
	if(pView && pDoc)
	{
		CTreeCtrl& objTreeCtrl = pView->GetTreeCtrl();
		CList<CComBSTR> StgStructList;
		HRESULT hr = EnumStgDocStructure(StgStructList, objTreeCtrl, (LPTSTR)(LPCTSTR)(pDoc->GetPathName()), FALSE);

		if(pDoc)
		{
			POSITION pos = StgStructList.GetHeadPosition();
			while(pos)
			{
				pDoc->m_StatStgList.AddTail(StgStructList.GetNext(pos));   
			}
		}
	}
}

void CStgDocStructApp::OnFileOpenShowSubStorage()
{
	CWinApp::OnFileOpen();
	CWnd* pWnd = m_pMainWnd;
	if (pWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)))	 
	{
		BOOL max;
		CFrameWnd *pFrame = ((CMainFrame*)pWnd)->MDIGetActive(&max);
		if (pFrame) 
		{
			pWnd = pFrame->GetActiveView();
		}
	}
	CStgDocStructView * pView = DYNAMIC_DOWNCAST(CStgDocStructView, pWnd);
	CStgDocStructDoc* pDoc = pView ? pView->GetDocument() : NULL;
	if(pView && pDoc)
	{
		CTreeCtrl& objTreeCtrl = pView->GetTreeCtrl();
		CList<CComBSTR> StgStructList;
		HRESULT hr = EnumStgDocStructure(StgStructList, objTreeCtrl, (LPTSTR)(LPCTSTR)(pDoc->GetPathName()), TRUE);

		if(pDoc)
		{
			POSITION pos = StgStructList.GetHeadPosition();
			while(pos)
			{
				pDoc->m_StatStgList.AddTail(StgStructList.GetNext(pos));   
			}
		}
	}
}
