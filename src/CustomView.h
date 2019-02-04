#pragma once

#include "matrixoperations.h"

// CustomView dialog
class CBSplineView;
class CustomView : public CDialog
{
	DECLARE_DYNAMIC(CustomView)

public:
	CustomView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CustomView();

	CBSplineView* pView;

	BOOL animationOn;
	StandardAxis stdAxis;
	GeomPoint rotationCenter;
	GeomVector rotationAxis;
	double rotationAngle;
	int steps;

// Dialog Data
	enum { IDD = IDD_DIALOG_VIEW_CUSTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRun();
	afx_msg void OnCbnSelchangeComboAxis();
	afx_msg void OnBnClickedCheckAnimate();
	afx_msg void OnEnChangeEditAngle();
	afx_msg void OnEnChangeEditX1();
	afx_msg void OnEnChangeEditY1();
	afx_msg void OnEnChangeEditZ1();
	afx_msg void OnEnChangeEditX2();
	afx_msg void OnEnChangeEditY2();
	afx_msg void OnEnChangeEditZ2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditStep();
};
