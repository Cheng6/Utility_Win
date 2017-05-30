// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "StgDocStruct.h"

#include "MainFrm.h"
#include "StgDocStructDoc.h"
#include "StgDocStructView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT EnumStgDocStructure(CList<CComBSTR> &StgStructList, CTreeCtrl& objTreeCtrl, LPTSTR filename, BOOL bSubStorage = TRUE);
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers
void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	__super::OnDropFiles(hDropInfo);
	CWnd* pWnd = this;
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


