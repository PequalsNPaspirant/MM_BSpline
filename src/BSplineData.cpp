// BSplineData.cpp: implementation of the BSplineData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BSpline.h"
#include "BSplineData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BSplineData::BSplineData(int degree /*= 0*/)
{
	m_degree = degree;
}

BSplineData::~BSplineData()
{

}

GeomPoint BSplineData::GetPointOnCurve(double parameter)
{
	GeomPoint pt;

	int noOfCtrlPts = m_controlPoints.size();
	for(int i = 0; i < noOfCtrlPts; i++)
	{
		double basisVal = CalculateBasisValue(i, m_degree, parameter);

		pt.m_x += basisVal * m_controlPoints[i].m_x;

		pt.m_y += basisVal * m_controlPoints[i].m_y;

		pt.m_z += basisVal * m_controlPoints[i].m_z;
	}

	return pt;
}

double BSplineData::CalculateBasisValue(int CtrlPtIndex, int degree, double parameter)
{
	if(degree == 0)
	{
		if(m_knotSequence[CtrlPtIndex] <= parameter && parameter < m_knotSequence[CtrlPtIndex + 1])
			return 1.0;
		else
			return 0.0;
	}

	double division = 0.0;

	double numerator = (parameter - m_knotSequence[CtrlPtIndex]);
	double denominator = (m_knotSequence[CtrlPtIndex + degree] - m_knotSequence[CtrlPtIndex]);
	if(denominator != 0)
		division = numerator / denominator;
	double firstPart = division * CalculateBasisValue(CtrlPtIndex, (degree - 1), parameter);

	numerator = (m_knotSequence[CtrlPtIndex + degree + 1] - parameter);
	denominator = (m_knotSequence[CtrlPtIndex + degree + 1] - m_knotSequence[CtrlPtIndex + 1]);
	if(denominator != 0)
		division = numerator / denominator;
	double secondPart = division * CalculateBasisValue((CtrlPtIndex + 1), (degree - 1), parameter);

	return (firstPart + secondPart);
}

void BSplineData::CalculateKnotVector(void)
{
	int noOfCtrlPts = m_controlPoints.size();
	if(noOfCtrlPts < 2)
		return;

	if(noOfCtrlPts <= m_degree)
	{
		m_degree = noOfCtrlPts - 1;
	}
	
	int noOfKnotsReq = noOfCtrlPts + m_degree + 1;

	m_knotSequence.clear();
	double knotVal = 0.0;
	for(int i = 0; i < noOfKnotsReq; i++)
	{
		//To make curve pass thru end ctrl pts
		if(i <= m_degree)
			knotVal = 0.0;
		else if(i >= noOfCtrlPts) // noOfCtrlPts = noOfKnotsReq - pBSplineData->m_degree - 1
			knotVal = 1.0;
		else
		{			
			knotVal = double(i) / noOfKnotsReq;
		}
		m_knotSequence.push_back(knotVal);
	}
}

void BSplineData::MakeValid(BOOL adjustKnots /*= TRUE*/)
{
	int noOfCtrlPts = m_controlPoints.size();
	int noOfKnots = m_knotSequence.size();

	int lhs = noOfKnots - 1;
	int rhs = noOfCtrlPts + m_degree;		

	if(lhs == rhs)
		return;

	if(adjustKnots == TRUE)
	{
		if(lhs < rhs)
		{
			for(int i = 0; i < (rhs - lhs); i++)
				m_knotSequence.push_back(1.0);
		}
		else
		{
			for(int i = 0; i < (lhs - rhs); i++)
				m_knotSequence.pop_back();
		}
	}
	else
	{
		m_degree += (lhs - rhs);
	}	
}