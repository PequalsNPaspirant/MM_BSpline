/*
Matrix m

   Rotation:
    _______________________________
   |m[0][0]		m[0][1]		m[0][2]|	m[0][3]
   |							   |
   |m[1][0]		m[1][1]		m[1][2]|	m[1][3]
   |							   |
   |m[2][0]		m[2][1]		m[2][2]|	m[2][3]
   |_______________________________|

   |m[3][0]		m[3][1]		m[3][2]		m[3][3]
	|<-----   translation   ----->|		|<--->|
										 Scale
*/

#include "stdafx.h"

#include <math.h>
#include "matrixOperations.h"

const GeomVector GeomVector::stdVectX(1.0, 0.0, 0.0);
const GeomVector GeomVector::stdVectY(0.0, 1.0, 0.0);
const GeomVector GeomVector::stdVectZ(0.0, 0.0, 1.0);

Matrix::Matrix(int row /*= 4*/, int column /*= 4*/)
{
	initialise(row, column);
}

Matrix Matrix::transpose()
{
	Matrix TempMatrix(m_column, m_row);

	for(int Row=0; Row<m_row; Row++)
		for(int Column=0; Column<m_column; Column++)
			TempMatrix.m_val[Row][Column] = m_val[Column][Row];

	return TempMatrix;
}

double Matrix::determinant()
{
	if(m_row != m_column)
		return 0.0;

	if(m_row == 1)
			return(m_val[0][0]);

	int Order = m_row;
	int j, k, R, C;
	double TempResult = 0.0;
	for(int Column=0; Column<Order; Column++)
	{
		Matrix TempMatrix(Order, Order);

		for(R = 0, j=1; j<Order; j++) {
			for(C = 0, k=0; k<Order; k++) {
				if(k == Column) continue;
				TempMatrix.m_val[R][C] = m_val[j][k];
				C++;
			}
			R++;
		}

		double p1 = pow(-1, Column);
		double p2 = m_val[0][Column];
		TempMatrix.m_row = Order-1;
		TempMatrix.m_column = Order-1;
		double p3 = TempMatrix.determinant();
		
		TempResult += p1*p2*p3;
	}

	return(TempResult);
}

const Matrix operator* (const Matrix& Matrix1, const Matrix& Matrix2)
{
	Matrix Result(Matrix1.m_row, Matrix2.m_column);

	if(Matrix1.m_column != Matrix2.m_row)
	{
		ASSERT(0);
		return Result;
	}

	int i, j, k;
	int Row, Column;

	for(i=0; i<Result.m_row; i++)
		for(j=0; j<Result.m_column; j++)
			Result.m_val[i][j] = 0.0;

	for(i=0; i<Result.m_row; i++)
		for(j=0; j<Result.m_column; j++)
			for(k=0; k<Matrix1.m_column; k++) // OR use Matrix2.m_row. Both are equal.
				Result.m_val[i][j] += Matrix1.m_val[i][k]*Matrix2.m_val[k][j];

	for(Row=0; Row<Result.m_row; Row++)
		for(Column=0; Column<Result.m_column; Column++)
			if(fabs(Result.m_val[Row][Column])<Tolerance)
				Result.m_val[Row][Column] = 0.0;

	return Result;
}

/*
can't collect product in the same matrix....dimentions may be different
void Matrix::operator*= (const Matrix& Matrix2)
{
	Matrix Result(Matrix1.m_row, Matrix2.m_column);

	if(Matrix1.m_column != Matrix2.m_row)
	{
		ASSERT(0);
		return Result;
	}

	int i, j, k;
	int Row, Column;

	for(i=0; i<Result.m_row; i++)
		for(j=0; j<Result.m_column; j++)
			Result.m_val[i][j] = 0.0;

	for(i=0; i<Result.m_row; i++)
		for(j=0; j<Result.m_column; j++)
			for(k=0; k<Matrix1.m_column; k++) // OR use Matrix2.m_row. Both are equal.
				Result.m_val[i][j] += Matrix1.m_val[i][k]*Matrix2.m_val[k][j];

	for(Row=0; Row<Result.m_row; Row++)
		for(Column=0; Column<Result.m_column; Column++)
			if(fabs(Result.m_val[Row][Column])<Tolerance)
				Result.m_val[Row][Column] = 0.0;

}
*/

Matrix Matrix::inverse()
{
	Matrix Result(m_row, m_column);

	if(m_row != m_column)
	{
		ASSERT(0);
		return Result;
	}

	int Order = m_row;
	int j, k;
	int Row, Column, R, C;
	double Dmnt = determinant();

	if(fabs(Dmnt) < Tolerance) { // Inverse doesnt exist
		return Result;
	}

	Matrix TempMatrix(Order - 1, Order - 1);

	for(Row=0; Row<Order; Row++)
		for(Column=0; Column<Order; Column++) {

			for(R = 0, j=0; j<Order; j++) {
				if(j == Row) continue;
				for(C = 0, k=0; k<Order; k++) {
					if(k == Column) continue;
					TempMatrix.m_val[R][C] = m_val[j][k];
					C++;
				}
				R++;
			}
			double p = TempMatrix.determinant();
			Result.m_val[Row][Column] = p;

		}

	for(Row=0; Row<Order; Row++)
		for(Column=0; Column<Order; Column++)
				Result.m_val[Row][Column] = (pow(-1, (Row+Column)))*Result.m_val[Row][Column];

	Result = Result.transpose();

	for(Row=0; Row<Order; Row++)
		for(Column=0; Column<Order; Column++)
			Result.m_val[Row][Column] /= Dmnt;

	return Result;
}

void Matrix::addTranslation(double x, double y, double z)
{
	m_val[3][0] += x;
	m_val[3][1] += y;
	m_val[3][2] += z;
}

void Matrix::getTranslation(double& x, double& y, double& z)
{
	x = m_val[3][0];
	y = m_val[3][1];
	z = m_val[3][2];
}

void Matrix::addScale(double scale)
{
	m_val[3][3] *= scale;
}

double Matrix::getScale(void)
{
	return m_val[3][3];
}

void Matrix::removeTranslation()
{
	m_val[3][0] = 0.0;
	m_val[3][1] = 0.0;
	m_val[3][2] = 0.0;
}

void Matrix::removeScaling()
{
	m_val[3][3] = 1;
}

void Matrix::reset()
{
	initialise(m_row, m_column);
}

void Matrix::initialise(int row /*= 4*/, int column /*= 4*/)
{
	m_row = row;
	m_column = column;
	for(int Row=0; Row<4; Row++)
	for(int Column=0; Column<4; Column++)
		if(Row == Column)
			m_val[Row][Column] = 1.0;
		else
			m_val[Row][Column] = 0.0;
}

BOOL Matrix::isUnitMatrix(void)
{
	for(int Row=0; Row<4; Row++)
	for(int Column=0; Column<4; Column++)
		if(Row == Column)
		{
			if(fabs(m_val[Row][Column] - 1.0) > Tolerance)
				return FALSE;
		}
		else
		{
			if(fabs(m_val[Row][Column] - 0.0) > Tolerance)
				return FALSE;
		}

	return TRUE;
}

Matrix Translate(double x, double y, double z)
{
	Matrix mx;

	mx.m_val[3][0] += x;
	mx.m_val[3][1] += y;
	mx.m_val[3][2] += z;

	return mx;
}

Matrix Scale(GeomPoint center, double scale)
{
	Matrix mx1 = Translate(-center.m_x, -center.m_y, -center.m_z);

	Matrix mx2;
	mx2.m_val[3][3] *= scale;

	Matrix mx3 = Translate(center.m_x, center.m_y, center.m_z);

	return (mx1 * mx2 * mx3);
}

Matrix RotateAroundStandardAxis(StandardAxis stdAxis, double angle)
{
	Matrix mat;
	switch(stdAxis)
	{
	case axisX:
		mat.m_val[1][1] = cos(angle);
		mat.m_val[1][2] = sin(angle);
		mat.m_val[2][1] = -sin(angle);
		mat.m_val[2][2] = cos(angle);
		break;
	case axisY:
		mat.m_val[0][0] = cos(angle);
		mat.m_val[0][2] = -sin(angle);
		mat.m_val[2][0] = sin(angle);
		mat.m_val[2][2] = cos(angle);
		break;
	case axisZ:
		mat.m_val[0][0] = cos(angle);
		mat.m_val[0][1] = sin(angle);
		mat.m_val[1][0] = -sin(angle);
		mat.m_val[1][1] = cos(angle);
		break;
	default:
		ASSERT(0);
	}

	return mat;
}

Matrix Rotate(GeomPoint center, GeomVector axis, double angle)
{
	Matrix ret;

	if(fabs(angle) < Tolerance)
		return ret;
	
	BOOL reverseTranslate =  FALSE;

	//bring it to origin
	BOOL centerIsOrigin = TRUE;
	if(fabs(center.m_x) > Tolerance || fabs(center.m_x) > Tolerance || fabs(center.m_x) > Tolerance)
		centerIsOrigin = FALSE;

	Matrix mx1, mx2, mx3, mx4, mx5, mx6, mx7;
	if(centerIsOrigin == FALSE)
	{
		mx1 = Translate(-center.m_x, -center.m_y, -center.m_z);
		ret = ret * mx1;
		reverseTranslate = TRUE;
	}	
	
	//Check if we are rotating around standard axes
	if(axis == GeomVector::stdVectX)
	{
		Matrix mx = RotateAroundStandardAxis(axisX, angle);
		ret = ret * mx;
	}
	else if(axis == GeomVector::stdVectY)
	{
		Matrix mx = RotateAroundStandardAxis(axisY, angle);
		ret = ret * mx;
	}
	else if(axis == GeomVector::stdVectZ)
	{
		Matrix mx = RotateAroundStandardAxis(axisZ, angle);
		ret = ret * mx;
	}
	else
	{
		BOOL reverseRotate1 =  FALSE;
		BOOL reverseRotate2 =  FALSE;

		//Rotate around Z-axis to align axis to YZ plane
		GeomVector axisUnitVect = axis.getUnitVector();
		GeomVector axisProjectedToXY(axisUnitVect.m_x, axisUnitVect.m_y, 0.0);
		double angleWithY = axisProjectedToXY.angle(GeomVector::stdVectY, GeomVector::stdVectZ);
		if(fabs(angleWithY) > Tolerance)
		{
			mx2 = RotateAroundStandardAxis(axisZ, angleWithY);
			ret = ret * mx2;
			reverseRotate1 = TRUE;
		}

		{
			//test: axisProjectedToXY should be aligned to Y-axis i.e. x & z co-ordinates should be zero
			GeomVector dummyProj = axisProjectedToXY * mx2;
			if(fabs(dummyProj.m_x) > Tolerance || fabs(dummyProj.m_z) > Tolerance)
			{
				ASSERT(0);
			}
			//test: now the axis should be in YZ plane i.e. x-co-ordinate should be zero
			GeomVector dummy = axis * mx2;
			if(fabs(dummy.m_x) > Tolerance)
			{
				ASSERT(0);
			}
		}

		//Now the axis is in YZ plane
		//Rotate around X-axis to align axis to Z-axis
		//GeomVector axisProjectedToYZ(0.0, axisUnitVect.m_y, axisUnitVect.m_z);
		GeomVector axisInYZPlane = axis * mx2;
		double angleWithZ = axisInYZPlane.angle(GeomVector::stdVectZ, GeomVector::stdVectX);
		if(fabs(angleWithZ) > Tolerance)
		{
			mx3 = RotateAroundStandardAxis(axisX, angleWithZ);
			ret = ret * mx3;
			reverseRotate2 = TRUE;
		}

		{
			//test: axisInYZPlane should be aligned to Z-axis i.e. x & y co-ordinates should be zero
			GeomVector dummyProj = axisInYZPlane * mx3;
			dummyProj = dummyProj.getUnitVector();
			if(fabs(dummyProj.m_x) > Tolerance || fabs(dummyProj.m_y) > Tolerance)
			{
				ASSERT(0);
			}
			//test: now the axis should be parellel to Z-axis after both rotations i.e. x & y co-ordinates should be zero
			//GeomVector result = axis * Matrix(mx2 * mx3);
			GeomVector result = axis * mx2 * mx3;
			//Matrix res = mx2 * mx3;
			//GeomVector result = axis * res;
			if(fabs(result.m_x) > Tolerance || fabs(result.m_y) > Tolerance)
			{
				ASSERT(0);
			}
		}

		//Now the axis is aligned to Z axis
		//Rotate around Z axis
		if(fabs(angle) > Tolerance)
		{
			mx4 = RotateAroundStandardAxis(axisZ, angle);
			ret = ret * mx4;
		}		

		if(reverseRotate2)
		{
			mx5 = RotateAroundStandardAxis(axisX, -angleWithZ);
			ret = ret * mx5;
		}

        if(reverseRotate1)
		{
			mx6 = RotateAroundStandardAxis(axisZ, -angleWithY);
			ret = ret * mx6;
		}
	}

	if(reverseTranslate)
	{
		mx7 = Translate(center.m_x, center.m_y, center.m_z);
		ret = ret * mx7;
	}

	//verification
	Matrix result = mx1 * mx7;
	if(!result.isUnitMatrix())
	{
		ASSERT(0);
	}

	result = mx2 * mx6;
	if(!result.isUnitMatrix())
	{
		ASSERT(0);
	}

	result = mx3 * mx5;
	if(!result.isUnitMatrix())
	{
		ASSERT(0);
	}

	Matrix res1 = mx2 * mx3;
	Matrix res2 = mx5 * mx6;
	result = res1 * res2;
	if(!result.isUnitMatrix())
	{
		ASSERT(0);
	}

	return ret;
}

GeomPoint GeomPoint::operator* (Matrix& mat)
{
	GeomPoint pt;
	Matrix ptMatrix(1, 4);
	ptMatrix.m_val[0][0] = m_x;
	ptMatrix.m_val[0][1] = m_y;
	ptMatrix.m_val[0][2] = m_z;
	ptMatrix.m_val[0][3] = 1.0;

	ptMatrix = ptMatrix * mat;

	double weight = 1.0;
	weight = ptMatrix.m_val[0][3];
	pt.m_x = ptMatrix.m_val[0][0] / weight;
	pt.m_y = ptMatrix.m_val[0][1] / weight;
	pt.m_z = ptMatrix.m_val[0][2] / weight;

	return pt;
}