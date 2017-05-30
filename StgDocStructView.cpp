// StgDocStructView.cpp : implementation of the CStgDocStructView class
//

#include "stdafx.h"
#include "StgDocStruct.h"

#include "StgDocStructDoc.h"
#include "StgDocStructView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HRESULT EnumStgDocStructure(CList<CComBSTR> &StgStructList, CTreeCtrl& objTreeCtrl, LPTSTR filename, BOOL bSubStorage = TRUE);
// CStgDocStructView

IMPLEMENT_DYNCREATE(CStgDocStructView, CTreeView)

BEGIN_MESSAGE_MAP(CStgDocStructView, CTreeView)
	ON_COMMAND(ID_FILE_OPEN, &CStgDocStructView::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_SUB_STORAGE, &CStgDocStructView::OnFileOpenShowSubStorage)
END_MESSAGE_MAP()

// CStgDocStructView construction/destruction

CStgDocStructView::CStgDocStructView()
{
	// TODO: add construction code here

}

CStgDocStructView::~CStgDocStructView()
{
}

BOOL CStgDocStructView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

void CStgDocStructView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

}


// CStgDocStructView diagnostics

#ifdef _DEBUG
void CStgDocStructView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CStgDocStructView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CStgDocStructDoc* CStgDocStructView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStgDocStructDoc)));
	return (CStgDocStructDoc*)m_pDocument;
}
#endif //_DEBUG


// CStgDocStructView message handlers

void CStgDocStructView::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE);
	if ( dlg.DoModal() != IDOK )
	{
		return;
	}
	CTreeCtrl& objTreeCtrl = this->GetTreeCtrl();
	CList<CComBSTR> StgStructList;
	HRESULT hr = EnumStgDocStructure(StgStructList, objTreeCtrl, dlg.m_ofn.lpstrFile, FALSE);

	CStgDocStructDoc* pDoc = GetDocument();
	if(pDoc)
	{
		POSITION pos = StgStructList.GetHeadPosition();
		while(pos)
		{
			pDoc->m_StatStgList.AddTail(StgStructList.GetNext(pos));   
		}
	}
}

void CStgDocStructView::OnFileOpenShowSubStorage()
{
	CFileDialog dlg(TRUE);
	if ( dlg.DoModal() != IDOK )
	{
		return;
	}
	CTreeCtrl& objTreeCtrl = this->GetTreeCtrl();
	CList<CComBSTR> StgStructList;
	HRESULT hr = EnumStgDocStructure(StgStructList, objTreeCtrl, dlg.m_ofn.lpstrFile, TRUE);

	CStgDocStructDoc* pDoc = GetDocument();
	if(pDoc)
	{
		POSITION pos = StgStructList.GetHeadPosition();
		while(pos)
		{
			pDoc->m_StatStgList.AddTail(StgStructList.GetNext(pos));   
		}
	}
}

/*{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE);
	if ( dlg.DoModal() != IDOK )
	{
		return;
	}

	CComBSTR bstrFileName( dlg.m_ofn.lpstrFile );
	HRESULT hr = StgIsStorageFile(bstrFileName);
	if ( FAILED(hr) )
	{
		AfxMessageBox( _T("Not Compound File!") );
		return;
	}

	IStoragePtr spiStorage;
	hr = StgOpenStorageEx( bstrFileName, 
		STGM_READ|STGM_TRANSACTED|STGM_SHARE_DENY_NONE,
		0, 0, 0, 0, IID_IStorage,  (void**)&spiStorage);
	if ( FAILED(hr) )
	{
		AfxMessageBox( _T("Can not open the file") );
		return;
	}

	CString szSize;
	_stat64i32     f_stat;
	if (_tstat(dlg.m_ofn.lpstrFile, &f_stat) == -1) {
		ASSERT(0);
	}
	szSize.Format( _T(" /Size=%.6fMB "), (long)f_stat.st_size/1024/1024.0 );

	CString szFileName = dlg.m_ofn.lpstrFile;
	szFileName += szSize;
	HTREEITEM hRoot = this->GetTreeCtrl().InsertItem(szFileName);

	CList<CComBSTR>		StgStructList; 
	CTreeCtrl& objTreeCtrl = this->GetTreeCtrl();
	hr = EnumStorageStructure(StgStructList, spiStorage, NULL, &objTreeCtrl, TRUE);
	CStgDocStructDoc* pDoc = GetDocument();
	objTreeCtrl.Expand(hRoot, TVE_EXPAND);
	//objTreeCtrl.SetItemState( hRoot, TVIS_EXPANDED, TVIS_EXPANDED );
}
*/
