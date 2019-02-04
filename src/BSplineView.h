// BSplineView.h : interface of the CBSplineView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BSPLINEVIEW_H__05D36C91_8E89_45B2_B118_C73027679CBE__INCLUDED_)
#define AFX_BSPLINEVIEW_H__05D36C91_8E89_45B2_B118_C73027679CBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BSplineData.h"
#include "matrixOperations.h"
#include "BoundingBox.h"

class CBSplineDoc;
class CustomView;

class CBSplineView : public CView
{
protected: // create from serialization only
	CBSplineView();
	DECLARE_DYNCREATE(CBSplineView)

// Attributes
public:
	CBSplineDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBSplineView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBSplineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	void iDraw(BOOL draw = TRUE);

	BOOL addingCtrlPts;
	int insertPointAt;

	CustomView* pCustomViewDlg;
	void drawLine(CPoint& fromPt, CPoint& toPt);

	void setSelection(CPoint& point);
	void updateSettingsDlg(void);
	void setStaturBarMsg(CString& str);
	void setStaturBarMsg2(CString& str);

	CPoint calcGeomToScreen(GeomPoint& ptIn);
	GeomPoint calcScreenToGeom(CPoint& ptIn);
	GeomPoint calcScreenToGeom(GeomPoint& ptIn);

	void zoom(void);
	void pan(void);
	void rotate(void);

	double m_zoomFactor;
	double m_zoomIncrement;
	double m_scale;
	GeomPoint m_zoomCenter;

	CPoint m_panStart;
	BOOL isPanning;
	double m_xPanDist;
	double m_yPanDist;

	BOOL isRotating;
	GeomPoint m_rotationCenter;
	GeomVector m_rotationAxis;
	double m_rotationAngle;
	GeomPoint m_referecePtForAxis;

	//Stores complete transform required for plotting geom to screen
	Matrix transform;
/*
When the transform is unit matrix:
	o-------> X-axis
	|
	|
	|
	V  
  Y-axis
*/

protected:
	LONG m_areaLeft;
	LONG m_areaRight;
	LONG m_areaTop;
	LONG m_areaBottom;

	static double m_axisLength;

	void resetTransform()
	{
		//Initial transform
		transform = RotateAroundStandardAxis(axisX, PI);
		transform = transform * Translate(50, 700, 0.0);
	}

	void resetBBox()
	{
		m_bbox.Reset();
		m_bbox.Add(GeomPoint(m_axisLength, m_axisLength, m_axisLength));
		m_bbox.Add(GeomPoint(0.0, 0.0, 0.0));
	}

	void moveGeomToCenter();
	void scaleToFit();

	CBoundingBox m_bbox;

// Generated message map functions
protected:
	//{{AFX_MSG(CBSplineView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBSplineNew();
	afx_msg void OnBSplineDraw();
	afx_msg void OnHelpCommandsInfo();
	afx_msg void OnHelpAbout();
	afx_msg void OnViewFront();
	afx_msg void OnViewTop();
	afx_msg void OnViewRHV();
	afx_msg void OnViewLHV();
	afx_msg void OnViewIsometric();
	afx_msg void OnViewCustom();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDeletePoint();
	//afx_msg void OnInsertPoint(int ID);
	afx_msg void OnInsertPoint(UINT ID); //For VS2003
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void drawCircle(CDC* pDC, CPoint &center, double radius, BOOL fill = FALSE);
};

#ifndef _DEBUG  // debug version in BSplineView.cpp
inline CBSplineDoc* CBSplineView::GetDocument()
   { return (CBSplineDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BSPLINEVIEW_H__05D36C91_8E89_45B2_B118_C73027679CBE__INCLUDED_)
