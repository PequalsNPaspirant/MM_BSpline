// BSplineView.cpp : implementation of the CBSplineView class
//

#include "stdafx.h"
#include "BSpline.h"

#include "BSplineDoc.h"
#include "BSplineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "settings.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "CustomView.h"

/////////////////////////////////////////////////////////////////////////////
// CBSplineView

IMPLEMENT_DYNCREATE(CBSplineView, CView)

BEGIN_MESSAGE_MAP(CBSplineView, CView)
	//{{AFX_MSG_MAP(CBSplineView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()	
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BSPLINE_NEW, OnBSplineNew)
	ON_COMMAND(ID_BSPLINE_DRAW, OnBSplineDraw)
	ON_COMMAND(ID_HELP_COMMANDSINFO, OnHelpCommandsInfo)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_VIEW_FRONT, OnViewFront)
	ON_COMMAND(ID_VIEW_TOP, OnViewTop)
	ON_COMMAND(ID_VIEW_RHV, OnViewRHV)
	ON_COMMAND(ID_VIEW_LHV, OnViewLHV)
	ON_COMMAND(ID_VIEW_ISOMETRIC, OnViewIsometric)
	ON_COMMAND(ID_VIEW_CUSTOM, OnViewCustom)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(501, OnDeletePoint)
	ON_COMMAND_RANGE(601, 602, OnInsertPoint)
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

double CBSplineView::m_axisLength = 300;

/////////////////////////////////////////////////////////////////////////////
// CBSplineView construction/destruction

CBSplineView::CBSplineView()
	:pCustomViewDlg(NULL)
{
	// TODO: add construction code here	

	addingCtrlPts = TRUE;

	insertPointAt = -1;

	m_zoomFactor = 1.0;
	m_scale = 1.0;
	m_zoomIncrement = 0.05;

	isPanning = FALSE;
	m_xPanDist = 0.0;
	m_yPanDist = 0.0;

	isRotating = FALSE;
	m_rotationAngle = 0.0;

	//Initial transform
	resetTransform();
	resetBBox();
}

CBSplineView::~CBSplineView()
{
	if(NULL != pCustomViewDlg)
	{
		pCustomViewDlg->DestroyWindow();
		delete pCustomViewDlg;
		pCustomViewDlg = NULL;
	}
	
}

BOOL CBSplineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBSplineView printing

BOOL CBSplineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBSplineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBSplineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBSplineView diagnostics

#ifdef _DEBUG
void CBSplineView::AssertValid() const
{
	CView::AssertValid();
}

void CBSplineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBSplineDoc* CBSplineView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBSplineDoc)));
	return (CBSplineDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBSplineView drawing

void CBSplineView::OnDraw(CDC* pDC)
{
	CBSplineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	{
		//test 1: success
		Matrix t;
		GeomPoint center(26.24, 4.35, 58.65);
		GeomVector axis(26, 45, 85);
		double angle = 0.0046;
		Matrix rotationMatrix = Rotate(center, axis, angle);
		t = t * rotationMatrix;

		angle = -0.0046;
		rotationMatrix = Rotate(center, axis, angle);
		t = t * rotationMatrix;
		if(!t.isUnitMatrix())
		{
			ASSERT(0);
		}
	}
	{
		//test 2: 
		Matrix t;
		GeomPoint center(26.24, 4.35, 58.65);
		//GeomVector axis(26, 45, 85);
		GeomVector axis(1, 1, 1);
		double angle = 0.0046;
		Matrix rotationMatrix = Rotate(center, axis, angle);
		t = t * rotationMatrix;

		//test on matrix inverse
		Matrix inv = t.inverse();
		Matrix res = t * inv;
		if(!res.isUnitMatrix())
		{
			ASSERT(0);
		}
		res = inv * t;
		if(!res.isUnitMatrix())
		{
			ASSERT(0);
		}

		//GeomVector axis(-26, -45, -85);
		axis = GeomVector(-1, -1, -1);
		rotationMatrix = Rotate(center, axis, angle);
		t = t * rotationMatrix;
		if(!t.isUnitMatrix())
		{
			ASSERT(0);
		}
	}
	{
		//test 2: around X-axis 
		Matrix t;
		GeomPoint center(26.24, 4.35, 58.65);
		GeomVector axis(1, 0, 0);
		double angle = 0.0046;
		Matrix rotationMatrix = Rotate(center, axis, angle);
		t = t * rotationMatrix;

		axis = GeomVector(-1, 0, 0);
		rotationMatrix = Rotate(center, axis, angle);
		t = t * rotationMatrix;
		if(!t.isUnitMatrix())
		{
			ASSERT(0);
		}
	}
	{
		//test 2: around Y-axis 
		Matrix t;
		GeomPoint center(26.24, 4.35, 58.65);
		GeomVector axis(0, 1, 0);
		double angle = 0.0046;
		Matrix rotationMatrix = Rotate(center, axis, angle);
		t = t * rotationMatrix;

		axis = GeomVector(0, -1, 0);
		rotationMatrix = Rotate(center, axis, angle);
		t = t * rotationMatrix;
		if(!t.isUnitMatrix())
		{
			ASSERT(0);
		}
	}
	{
		//test 2: around Z-axis 
		Matrix t;
		GeomPoint center(26.24, 4.35, 58.65);
		GeomVector axis(0, 0, 1);
		double angle = 0.0046;
		Matrix rotationMatrix = Rotate(center, axis, angle);
		t = t * rotationMatrix;

		axis = GeomVector(0, 0, -1);
		rotationMatrix = Rotate(center, axis, angle);
		t = t * rotationMatrix;
		if(!t.isUnitMatrix())
		{
			ASSERT(0);
		}
	}
	//TO DO
	{
		//test on mtrix operations
		Matrix t;
	}

	CRect rect;
	this->GetClientRect(&rect);
		
	long clearance = 5;
	m_areaLeft = rect.left + clearance;
	m_areaRight = rect.right - clearance;
	m_areaTop = rect.top + clearance;
	m_areaBottom = rect.bottom - clearance;

	CClientDC dc(this);
	dc.SetROP2(R2_COPYPEN);

	//Draw border
	dc.MoveTo ( CPoint(m_areaLeft, m_areaTop) ) ;
	dc.LineTo ( CPoint(m_areaRight, m_areaTop) ) ;
	dc.LineTo ( CPoint(m_areaRight, m_areaBottom) ) ;
	dc.LineTo ( CPoint(m_areaLeft, m_areaBottom) ) ;
	dc.LineTo ( CPoint(m_areaLeft, m_areaTop) ) ;	

	iDraw();

	CString str;
	if(addingCtrlPts == TRUE)
	str = "Press Left Mouse button on screen to place control points. Select BSpline -> Draw from Menu to draw the curve";
	else
	str = "Select & Drag control points on screen to move it.Selected point & portion of curve affected are highlighted in BLUE on screen.";
	setStaturBarMsg(str);
}


/////////////////////////////////////////////////////////////////////////////
// CBSplineView message handlers

void CBSplineView::iDraw(BOOL draw)
{
	CBSplineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CClientDC dcPoly(this);

	//Draw co-ordinate Axes
	GeomPoint origin(0.0, 0.0, 0.0);
	CPoint ptOrigin = calcGeomToScreen(origin);

	//draw x axis
	GeomPoint xStartG(-m_axisLength, 0.0, 0.0);
	CPoint xStart = calcGeomToScreen(xStartG);
	GeomPoint xEndG(m_axisLength, 0.0, 0.0);
	CPoint xEnd = calcGeomToScreen(xEndG);
	dcPoly.MoveTo(ptOrigin);
	dcPoly.LineTo(xEnd);

	//draw y axis
	GeomPoint yStartG(0.0, -m_axisLength, 0.0);
	CPoint yStart = calcGeomToScreen(yStartG);
	GeomPoint yEndG(0.0, m_axisLength, 0.0);
	CPoint yEnd = calcGeomToScreen(yEndG);
	dcPoly.MoveTo(ptOrigin);
	dcPoly.LineTo(yEnd);

	//draw z axis
	GeomPoint zStartG(0.0, 0.0, -m_axisLength);
	CPoint zStart = calcGeomToScreen(zStartG);
	GeomPoint zEndG(0.0, 0.0, m_axisLength);
	CPoint zEnd = calcGeomToScreen(zEndG);
	dcPoly.MoveTo(ptOrigin);
	dcPoly.LineTo(zEnd);

	//Circle at origin
//	drawCircle()

	{
	//Draw direction guide at top-right corner
	//find origin of direction guide
	CPoint ptGuide(m_areaRight - 50, m_areaTop + 50);
	drawCircle(&dcPoly, ptGuide, 2);
	GeomPoint ptGuideGeom = calcScreenToGeom(ptGuide);

	//Keep the length of axes constant on screen irrespective of scale, zoom, pan
	const int length = 40 * transform.getScale();
	GeomPoint xEndG(ptGuideGeom.m_x + length, ptGuideGeom.m_y, ptGuideGeom.m_z);
	GeomPoint yEndG(ptGuideGeom.m_x, ptGuideGeom.m_y + length, ptGuideGeom.m_z);
	GeomPoint zEndG(ptGuideGeom.m_x, ptGuideGeom.m_y, ptGuideGeom.m_z + length);
	
	CRect r(0, 100, 100, 0);

	//draw x axis
	CPoint xEnd = calcGeomToScreen(xEndG);
	dcPoly.MoveTo(ptGuide);
	dcPoly.LineTo(xEnd);
	/*r.MoveToXY(xEnd);
	CRect re(xEnd, CPoint(xEnd.x + 25, xEnd.y - 25));
	dcPoly.DrawText("X", re, DT_SINGLELINE);*/
	//draw y axis
	CPoint yEnd = calcGeomToScreen(yEndG);
	dcPoly.MoveTo(ptGuide);
	dcPoly.LineTo(yEnd);
	//draw z axis
	CPoint zEnd = calcGeomToScreen(zEndG);
	dcPoly.MoveTo(ptGuide);
	dcPoly.LineTo(zEnd);
	}

	if(pDoc->pBSplineData->m_controlPoints.size() == 0)
		return;

	CClientDC dcCurve(this);
	COLORREF clr = RGB(255, 0, 0);
	CPen pen(0, 1, clr);
	CPen* pCurrPen = dcCurve.SelectObject(&pen);

	CClientDC dcSelection(this);
	COLORREF clrSelection = RGB(0, 0, 255);
	CPen penSelection(0, 1, clrSelection);
	CPen* pCurrPen2 = dcSelection.SelectObject(&penSelection);
	
	//Draw Control Polygon
	CPoint ctrlPt, lastCtrlPt;
	//lastCtrlPt.x = pDoc->pBSplineData->m_controlPoints[0].m_x; //To make it work as 3D
	//lastCtrlPt.y = pDoc->pBSplineData->m_controlPoints[0].m_y;
	//lastCtrlPt = calcGeomToScreen(GeomPoint(lastCtrlPt));
	lastCtrlPt = calcGeomToScreen(pDoc->pBSplineData->m_controlPoints[0]);
	for(int i = 0; i < pDoc->pBSplineData->m_controlPoints.size(); i++)
	{
		//ctrlPt.x = pDoc->pBSplineData->m_controlPoints[i].m_x; //To make it work as 3D
		//ctrlPt.y = pDoc->pBSplineData->m_controlPoints[i].m_y;
		//ctrlPt = calcGeomToScreen(GeomPoint(ctrlPt));
		ctrlPt = calcGeomToScreen(pDoc->pBSplineData->m_controlPoints[i]);
		
		dcPoly.MoveTo(lastCtrlPt);
		dcPoly.LineTo(ctrlPt);
		lastCtrlPt = ctrlPt;

		if(pDoc->m_selectedPt == i)
			drawCircle(&dcSelection, ctrlPt, 4.0, TRUE);
		else
			drawCircle(&dcPoly, ctrlPt, 4.0);	
	}

	if(addingCtrlPts == TRUE)
		return;

	//Draw Curve
	GeomPoint pt = pDoc->pBSplineData->GetPointOnCurve(0.0);
	CPoint cpt;
	//cpt.x = pt.m_x;
	//cpt.y = pt.m_y;
	//cpt = calcGeomToScreen(GeomPoint(cpt));
	//Need change to draw 3D curve
	cpt = calcGeomToScreen(pt);
	dcCurve.MoveTo(cpt);
	dcSelection.MoveTo(cpt);

	int knotToDraw = 0;
	int noOfKnots = pDoc->pBSplineData->m_knotSequence.size();
	double knotVal = pDoc->pBSplineData->m_knotSequence[knotToDraw];
	double oldKnotVal = knotVal;
	BOOL affectedCurve = FALSE;
	for(double t = 0.0; t <= 1; t += 0.001)
	{
		pt = pDoc->pBSplineData->GetPointOnCurve(t);
		//cpt.x = pt.m_x;
		//cpt.y = pt.m_y;
		//cpt = calcGeomToScreen(GeomPoint(cpt));
		//Need change to draw 3D curve
		cpt = calcGeomToScreen(pt);

		//Check if this portion of curve is changed while moving selected point
		if(pDoc->m_selectedPt != -1)
		{
			if(pDoc->m_selectedPt <= (knotToDraw - 1) && (knotToDraw - 1) <= (pDoc->m_selectedPt + pDoc->pBSplineData->m_degree))
				affectedCurve = TRUE;
			else
				affectedCurve = FALSE;
		}
		if(affectedCurve)
			dcSelection.LineTo(cpt);
		else
			dcCurve.LineTo(cpt);
		
		if(knotToDraw < noOfKnots &&  fabs(knotVal - t) < PRECISION)
		{
			if(pDoc->m_selectedKnot == knotToDraw)
				drawCircle(&dcSelection, cpt, 5, TRUE);
			else
				drawCircle(&dcCurve, cpt, 5);
			dcCurve.MoveTo(cpt);
			do 
			{
				oldKnotVal = knotVal;
				knotToDraw++;
				knotVal = pDoc->pBSplineData->m_knotSequence[knotToDraw];
			}while(fabs(oldKnotVal - knotVal) < PRECISION);
		}
		dcCurve.MoveTo(cpt);
		dcSelection.MoveTo(cpt);
	}

	{
		CPoint xEnd(250, 250);
		CRect re(xEnd, CPoint(xEnd.x + 25, xEnd.y - 25));
	dcPoly.DrawText("XY", re, DT_SINGLELINE);
	}
}

void CBSplineView::drawCircle(CDC* pDC, CPoint &center, double radius, BOOL fill /*= FALSE*/)
{
	if(NULL == pDC)
		return;

	pDC->MoveTo(CPoint(center.x + radius, center.y));
	pDC->AngleArc(center.x, center.y, radius, 0, 360);

	if(fill == TRUE)
	{
		radius += 2;
		CRgn rgn;
		rgn.CreateEllipticRgn(center.x - radius, center.y - radius, center.x + radius, center.y + radius);
		CBrush brush(RGB(0, 0, 255));
		pDC->FillRgn(&rgn, &brush);
	}
}

int CBSplineView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBSplineView message handlers

void CBSplineView::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	if(addingCtrlPts == TRUE)
		return;

	setSelection(point);

	CView::OnLButtonDown(nFlags, point);
}

void CBSplineView::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	CBSplineDoc* pDoc = GetDocument();
	GeomPoint pt = calcScreenToGeom(point);

	if(addingCtrlPts == TRUE)
	{		
		pDoc->pBSplineData->m_controlPoints.push_back(pt);
	}

	if(insertPointAt != -1)
	{
		std::vector<GeomPoint>::iterator it = pDoc->pBSplineData->m_controlPoints.begin();
		it += insertPointAt;

		pDoc->pBSplineData->m_controlPoints.insert(it, pt);
		insertPointAt = -1;

		//Update bbox
		m_bbox.Add(pt);

		pDoc->pBSplineData->MakeValid(TRUE);
		updateSettingsDlg();
	}

	if(pDoc->m_selectedKnot != -1)
	{
		pDoc->m_selectedKnot = -1;
		updateSettingsDlg();
	}
		
	isPanning = FALSE;

	RedrawWindow();
	
	CView::OnLButtonUp(nFlags, point);
}

void CBSplineView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if ( ::GetKeyState ( VK_CONTROL ) < 0 )
	{
		isPanning = TRUE;
		m_panStart = point;
	}
	else
	{
		isRotating = TRUE;

		//rotate about view window center
		CRect r;
		GetClientRect(&r);
		ClientToScreen(&r);
		CPoint center = r.CenterPoint();
		//m_rotationCenter = calcScreenToGeom(center);
		m_rotationCenter = GeomPoint(center);

		m_referecePtForAxis = GeomPoint(point);
	}	
	
	CView::OnRButtonDown(nFlags, point);
}

void CBSplineView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	isPanning = FALSE;
	isRotating = FALSE;
	
	setSelection(point);

	RedrawWindow();

	CView::OnRButtonUp(nFlags, point);
}

void CBSplineView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CBSplineDoc* pDoc = GetDocument();

	GeomPoint pt = calcScreenToGeom(point);

	if(nFlags == MK_LBUTTON && (pDoc->m_selectedPt != -1))
	{		
		pDoc->pBSplineData->m_controlPoints[pDoc->m_selectedPt] = pt;
		RedrawWindow();
	}

	if(isPanning == TRUE)
	{
		double xShift = point.x - m_panStart.x;
		double yShift = point.y - m_panStart.y;
		if(fabs(xShift) > PRECISION || fabs(yShift) > PRECISION)
		{
			m_xPanDist = xShift;
			m_yPanDist = yShift;
			pan();
			RedrawWindow();
			m_panStart = point;
		}
	}
	else if(isRotating == TRUE)
	{
		GeomPoint referecePtForAxis2(point);
		GeomVector dir(referecePtForAxis2, m_referecePtForAxis);
		//if(!dir.isNULL())
		if(dir.magnitude() > 5.0)
		{
			GeomVector screenNormal(0.0, 0.0, 1.0);
			dir = dir.getUnitVector();
			GeomVector rotationAxis = screenNormal.cross(dir);
			//m_rotationAxis = rotationAxis * transform.Inverse();
			m_rotationAxis = rotationAxis;

			//GeomPoint pt1 = calcScreenToGeom(m_referecePtForAxis);
			//GeomPoint pt2 = calcScreenToGeom(referecePtForAxis2);
			//GeomVector vect1(m_referecePtForAxis, m_rotationCenter);
			//GeomVector vect2(referecePtForAxis2, m_rotationCenter);
			//m_rotationAngle = abs(vect1.angle(vect2));
			//m_rotationAngle = vect1.angle(vect2);
			m_rotationAngle = 0.005;
			rotate();
			RedrawWindow();

			{
				//to be removed
			CClientDC dcPoly(this);
			//drawCircle(&dcPoly, center, 2);
			dcPoly.MoveTo(CPoint(m_rotationCenter.m_x, m_rotationCenter.m_y));
			CPoint nextPt(m_rotationCenter.m_x + rotationAxis.m_x * 25, m_rotationCenter.m_y + rotationAxis.m_y * 25);
			dcPoly.LineTo(nextPt);

			dcPoly.MoveTo(CPoint(m_referecePtForAxis.m_x, m_referecePtForAxis.m_y));
			dcPoly.LineTo(CPoint(referecePtForAxis2.m_x, referecePtForAxis2.m_y));
			}

			m_referecePtForAxis = referecePtForAxis2;			
		}
	}

	CString str;
	str.Format("x = %.2lf y = %.2lf z = %.2lf", pt.m_x, pt.m_y, pt.m_z);
	setStaturBarMsg(str);
	CView::OnMouseMove(nFlags, point);
}

BOOL CBSplineView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{	
	//This point is screen point so convert it to client
	CPoint localPt = pt;
	ScreenToClient(&localPt);
	m_zoomCenter.m_x = localPt.x;
	m_zoomCenter.m_y = localPt.y;
	m_zoomCenter.m_z = 0.0;
	double oldZoomFactor = m_zoomFactor;
	if(zDelta < 0)
		m_zoomFactor += m_zoomIncrement;
	else
		m_zoomFactor -= m_zoomIncrement;

	if(m_zoomFactor < m_zoomIncrement)
	{
		m_zoomFactor = m_zoomIncrement;
		AfxMessageBox("Can not zoom in further");
	}
	else if(m_zoomFactor > 10)
	{
		m_zoomFactor = 10;
		AfxMessageBox("Can not zoom out further");
	}
	else
	{
		m_scale = m_zoomFactor / oldZoomFactor;
		zoom();
		RedrawWindow();
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CBSplineView::zoom(void)
{	
	Matrix zoomMatrix = Scale(m_zoomCenter, m_scale);
	transform = transform * zoomMatrix;	
}

void CBSplineView::pan(void)
{	
	Matrix panMatrix = Translate(m_xPanDist, m_yPanDist, 0.0);
	transform = transform * panMatrix;
}

void CBSplineView::rotate(void)
{
	Matrix rotationMatrix = Rotate(m_rotationCenter, m_rotationAxis, m_rotationAngle);
	transform = transform * rotationMatrix;
}

void CBSplineView::setSelection(CPoint& point)
{
	CBSplineDoc* pDoc = GetDocument();
	pDoc->m_selectedPt = -1;
	CPoint ctrlPt;
	for(int i = 0; i < pDoc->pBSplineData->m_controlPoints.size(); i++)
	{
		//ctrlPt.x = pDoc->pBSplineData->m_controlPoints[i].m_x;
		//ctrlPt.y = pDoc->pBSplineData->m_controlPoints[i].m_y;
		//ctrlPt = calcGeomToScreen(GeomPoint(ctrlPt));
		ctrlPt = calcGeomToScreen(pDoc->pBSplineData->m_controlPoints[i]);

		if((ctrlPt.x - 4) < point.x && point.x < (ctrlPt.x + 4)
			&& (ctrlPt.y - 4) < point.y && point.y < (ctrlPt.y + 4))
		{
			pDoc->m_selectedPt = i;
			break;
		}		
	}
}

void CBSplineView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{	
	CChildFrame* pChildFrame = (CChildFrame*)this->GetParentFrame();
	CMainFrame* pMainFrame = (CMainFrame*)pChildFrame->GetParentFrame();
	pMainFrame->pSettingsDlg->pView = this;

	CBSplineDoc* pDoc = GetDocument();
	pMainFrame->pSettingsDlg->pDoc = pDoc;

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CBSplineView::OnBSplineNew()
{
	CBSplineDoc* pDoc = GetDocument();
	pDoc->pBSplineData->m_controlPoints.clear();
	pDoc->pBSplineData->m_knotSequence.clear();
	pDoc->pBSplineData->m_degree = 3;

	pDoc->m_selectedPt = -1;
	addingCtrlPts = TRUE;

	insertPointAt = -1;

	m_zoomFactor = 1.0;
	m_scale = 1.0;
	m_zoomIncrement = 0.05;

	isPanning = FALSE;
	m_xPanDist = 0.0;
	m_yPanDist = 0.0;

	resetBBox();

	Invalidate();

	CChildFrame* pChildFrame = (CChildFrame*)this->GetParentFrame();
	CMainFrame* pMainFrame = (CMainFrame*)pChildFrame->GetParentFrame();
	pMainFrame->pSettingsDlg->ShowWindow(FALSE);

}

void CBSplineView::OnBSplineDraw()
{
	addingCtrlPts = FALSE;
	CBSplineDoc* pDoc = GetDocument();
	pDoc->pBSplineData->CalculateKnotVector();
	Invalidate();

	CChildFrame* pChildFrame = (CChildFrame*)this->GetParentFrame();
	CMainFrame* pMainFrame = (CMainFrame*)pChildFrame->GetParentFrame();
	pMainFrame->pSettingsDlg->ShowWindow(TRUE);
	pMainFrame->pSettingsDlg->UpdateData(FALSE);
}

void CBSplineView::OnHelpCommandsInfo()
{
	CDialog dlg(IDD_DIALOG_HELP);
	dlg.DoModal();
}

void CBSplineView::OnHelpAbout()
{
	::AfxMessageBox("Not yet released :-)");
}

void CBSplineView::moveGeomToCenter()
{
	//translate to match screen center with BBox center
	GeomPoint center = m_bbox.GetCenter();
	CPoint cen = calcGeomToScreen(center);
	CPoint screenCenter((m_areaLeft + m_areaRight)/2, (m_areaTop + m_areaBottom)/2);
	transform.addTranslation((screenCenter.x - cen.x), (screenCenter.y - cen.y), 0.0);
}

void CBSplineView::scaleToFit()
{
	//scale to fit entire geom in screen
	GeomPoint minGeom = m_bbox.GetMin();
	CPoint min = calcGeomToScreen(minGeom);
	GeomPoint maxGeom = m_bbox.GetMax();
	CPoint max = calcGeomToScreen(maxGeom);
		
	long xdist = abs(max.x - min.x);
	double xfactor = (m_areaRight - m_areaLeft)/xdist;

	long ydist = abs(max.y - min.y);
	double yfactor = (m_areaBottom - m_areaTop)/xdist;

	double scale = xfactor < yfactor ? xfactor : yfactor;
	transform.addScale(scale);
}

void CBSplineView::OnViewFront()
{
	resetTransform();

	moveGeomToCenter();
	scaleToFit();	
	//redraw
	RedrawWindow();
}

void CBSplineView::OnViewTop()
{
	//transform = RotateAroundStandardAxis(axisX, PI) * RotateAroundStandardAxis(axisX, halfPI);
	transform = RotateAroundStandardAxis(axisX, -halfPI);

	moveGeomToCenter();
	scaleToFit();	
	//redraw
	RedrawWindow();
}

void CBSplineView::OnViewRHV()
{
	transform = RotateAroundStandardAxis(axisX, PI) * RotateAroundStandardAxis(axisY, -halfPI);

	moveGeomToCenter();
	scaleToFit();	
	//redraw
	RedrawWindow();
}

void CBSplineView::OnViewLHV()
{
	transform = RotateAroundStandardAxis(axisX, PI) * RotateAroundStandardAxis(axisY, halfPI);

	moveGeomToCenter();
	scaleToFit();	
	//redraw
	RedrawWindow();
}

void CBSplineView::OnViewIsometric()
{
	transform = RotateAroundStandardAxis(axisX, PI);


	moveGeomToCenter();
	scaleToFit();	
	//redraw
	RedrawWindow();
}

void CBSplineView::OnViewCustom()
{
	if(NULL == pCustomViewDlg)
	{
		pCustomViewDlg = new CustomView(this);
		//pSettingsDlg->pView = this;
		pCustomViewDlg->Create(IDD_DIALOG_VIEW_CUSTOM);
		pCustomViewDlg->ShowWindow(TRUE);
	}

	//redraw
	RedrawWindow();
}

void CBSplineView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CRect r;
	GetClientRect(&r);
	ClientToScreen(&r);

	CBSplineDoc* pDoc = GetDocument();

	if(r.PtInRect(point) && pDoc->m_selectedPt != -1)
	{
		CMenu mymenu, casmenu;

		mymenu.CreatePopupMenu();
		casmenu.CreatePopupMenu();

		mymenu.AppendMenu(MF_STRING, 501, "&Delete This Point");

		mymenu.AppendMenu(MF_POPUP, (UINT)casmenu.GetSafeHmenu( ), "&Insert Point");		
		casmenu.AppendMenu(MF_STRING, 601, "&After This Point");
		casmenu.AppendMenu(MF_STRING, 602, "&Before This Point");

		mymenu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, 0);
	}
	else
		CWnd::OnContextMenu(pWnd, point);	
}

void CBSplineView::OnDeletePoint()
{
	CBSplineDoc* pDoc = GetDocument();
	if(pDoc->m_selectedPt == -1)
		return;
	
	std::vector<GeomPoint>::iterator it = pDoc->pBSplineData->m_controlPoints.begin();
	it += pDoc->m_selectedPt;

	pDoc->pBSplineData->m_controlPoints.erase(it);
	pDoc->pBSplineData->MakeValid(TRUE);

	RedrawWindow();
	updateSettingsDlg();
}

void CBSplineView::OnInsertPoint(UINT ID)
{
	CBSplineDoc* pDoc = GetDocument();
	if(pDoc->m_selectedPt == -1)
		return;

	switch(ID)
	{
	case 601:
		insertPointAt = pDoc->m_selectedPt + 1;
		break;
	case 602:
		insertPointAt = pDoc->m_selectedPt;
		break;
	}
}

void CBSplineView::updateSettingsDlg(void)
{
	CChildFrame* pChildFrame = (CChildFrame*)this->GetParentFrame();
	CMainFrame* pMainFrame = (CMainFrame*)pChildFrame->GetParentFrame();
	pMainFrame->pSettingsDlg->Invalidate();
}

void CBSplineView::setStaturBarMsg(CString& str)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = &pMainFrame->m_wndStatusBar;
	pStatusBar->SetPaneText(0, str);
}

void CBSplineView::setStaturBarMsg2(CString& str)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = &pMainFrame->m_wndStatusBar;
	pStatusBar->SetPaneText(1, str);
}


void CBSplineView::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
	
	CString str("Select and Drag knot points on dialogue to move it. The selected knot is highlighted in BLUE on screen.");
	setStaturBarMsg(str);	
}

CPoint CBSplineView::calcGeomToScreen(GeomPoint& ptIn)
{
	GeomPoint pt(ptIn);
	pt = pt * transform;
	return CPoint(pt.m_x, pt.m_y);
}

GeomPoint CBSplineView::calcScreenToGeom(CPoint& ptIn)
{
	GeomPoint pt(ptIn);
	pt = pt * transform.inverse();
	return pt;
}

GeomPoint CBSplineView::calcScreenToGeom(GeomPoint& ptIn)
{
	GeomPoint pt(ptIn);
	pt = pt * transform.inverse();
	return pt;
}

void CBSplineView::drawLine(CPoint& fromPt, CPoint& toPt)
{
	CClientDC dcPoly(this);
	dcPoly.MoveTo(fromPt);
	dcPoly.LineTo(toPt);
}
