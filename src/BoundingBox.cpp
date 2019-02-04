#include "stdafx.h"
#include ".\boundingbox.h"

CBoundingBox::CBoundingBox(void)
{
}

CBoundingBox::~CBoundingBox(void)
{
}

void CBoundingBox::Add(GeomPoint pt)
{
	//Check x bound
	if(pt.m_x < m_minPoint.m_x)
		m_minPoint.m_x = pt.m_x;
	else if(pt.m_x > m_maxPoint.m_x)
		m_maxPoint.m_x = pt.m_x;

	//Check y bound
	if(pt.m_y < m_minPoint.m_y)
		m_minPoint.m_y = pt.m_y;
	else if(pt.m_y > m_maxPoint.m_y)
		m_maxPoint.m_y = pt.m_y;

	//Check z bound
	if(pt.m_z < m_minPoint.m_z)
		m_minPoint.m_z = pt.m_z;
	else if(pt.m_z > m_maxPoint.m_z)
		m_maxPoint.m_z = pt.m_z;
}

GeomPoint CBoundingBox::GetCenter()
{
	return GeomPoint((m_minPoint.m_x + m_maxPoint.m_x)/2, (m_minPoint.m_y + m_maxPoint.m_y)/2, (m_minPoint.m_z + m_maxPoint.m_z)/2);
}



