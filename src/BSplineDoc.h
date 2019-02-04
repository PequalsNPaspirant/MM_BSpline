// BSplineDoc.h : interface of the CBSplineDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BSPLINEDOC_H__DB347796_0F9E_4963_A6FE_8F80BB010BFC__INCLUDED_)
#define AFX_BSPLINEDOC_H__DB347796_0F9E_4963_A6FE_8F80BB010BFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BSplineData;

class CBSplineDoc : public CDocument
{
protected: // create from serialization only
	CBSplineDoc();
	DECLARE_DYNCREATE(CBSplineDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBSplineDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBSplineDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BSplineData* pBSplineData;
	int m_selectedPt;
	int m_selectedKnot;
	
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBSplineDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BSPLINEDOC_H__DB347796_0F9E_4963_A6FE_8F80BB010BFC__INCLUDED_)
