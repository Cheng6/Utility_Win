// StgDocStructView.h : interface of the CStgDocStructView class
//


#pragma once


class CStgDocStructView : public CTreeView
{
protected: // create from serialization only
	CStgDocStructView();
	DECLARE_DYNCREATE(CStgDocStructView)

// Attributes
public:
	CStgDocStructDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CStgDocStructView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenShowSubStorage();
};

#ifndef _DEBUG  // debug version in StgDocStructView.cpp
inline CStgDocStructDoc* CStgDocStructView::GetDocument() const
   { return reinterpret_cast<CStgDocStructDoc*>(m_pDocument); }
#endif

