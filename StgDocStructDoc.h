// StgDocStructDoc.h : interface of the CStgDocStructDoc class
//


#pragma once


class CStgDocStructDoc : public CDocument
{
protected: // create from serialization only
	CStgDocStructDoc();
	DECLARE_DYNCREATE(CStgDocStructDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CStgDocStructDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CList<CComBSTR>		m_StatStgList;

protected:
	
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


