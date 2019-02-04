
#if !defined(AFX_MATRIXOPERATIONS_H__ADB02D0F_E373_4DBF_A367_1FA96411A3EE__INCLUDED_)
#define AFX_MATRIXOPERATIONS_H__ADB02D0F_E373_4DBF_A367_1FA96411A3EE__INCLUDED_

const double Tolerance = 0.0000000001;
const double TwoPI = 6.283185307179587;
const double PI = 3.141592653589793;
const double halfPI = 1.570796326794897;

class Matrix
{
public:
	Matrix(int row = 4, int column = 4);

	Matrix transpose();
	double determinant();
	Matrix inverse();

	void addTranslation(double x, double y, double z);
	void getTranslation(double& x, double& y, double& z);

	void addScale(double scale);
	double getScale(void);

	void removeTranslation();
	void removeScaling();

	void reset();
	void initialise(int row = 4, int column = 4);
	BOOL isUnitMatrix(void);

	friend const Matrix operator* (const Matrix& Matrix1, const Matrix& Matrix2);
	//void operator*= (const Matrix& Matrix2);

	double m_val[4][4];
	int m_row;
	int m_column;
};

class GeomPoint
{
public:
	GeomPoint(double x = 0.0, double y = 0.0, double z = 0.0)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	GeomPoint(CPoint pt)
	{
		m_x = pt.x;
		m_y = pt.y;
		m_z = 0.0;
	}

	GeomPoint(const GeomPoint& pt)
	{
		*this = pt;
	}

	~GeomPoint()
	{
	}

	GeomPoint operator* (Matrix& mat);

	BOOL operator==(const GeomPoint& rhs)
	{
		return(fabs(m_x - rhs.m_x) < Tolerance 
			&& fabs(m_y - rhs.m_y) < Tolerance 
			&& fabs(m_z - rhs.m_z) < Tolerance);
	}

	double distanceFrom(const GeomPoint& rhs)
	{
		return (sqrt(m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z));
	}

//private:
	double m_x;
	double m_y;
	double m_z;
};


class GeomVector : public GeomPoint
{
public:
	GeomVector(double x = 0.0, double y = 0.0, double z = 0.0) : GeomPoint(x, y, z)
	{
	}

	GeomVector(GeomPoint &Pt) : GeomPoint(Pt)
	{
	}

	GeomVector(GeomPoint &startPt, GeomPoint &endPt) 
		: GeomPoint(endPt.m_x - startPt.m_x, endPt.m_y - startPt.m_y, endPt.m_z - startPt.m_z)
	{
	}

	GeomVector operator-()
	{
		return GeomVector(-m_x, -m_y, -m_z);
	}

	BOOL operator==(const GeomVector& rhs)
	{
		return(fabs(m_x - rhs.m_x) < Tolerance 
			&& fabs(m_y - rhs.m_y) < Tolerance 
			&& fabs(m_z - rhs.m_z) < Tolerance);
	}

	double angle(GeomVector vect, GeomVector refVect = GeomVector())
	{
		GeomVector unitVect1 = getUnitVector();
		GeomVector unitVect2 = vect.getUnitVector();
		double dot = unitVect1.dot(unitVect2);

		//Safety check
		if(dot > 1.0)
			dot = 1.0;
		else if(dot < -1.0)
			dot = -1.0;

		double angle = acos(dot);

		if(refVect.isNULL())
			return angle;

		GeomVector CP = unitVect1.cross(unitVect2);
		double refDot = refVect.dot(CP);
		if(refDot < 0.0)
			angle = TwoPI - angle;

		return angle;
	}

	BOOL isNULL()
	{
		return(fabs(m_x) < Tolerance && fabs(m_y) < Tolerance && fabs(m_z) < Tolerance);
	}

	double magnitude()
	{
		return(fabs(sqrt(m_x * m_x + m_y * m_y + m_z * m_z)));
	}

	double dot(GeomVector vect)
	{
		return(m_x * vect.m_x + m_y * vect.m_y + m_z * vect.m_z);
	}


/* Cross Product
	determinant of:
	i	j	k
	x1	y1	z1
	x2	y2	z2

	=i(y1z2-z1y2)-j(x1z2-z1x2)+k(x1y2-y1x2)

*/
	GeomVector cross(GeomVector vect)
	{
		double x = m_y * vect.m_z - m_z * vect.m_y;
		double y = - (m_x * vect.m_z - m_z * vect.m_x);
		double z = m_x * vect.m_y - m_y * vect.m_x;
		return GeomVector(x, y, z);
	}

	GeomVector getUnitVector()
	{
		if(isNULL())
			return *this;

		double mag = magnitude();
		return GeomVector(m_x / mag, m_y / mag, m_z / mag);
	}

	GeomVector operator* (Matrix& mat) const
	{
		GeomPoint pt(m_x, m_y, m_z);
		//while applying transformation to vector, translation should be removed
		//Assume that we are transforming a line from origine to end pt...the directions
		//should remain same
		Matrix matTemp(mat);
		matTemp.removeTranslation();
		pt = pt * matTemp;
		return GeomVector(pt.m_x, pt.m_y, pt.m_z);
	}

public:
	static const GeomVector stdVectX;
	static const GeomVector stdVectY;
	static const GeomVector stdVectZ;
};

enum StandardAxis
{
	axisX = 1,
	axisY = 2,
	axisZ = 3,
	axisSkew = 4
};

Matrix Translate(double x, double y, double z);
Matrix Scale(GeomPoint center, double scale);
Matrix Rotate(GeomPoint center, GeomVector axis, double angle);
Matrix RotateAroundStandardAxis(StandardAxis stdAxis, double angle);

#endif // !defined(AFX_MATRIXOPERATIONS_H__ADB02D0F_E373_4DBF_A367_1FA96411A3EE__INCLUDED_)