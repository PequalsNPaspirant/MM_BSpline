// CustomView.cpp : implementation file
//

#include "stdafx.h"
#include "BSpline.h"
#include "CustomView.h"
//#include ".\customview.h"
#include "BSplineView.h"

// CustomView dialog

IMPLEMENT_DYNAMIC(CustomView, CDialog)
CustomView::CustomView(CWnd* pParent /*=NULL*/)
	: CDialog(CustomView::IDD, pParent),
	pView((CBSplineView*)pParent),
	animationOn(TRUE),
	stdAxis(axisX),
	rotationCenter(0.0, 0.0, 0.0),
	rotationAxis(1.0, 1.0, 1.0),
	rotationAngle(0.0),
	steps(1)
{
	pView = (CBSplineView*)pParent;

	//CComboBox bx();
	//bx.
}

CustomView::~CustomView()
{
}

void CustomView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettings)
	DDX_Text(pDX, IDC_EDIT_ANGLE, rotationAngle);
	DDX_Check(pDX, IDC_CHECK_ANIMATE, animationOn);
	DDX_Text(pDX, IDC_EDIT_X1, rotationCenter.m_x);
	DDX_Text(pDX, IDC_EDIT_Y1, rotationCenter.m_y);
	DDX_Text(pDX, IDC_EDIT_Z1, rotationCenter.m_z);
	DDX_Text(pDX, IDC_EDIT_X2, rotationAxis.m_x);
	DDX_Text(pDX, IDC_EDIT_Y2, rotationAxis.m_y);
	DDX_Text(pDX, IDC_EDIT_Z2, rotationAxis.m_z);
	DDX_Text(pDX, IDC_EDIT_STEP, steps);
	int tempAxis;
	DDX_CBIndex(pDX, IDC_COMBO_AXIS, tempAxis);
	stdAxis = StandardAxis(tempAxis + 1);
	//}}AFX_DATA_MAP
}

BOOL CustomView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE); //populate dialoge with defauls values of controls

	((CComboBox*)GetDlgItem(IDC_COMBO_AXIS))->SetCurSel(stdAxis);

	GetDlgItem(IDC_EDIT_X2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Y2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Z2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_STEP)->EnableWindow(TRUE);

	//temporory fix. Need to work on
	GetDlgItem(IDOK)->ShowWindow(FALSE);
	GetDlgItem(IDCANCEL)->ShowWindow(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CustomView, CDialog)
	//{{AFX_MSG_MAP(CustomView)
	ON_BN_CLICKED(ID_RUN, OnBnClickedRun)
	ON_CBN_SELCHANGE(IDC_COMBO_AXIS, OnCbnSelchangeComboAxis)	
	ON_BN_CLICKED(IDC_CHECK_ANIMATE, OnBnClickedCheckAnimate)
	ON_EN_CHANGE(IDC_EDIT_ANGLE, OnEnChangeEditAngle)
	ON_EN_CHANGE(IDC_EDIT_X1, OnEnChangeEditX1)
	ON_EN_CHANGE(IDC_EDIT_Y1, OnEnChangeEditY1)
	ON_EN_CHANGE(IDC_EDIT_Z1, OnEnChangeEditZ1)
	ON_EN_CHANGE(IDC_EDIT_X2, OnEnChangeEditX2)
	ON_EN_CHANGE(IDC_EDIT_Y2, OnEnChangeEditY2)
	ON_EN_CHANGE(IDC_EDIT_Z2, OnEnChangeEditZ2)
	ON_EN_CHANGE(IDC_EDIT_STEP, OnEnChangeEditStep)
	
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CustomView message handlers

void CustomView::OnBnClickedRun()
{	
	switch(stdAxis)
	{
	case axisX:
		pView->m_rotationAxis = GeomVector::stdVectX * pView->transform;
		break;
	case axisY:
		pView->m_rotationAxis = GeomVector::stdVectY * pView->transform;
		break;
	case axisZ:
		pView->m_rotationAxis = GeomVector::stdVectZ * pView->transform;
		break;
	case axisSkew:
		pView->m_rotationAxis = rotationAxis * pView->transform;
		//pView->m_rotationAxis = rotationAxis; //Direction does not get transformed
		//Need to apply initial transformation
		//pView->m_rotationAxis = rotationAxis * RotateAroundStandardAxis(axisX, PI);
		//All above is wrong....there was a bug in "GeomVector operator* (Matrix& mat)"
		//Read comments in above function
		break;
	}
	pView->m_rotationCenter = rotationCenter * pView->transform;
	double angle = rotationAngle * PI / 180.0;
	if(!animationOn)
		steps = 1;

	pView->m_rotationAngle = angle / steps;
	//CPoint fromPt(pView->m_rotationCenter.m_x, pView->m_rotationCenter.m_y);
	//CPoint toPt(fromPt.x + pView->m_rotationAxis.m_x * 50, fromPt.y + pView->m_rotationAxis.m_y * 50);
	for(int i = 0; i < steps; i++)
	{
		pView->rotate();
		pView->RedrawWindow();
		//pView->drawLine(fromPt, toPt);
		::Sleep(100);
	}
}

void CustomView::OnCbnSelchangeComboAxis()
{
	// TODO: Add your control notification handler code here

	UpdateData();

	if(stdAxis == axisSkew)
	{
		GetDlgItem(IDC_EDIT_X2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Y2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_Z2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_X2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Y2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_Z2)->EnableWindow(FALSE);
	}
}

void CustomView::OnBnClickedCheckAnimate()
{
	UpdateData();

	if(animationOn)
	{
		GetDlgItem(IDC_EDIT_STEP)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_STEP)->EnableWindow(FALSE);
	}
}

void CustomView::OnEnChangeEditAngle()
{
	UpdateData();

	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CustomView::OnEnChangeEditX1()
{
	UpdateData();
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CustomView::OnEnChangeEditY1()
{
	UpdateData();
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CustomView::OnEnChangeEditZ1()
{
	UpdateData();
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CustomView::OnEnChangeEditX2()
{
	UpdateData();
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CustomView::OnEnChangeEditY2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CustomView::OnEnChangeEditZ2()
{
	UpdateData();
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CustomView::OnEnChangeEditStep()
{
	UpdateData();
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
void CustomView::OnBnClickedOk()
{	
	//delete pView->pCustomViewDlg;
	//pView->pCustomViewDlg = NULL;
	//OnOK();	
}

void CustomView::OnBnClickedCancel()
{
	//OnCancel();

	//delete pView->pCustomViewDlg;
	//pView->pCustomViewDlg = NULL;
}
