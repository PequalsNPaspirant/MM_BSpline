#pragma once

#include "MatrixOperations.h"

class CBoundingBox
{
public:
	CBoundingBox(void);
	virtual ~CBoundingBox(void);
	
	void Add(GeomPoint pt);
	GeomPoint GetCenter();
	void Reset()
	{	
		m_minPoint.m_x = 0.0;
		m_minPoint.m_y = 0.0;
		m_minPoint.m_z = 0.0;

		m_maxPoint.m_x = 0.0;
		m_maxPoint.m_y = 0.0;
		m_maxPoint.m_z = 0.0;
    }
	
	GeomPoint GetMin()
	{
		return m_minPoint;
	}

	GeomPoint GetMax()
	{
		return m_maxPoint;
	}

protected:

private:
	GeomPoint m_minPoint;
	GeomPoint m_maxPoint;
};
