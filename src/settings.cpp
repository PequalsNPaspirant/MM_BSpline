// settings.cpp : implementation file
//

#include "stdafx.h"
#include "BSpline.h"
#include "settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BSplineView.h"
#include "BSplineDoc.h"
#include "BSplineData.h"

/////////////////////////////////////////////////////////////////////////////
// CSettings dialog

CSettings::CSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettings)
//	m_degree = 3;
	//}}AFX_DATA_INIT

//	m_selectedKnot = -1;

	pView = (CBSplineView*)pParent;
	pDoc = pView ? pView->GetDocument() : NULL;

	m_knotPathStart = 0;
	m_knotPathEnd = 0;
}

void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettings)
	if(pDoc && pDoc->pBSplineData)
		DDX_Text(pDX, IDC_EDIT1, pDoc->pBSplineData->m_degree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSettings, CDialog)
	//{{AFX_MSG_MAP(CSettings)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettings message handlers

void CSettings::OnOK()
{
}

void CSettings::OnCancel()
{
}

BOOL CSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	return TRUE;
}

void CSettings::OnPaint() 
{
	CPaintDC dc(this);

	CRect rect;
	this->GetClientRect(&rect);

	COLORREF clr = RGB(192, 192, 192);
	CBrush brush(clr);
	dc.FillRect(&rect, &brush);
			
	long clearance = 5;
	long topClearance = 25;
	
	LONG areaLeft = rect.left + clearance;
	LONG areaRight = rect.right - clearance;
	LONG areaTop = rect.top + clearance + topClearance;
	LONG areaBottom = rect.bottom - clearance;

	//Always Draw border, so use dc, NOT pDC
	dc.MoveTo ( CPoint(areaLeft, areaTop) ) ;
	dc.LineTo ( CPoint(areaRight, areaTop) ) ;
	dc.LineTo ( CPoint(areaRight, areaBottom) ) ;
	dc.LineTo ( CPoint(areaLeft, areaBottom) ) ;
	dc.LineTo ( CPoint(areaLeft, areaTop) ) ;

	iDraw();
}

void CSettings::OnChangeEdit1() 
{
	UpdateData(TRUE);

	if(pDoc->pBSplineData->m_degree < 1 || pDoc->pBSplineData->m_degree > 9)
	{
		pDoc->pBSplineData->m_degree = 2;
		AfxMessageBox("Supports degree in the range 1 to 9");
		return;
	}

	pDoc->pBSplineData->MakeValid(TRUE);

	Invalidate();
	pView->Invalidate();
}

void CSettings::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	//Erase old selected knot
	iDraw(TRUE, FALSE);

	pDoc->m_selectedKnot = -1;
	for(int i = 0; i < m_knotPoints.size(); i++)
	{
		if((m_knotPoints[i].x - 4) < point.x && point.x < (m_knotPoints[i].x + 4)
			&& (m_knotPoints[i].y - 10) < point.y && point.y < (m_knotPoints[i].y + 10))
		{			
			pDoc->m_selectedKnot = i;
			iDraw();			
			break;
		}		
	}

	pView->RedrawWindow();
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CSettings::OnLButtonUp(UINT nFlags, CPoint point) 
{	
//	pDoc->m_selectedKnot = -1;
	CDialog::OnLButtonUp(nFlags, point);
}

void CSettings::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if ( nFlags == MK_LBUTTON && (pDoc->m_selectedKnot != -1))
	{
		double lowerLimit = 0.0;
		double upperLimit = 1.0;
		if(pDoc->m_selectedKnot > 0)
			lowerLimit = pDoc->pBSplineData->m_knotSequence[pDoc->m_selectedKnot - 1];
		if(pDoc->m_selectedKnot < pDoc->pBSplineData->m_knotSequence.size() - 1)
			upperLimit = pDoc->pBSplineData->m_knotSequence[pDoc->m_selectedKnot + 1];

		double val = double(point.x - m_knotPathStart) / (m_knotPathEnd - m_knotPathStart);
		if(lowerLimit <= val && val <= upperLimit)
		{
			//Erase old horizontal and vertical lines
			iDraw(FALSE);

			pDoc->pBSplineData->m_knotSequence[pDoc->m_selectedKnot] = val;

			iDraw();

			pView->RedrawWindow();
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CSettings::iDraw(BOOL draw /*= TRUE*/, BOOL selectKnot /*= TRUE*/)
{
	if(NULL == pDoc)
		return;

	CClientDC dcKnot(this);
	dcKnot.SetBkMode(TRANSPARENT);
	CFont fontKnot;
	fontKnot.CreateFont(15, 5, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	dcKnot.SelectObject(&fontKnot);
	
	COLORREF clr = RGB(128, 0, 0);
	if(draw == FALSE) //Erase all
	{
		clr = RGB(192, 192, 192); //background colour
		selectKnot = FALSE;
	}
	CPen pen(0, 1, clr);
	CPen* pCurrPen = dcKnot.SelectObject(&pen);
	dcKnot.SetTextColor(clr);

	CRect rect;
	this->GetClientRect(&rect);
		
	long clearance = 5;
	long topClearance = 25;
	
	LONG areaLeft = rect.left + clearance;
	LONG areaRight = rect.right - clearance;
	LONG areaTop = rect.top + clearance + topClearance;
	LONG areaBottom = rect.bottom - clearance;

	long halfVertLine = 5;	

	CRect textRect;
	CString str;
	long textBoxHeight = 15;
	long textBoxWidth = 45;
	UINT textStyle = 0;
	textStyle |= DT_TOP | DT_CENTER;

	m_knotPathStart = areaLeft + 2 * clearance + textBoxWidth;
	m_knotPathEnd = areaRight - clearance - textBoxWidth / 2;
	
	//Special Font for Knot no.s
	CClientDC dc(this);
	dc.SetBkMode(TRANSPARENT);
	CFont font;
	font.CreateFont(15, 5, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	dc.SelectObject(&font);

	int size = pDoc->pBSplineData->m_knotSequence.size();
	CPoint pt;
	for(int i = 0; i < size; i++)
	{
		if(pDoc->m_selectedKnot != -1)
			i = pDoc->m_selectedKnot;
		else if(draw == FALSE)
			break;

		pt.x = m_knotPathStart + (m_knotPathEnd - m_knotPathStart) * pDoc->pBSplineData->m_knotSequence[i];
		pt.y = areaTop + (i + 1) * (halfVertLine + textBoxHeight + 2 * clearance);
		if(m_knotPoints.size() > i)
			m_knotPoints[i] = pt;			
		else
			m_knotPoints.push_back(pt);			

		if(pDoc->m_selectedKnot == -1)
		{
			//Draw knot Index
			textRect.TopLeft() = CPoint(m_knotPathStart - textBoxWidth, m_knotPoints[i].y - textBoxHeight * 0.5);
			textRect.BottomRight() = CPoint(m_knotPathStart, m_knotPoints[i].y + textBoxHeight * 0.5);
			str.Format("Knot %d", i);
			dc.DrawText(str, &textRect, DT_VCENTER | DT_LEFT);
		}

		//Draw Horizontal line
		dc.MoveTo ( CPoint(m_knotPathStart, m_knotPoints[i].y) ) ;
		dc.LineTo ( CPoint(m_knotPathEnd, m_knotPoints[i].y) ) ;

		//Draw circle at knot position
//		dcKnot.MoveTo(CPoint(m_knotPoints[i].x + halfVertLine, m_knotPoints[i].y));
//		dcKnot.AngleArc(m_knotPoints[i].x, m_knotPoints[i].y, halfVertLine, 0, 360);
		drawCircle(&dcKnot, m_knotPoints[i], halfVertLine, selectKnot && pDoc->m_selectedKnot != -1);

		//Draw vertical line showing position of knot
		dcKnot.MoveTo ( CPoint(m_knotPoints[i].x, m_knotPoints[i].y - halfVertLine) ) ;
		dcKnot.LineTo ( CPoint(m_knotPoints[i].x, m_knotPoints[i].y + halfVertLine) ) ;

		//Draw knot value		
		str.Format("%.3lf", pDoc->pBSplineData->m_knotSequence[i]);
		
//		textRect.TopLeft() = CPoint(m_knotPoints[i].x - textBoxWidth, m_knotPoints[i].y - textBoxHeight);
//		textRect.BottomRight() = CPoint(m_knotPoints[i].x, m_knotPoints[i].y);
		textRect.TopLeft() = CPoint(m_knotPoints[i].x - textBoxWidth / 2, m_knotPoints[i].y - textBoxHeight - halfVertLine);
		textRect.BottomRight() = CPoint(m_knotPoints[i].x + textBoxWidth / 2, m_knotPoints[i].y - halfVertLine);
//		if(m_knotPathStart > textRect.left)
//		{
//			textRect.left = textRect.right;
//			textRect.right += textBoxWidth;
//		}
		dcKnot.DrawText(str, &textRect, textStyle);

		if(pDoc->m_selectedKnot != -1)
			break;
	}
}

void CSettings::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	Invalidate();
}

void CSettings::drawCircle(CDC* pDC, CPoint &center, double radius, BOOL fill /*= FALSE*/)
{
	if(NULL == pDC)
		return;

	radius += 2;
	CRgn rgn;
	rgn.CreateEllipticRgn(center.x - radius, center.y - radius, center.x + radius, center.y + radius);

//	CBrush brush(RGB(0, 0, 255));
//	if(fill == FALSE)//Fill with background colour
//		brush.CreateSolidBrush(RGB(192, 192, 192));
//	pDC->FillRgn(&rgn, &brush);
	
	CBrush brush1(RGB(0, 0, 255));
	CBrush brush2(RGB(192, 192, 192));
	if(fill == TRUE)
		pDC->FillRgn(&rgn, &brush1);
	else
		pDC->FillRgn(&rgn, &brush2);

	radius -= 2;
	pDC->MoveTo(CPoint(center.x + radius, center.y));
	pDC->AngleArc(center.x, center.y, radius, 0, 360);
}