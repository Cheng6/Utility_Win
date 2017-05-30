/********************************************************************
	created:	2012/11/08
	created:	8:11:2012   13:39
	filename: 	复合文档查看\StgFileViewer-vs2010\StgFileViewer\EnumStgDocStructure.cpp
	file path:	复合文档查看\StgFileViewer-vs2010\StgFileViewer
	file base:	EnumStgDocStructure
	file ext:	cpp
	author:		hcx
	
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "Objidl.h"
#include "comdef.h"
#include "comdefsp.h"

HRESULT EnumStorageStructure(CList<CComBSTR>& StgStructList, IStorage* spiStorage, IStorage* piRootStorage = NULL, CTreeCtrl* pTreeCtrl = NULL, BOOL bSubStorage = TRUE)
{
	if(!spiStorage)
		return E_INVALIDARG;

	IEnumSTATSTGPtr spiEnumStg;
	HRESULT hr = spiStorage->EnumElements(NULL, NULL, NULL, &spiEnumStg);
	if ( FAILED(hr) )
	{
		AfxMessageBox( _T("Can not Enum the Storage") );
		return E_FAIL;
	}
	STATSTG statStg;
	ULONG ulRead = 0;
	while( SUCCEEDED(spiEnumStg->Next(1, &statStg, &ulRead)) && (ulRead > 0) )
	{
		CComBSTR bstrName = statStg.pwcsName;
		StgStructList.AddTail( bstrName );

		CString szName = bstrName;
		CString szSize;
		szSize.Format( _T("\t\t\t\t /Size=%.6fMB "), statStg.cbSize.QuadPart/1024/1024.0 );
		szName += szSize;
		if (bSubStorage && (statStg.type == STGTY_STORAGE) )
		{
			StgStructList.AddTail(CComBSTR(_T("SubStorage_")));
			szName = _T("SubStorage_") + szName;
			IStoragePtr piSubStg = NULL;
			DWORD mode = STGM_TRANSACTED | STGM_READ | STGM_SHARE_EXCLUSIVE;
			IStoragePtr spiDocStg = piRootStorage ? piRootStorage : spiStorage;
			hr = spiDocStg->OpenStorage( statStg.pwcsName, NULL, mode, 0, 0, &piSubStg ); ASSERT( !FAILED(hr) );
			if(SUCCEEDED(hr))
			{
				HTREEITEM hRoot = TVI_FIRST;
				if(pTreeCtrl)
				{
					hRoot = pTreeCtrl->GetLastVisibleItem();
					TV_ITEM item;
					item.hItem = pTreeCtrl->InsertItem(szName, hRoot);;
					item.mask = TVIF_PARAM | TVIF_STATE;
					pTreeCtrl->GetItem( &item );
					item.mask |= TVIF_CHILDREN;
					item.cChildren = 1;	//Add plus. [11/6/2012 hcx]
					item.stateMask |= TVIS_BOLD;
					item.state |= TVIS_BOLD;
					pTreeCtrl->SetItem(&item);
					pTreeCtrl->Expand(hRoot, TVE_EXPAND);
				}
				hr = EnumStorageStructure(StgStructList, piSubStg, spiDocStg, pTreeCtrl, bSubStorage);
				if(pTreeCtrl && (hRoot != TVI_FIRST))
					pTreeCtrl->Expand(hRoot, TVE_COLLAPSE);
			}
		}
		else
		{
			if(pTreeCtrl)
				pTreeCtrl->InsertItem(szName, pTreeCtrl->GetLastVisibleItem());
		}
	}
	return NOERROR;
}

HRESULT EnumStgDocStructure(CList<CComBSTR> &StgStructList, CTreeCtrl& objTreeCtrl, LPTSTR filename, BOOL bSubStorage = TRUE)
{
	CComBSTR bstrFileName( filename );
	HRESULT hr = StgIsStorageFile(bstrFileName);
	if ( FAILED(hr) )
	{
		AfxMessageBox( _T("Not Compound File!") );
		return E_FAIL;
	}

	IStoragePtr spiStorage;
	hr = StgOpenStorageEx( bstrFileName, 
		STGM_READ|STGM_TRANSACTED|STGM_SHARE_DENY_NONE,
		0, 0, 0, 0, IID_IStorage,  (void**)&spiStorage);
	if ( FAILED(hr) )
	{
		AfxMessageBox( _T("Can not open the file") );
		return E_FAIL;
	}

	CString szSize;
	_stat64i32     f_stat;
	if (_tstat((LPTSTR)filename, &f_stat) == -1) {
		ASSERT(0);
	}
	szSize.Format( _T(" /Size=%.6fMB "), (long)f_stat.st_size/1024/1024.0 );

	CString szFileName(filename);
	szFileName += szSize;
	HTREEITEM hRoot = objTreeCtrl.InsertItem(szFileName);  
	hr = EnumStorageStructure(StgStructList, spiStorage, NULL, &objTreeCtrl, bSubStorage);
	//expand all sub_storage? [11/8/2012 hcx]
	objTreeCtrl.Expand(hRoot, TVE_EXPAND);
	//objTreeCtrl.SetItemState( hRoot, TVIS_EXPANDED, TVIS_EXPANDED );
	return NOERROR;
}