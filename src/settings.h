#if !defined(AFX_SETTINGS_H__820A5FCE_DB7A_4CEE_8995_5AD6B4976DCD__INCLUDED_)
#define AFX_SETTINGS_H__820A5FCE_DB7A_4CEE_8995_5AD6B4976DCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// settings.h : header file
//

class CBSplineView;
class CBSplineDoc;

/////////////////////////////////////////////////////////////////////////////
// CSettings dialog

class CSettings : public CDialog
{
// Construction
public:
	CSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettings)
	enum { IDD = IDD_DIALOG1 };
//	int		m_degree;
	//}}AFX_DATA


//	virtual void OnDraw(CDC* pDC); 

	CBSplineView* pView;
	CBSplineDoc* pDoc;

	void iDraw(BOOL draw = TRUE, BOOL selectKnot = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettings)
	afx_msg void OnChangeEdit1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void OnOK();
	void OnCancel();
	void drawCircle(CDC* pDC, CPoint &center, double radius, BOOL fill = FALSE);

//	int m_selectedKnot;
	std::vector<CPoint> m_knotPoints;

	long m_knotPathStart;
	long m_knotPathEnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGS_H__820A5FCE_DB7A_4CEE_8995_5AD6B4976DCD__INCLUDED_)
