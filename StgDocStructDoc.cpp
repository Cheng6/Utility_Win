// StgDocStructDoc.cpp : implementation of the CStgDocStructDoc class
//

#include "stdafx.h"
#include "StgDocStruct.h"

#include "StgDocStructDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStgDocStructDoc

IMPLEMENT_DYNCREATE(CStgDocStructDoc, CDocument)

BEGIN_MESSAGE_MAP(CStgDocStructDoc, CDocument)
END_MESSAGE_MAP()


// CStgDocStructDoc construction/destruction

CStgDocStructDoc::CStgDocStructDoc()
{
	// TODO: add one-time construction code here

}

CStgDocStructDoc::~CStgDocStructDoc()
{
}

BOOL CStgDocStructDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CStgDocStructDoc serialization

void CStgDocStructDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CStgDocStructDoc diagnostics

#ifdef _DEBUG
void CStgDocStructDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStgDocStructDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStgDocStructDoc commands
