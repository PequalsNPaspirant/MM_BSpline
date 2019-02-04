// BSplineData.h: interface for the BSplineData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BSPLINEDATA_H__ADB02D0F_E373_4DBF_A367_1FA96411A3EE__INCLUDED_)
#define AFX_BSPLINEDATA_H__ADB02D0F_E373_4DBF_A367_1FA96411A3EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PRECISION (0.001)

#include "MatrixOperations.h"

class BSplineData  
{
public:
	BSplineData(int degree = 0);

	virtual ~BSplineData();

	GeomPoint GetPointOnCurve(double parameter);
	double CalculateBasisValue(int CtrlPtIndex, int degree, double parameter);
	void MakeValid(BOOL adjustKnots = TRUE);

//protected:
	int m_degree;
	std::vector<double> m_knotSequence;
	std::vector<GeomPoint> m_controlPoints;

	void CalculateKnotVector(void);

};

#endif // !defined(AFX_BSPLINEDATA_H__ADB02D0F_E373_4DBF_A367_1FA96411A3EE__INCLUDED_)
