/*
 * XMatrix.cpp
 *
 *  Created on: 2018Äê3ÔÂ7ÈÕ
 *      Author: Marina
 */

#include "XMatrix.h"

XMatrix::XMatrix(mdata ** matrix, int matrixWidth, int matrixHeight) :
	mMatrix(matrix),
	intWidth(matrixWidth),
	intHeight(matrixHeight)
{
}

XMatrix::XMatrix()  :
	mMatrix(NULL),
	intWidth(0),
	intHeight(0)
{

}

XMatrix::XMatrix(int matrixWidth, int matrixHeight)  :
	mMatrix(NULL),
	intWidth(matrixWidth),
	intHeight(matrixHeight)
{
	CreateMatrix(matrixWidth, matrixHeight);
}

XMatrix::XMatrix(const XMatrix& other)  :
	mMatrix(NULL),
	intWidth(0),
	intHeight(0)
{
	if (other.mMatrix != NULL)
	{
		*this = other;
	}
}

XMatrix::~XMatrix()
{
	if (mMatrix != NULL)
	{
		for (int row = 0; row < intHeight; row++)
		{
			delete[] mMatrix[row];
		}
		delete[] mMatrix;
	}
}

const int& XMatrix::GetWidth() const
{
	return intWidth;
}

const int& XMatrix::GetHeight() const
{
	return intHeight;
}

void XMatrix::AllSet(mdata value)
{
	if (mMatrix == NULL)
	{
		return;
	}

	for (int row = 0; row < intHeight; row++)
	{
		for (int col = 0; col < intWidth; col++)
		{
			mMatrix[row][col] = value;
		}
	}
}

const XMatrix& XMatrix::SubMatrix(int x, int y, int width, int height) const
{
	XMatrix m_result(width, height);

	if (mMatrix == NULL)
	{
		return m_result;
	}

	for (int row = y; row < height; row++)
	{
		for (int col = x; col < width; col++)
		{
			m_result.mMatrix[row][col] = mMatrix[row][col];
		}
	}

	return m_result;
}

void XMatrix::SubMatrix(XMatrix& out_result, int x, int y, int width, int height) const
{
	if (mMatrix == NULL)
	{
		return;
	}

	//If not containable
	if (out_result.mMatrix == NULL ||
		out_result.intWidth != width ||
		out_result.intHeight != height)
	{
		if (mMatrix != NULL)
		{
			for (int row = 0; row < intHeight; row++)
			{
				delete[] out_result.mMatrix[row];
			}
			delete[] out_result.mMatrix;
		}
		out_result.CreateMatrix(width, height);
	}

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			out_result.mMatrix[row][col] = mMatrix[row + y][col + x];
		}
	}
}

const XMatrix::XSubMatrix& XMatrix::SSubMatrix(int x, int y, int width, int height)
{
	//If not indexable
	if (mMatrix == NULL ||
		x + width -1 >= intWidth ||
		y + height - 1 >= intHeight)
	{
		return XSubMatrix(NULL, XMSquare(0, 0, 0, 0));
	}

	return XSubMatrix(this, XMSquare(x, y, width, height));
}

void XMatrix::SetSubMatrix(const XMatrix& submatrix, int start_x, int start_y) const
{
	if (submatrix.mMatrix == NULL ||
		start_x + submatrix.intWidth - 1 >= intWidth ||
		start_y + submatrix.intHeight - 1 >= intHeight)
	{
		return;
	}

	for (int row = start_y; row < intHeight; row++)
	{
		for (int col = start_x; col < intWidth; col++)
		{
			mMatrix[row][col] = submatrix.mMatrix[row - start_y][col - start_x];
		}
	}
}

const XMatrix& XMatrix::operator= (const XMatrix& other)
{
	if (other.mMatrix != NULL)
	{
		if (mMatrix == NULL)
		{
			CreateMatrix(other.intWidth, other.intHeight);
		}
		else if (intWidth != other.intWidth && intHeight != other.intHeight)
		{
			for (int row = 0; row < intHeight; row++)
			{
				delete[] mMatrix[row];
			}
			delete[] mMatrix;
			CreateMatrix(other.intWidth, other.intHeight);
		}

		for (int row = 0; row < other.intHeight; row++)
		{
			for (int col = 0; col < other.intWidth; col++)
			{
				mMatrix[row][col] = other.mMatrix[row][col];
			}
		}
	}

	return *this;
}

const XMatrix& XMatrix::Plus(const XMatrix& other)
{
	if (other.mMatrix == NULL ||
		intHeight != other.intHeight ||
		intWidth != other.intWidth)
	{
		return *this;
	}

	if (mMatrix == NULL)
	{
		*this = other;
		return *this;
	}

	for (int row = 0; row < other.intHeight; row++)
	{
		for (int col = 0; col < intWidth; col++)
		{
			mMatrix[row][col] = mMatrix[row][col] + other.mMatrix[row][col];
		}
	}

	return *this;
}

const XMatrix& XMatrix::Minus(const XMatrix& other)
{
	if (other.mMatrix == NULL ||
		intHeight != other.intHeight ||
		intWidth != other.intWidth)
	{
		return *this;
	}

	if (mMatrix == NULL)
	{
		*this = other;
		return *this;
	}

	for (int row = 0; row < other.intHeight; row++)
	{
		for (int col = 0; col < intWidth; col++)
		{
			mMatrix[row][col] = mMatrix[row][col] - other.mMatrix[row][col];
		}
	}

	return *this;
}

const XMatrix& XMatrix::operator+ (const XMatrix& other) const
{
	XMatrix result(intWidth, intHeight);

	if (mMatrix == NULL ||
		other.mMatrix == NULL ||
		intHeight != other.intHeight ||
		intWidth != other.intWidth)
	{
		return result;
	}

	for (int row = 0; row < other.intHeight; row++)
	{
		for (int col = 0; col < intWidth; col++)
		{
			result.mMatrix[row][col] = mMatrix[row][col] + other.mMatrix[row][col];
		}
	}

	return result;
}

const XMatrix& XMatrix::operator- (const XMatrix& other) const
{
	XMatrix result(intWidth, intHeight);

	if (mMatrix == NULL ||
		other.mMatrix == NULL ||
		intHeight != other.intHeight ||
		intWidth != other.intWidth)
	{
		return result;
	}

	for (int row = 0; row < other.intHeight; row++)
	{
		for (int col = 0; col < intWidth; col++)
		{
			result.mMatrix[row][col] = mMatrix[row][col] - other.mMatrix[row][col];
		}
	}

	return result;
}

const XMatrix& XMatrix::operator* (const XMatrix& other) const
{
	XMatrix result(intWidth, intHeight);

	if (mMatrix == NULL ||
		other.mMatrix == NULL ||
		other.intHeight != intWidth)
	{
		return result;
	}

	for (int row = 0; row < other.intHeight; row++)
	{
		for (int col = 0; col < intWidth; col++)
		{
			result.mMatrix[row][col] = mMatrix[row][col] + other.mMatrix[row][col];
		}
	}

	return result;
}

void XMatrix::LeftMultiply(XMatrix& other, MultiplyTypes type)
{
	XMatrix m_result(intWidth, other.intHeight);

	if (intHeight != other.intWidth)
	{
		return;
	}

	if (type == Normal)
	{
		for (int row = 0; row < other.intHeight; row++)
		{
			for (int col = 0; col < intWidth; col++)
			{
				m_result.mMatrix[row][col] = 0;
				for (int i = 0; i < intHeight; i++)
				{
					m_result.mMatrix[row][col] += mMatrix[i][col] * other.mMatrix[row][i];
				}

			}
		}
	}
	else if (type == Strassen)
	{
		strassen(m_result, other);
	}

	*this = m_result;
}

const mdata & XMatrix::GetData(int row, int col)
{
	return mMatrix[row][col];
}

void XMatrix::strassen(XMatrix& out_result, XMatrix& other)
{
	int half_x = intWidth / 2;
	int half_y = intHeight / 2;
	XSubMatrix mA11 = SSubMatrix(0, 0, half_x, half_y);
	XSubMatrix mA12 = SSubMatrix(half_x, 0, intWidth - half_x, half_y);
	XSubMatrix mA21 = SSubMatrix(0, half_y, half_x, intHeight - half_y);
	XSubMatrix mA22 = SSubMatrix(half_x, half_y, intWidth - half_x, intHeight - half_y);

	int ohalf_x = other.intWidth / 2;
	int ohalf_y = other.intHeight / 2;
	XSubMatrix mB11 = other.SSubMatrix(0, 0, ohalf_x, ohalf_y);
	XSubMatrix mB12 = other.SSubMatrix(ohalf_x, 0, other.intWidth - ohalf_x, ohalf_y);
	XSubMatrix mB21 = other.SSubMatrix(0, ohalf_y, ohalf_x, other.intHeight - ohalf_y);
	XSubMatrix mB22 = other.SSubMatrix(ohalf_x, half_y, other.intWidth - ohalf_x, other.intHeight - ohalf_y);

	XSubMatrix mC11 = out_result.SSubMatrix(0, 0, half_x, ohalf_y);
	XSubMatrix mC12 = out_result.SSubMatrix(half_x, 0, intWidth - half_x, ohalf_y);
	XSubMatrix mC21 = out_result.SSubMatrix(0, ohalf_y, ohalf_x, other.intHeight - ohalf_y);
	XSubMatrix mC22 = out_result.SSubMatrix(half_x, half_y, intWidth - half_x, other.intHeight - ohalf_y);

	XMatrix mM1, mM2, mM3, mM4, mM5, mM6, mM7;
	Multiply(mM1, mA11, mB12 - mB22, Normal);
	//cout << mM1 << endl;
	Multiply(mM2, mA11 + mA12, mB22, Normal);
	//cout << mM2 << endl;
	Multiply(mM3, mA21 + mA22, mB11, Normal);
	//cout << mM3 << endl;
	Multiply(mM4, mA22, mB21 - mB11, Normal);
	//cout << mM4 << endl;
	Multiply(mM5, mA11 + mA22, mB11 + mB22, Normal);
	//cout << mM5 << endl;
	Multiply(mM6, mA12 - mA22, mB21 + mB22, Normal);
	//cout << mM6 << endl;
	Multiply(mM7, mA11 - mA21, mB11 + mB12, Normal);
	//cout << mM7 << endl;

	mC11.AllSet(0);
	mC11.Plus(mM5);
	mC11.Plus(mM4);
	mC11.Minus(mM2);
	mC11.Plus(mM6);

	//cout << mC11 << endl;

	mC12.AllSet(0);
	mC12.Plus(mM1);
	mC12.Plus(mM2);

	//cout << mC12 << endl;

	mC21.AllSet(0);
	mC21.Plus(mM3);
	mC21.Plus(mM4);

	//cout << mC21 << endl;

	mC22.AllSet(0);
	mC22.Plus(mM5);
	mC22.Plus(mM1);
	mC22.Minus(mM3);
	mC22.Minus(mM7);

	//cout << mC22 << endl;
}

void XMatrix::l_assign(XMatrix& other)
{
	if (mMatrix != NULL)
	{
		for (int row = 0; row < intHeight; row++)
		{
			delete[] mMatrix[row];
		}
		delete[] mMatrix;
	}

	mMatrix = other.mMatrix;
	intWidth = other.intWidth;
	intHeight = other.intHeight;

	other.mMatrix = NULL;
}

void XMatrix::CreateMatrix(int width, int height)
{
	if (mMatrix == NULL)
	{
		mMatrix = new mdata*[height];
		for (int row = 0; row < height; row++)
		{
			mMatrix[row] = new mdata[width];
		}
		intWidth = width;
		intHeight = height;
	}
}

mdata ** XMatrix::CreateMatrixPointer(int width, int height)
{
	mdata** matrix = new mdata*[height];
	for (int row = 0; row < height; row++)
	{
		matrix[row] = new mdata[width];
	}

	return matrix;
}

void XMatrix::Multiply(XMatrix& out_result, const XMatrix& left, const XMatrix& right, MultiplyTypes type)
{
	if (left.mMatrix == NULL ||
		right.mMatrix == NULL ||
		right.intHeight != left.intWidth)
	{
		return;
	}

	//If not containable
	if (out_result.mMatrix == NULL ||
		out_result.intWidth != right.intWidth ||
		out_result.intHeight != left.intHeight)
	{
		if (out_result.mMatrix != NULL)
		{
			for (int row = 0; row < out_result.intHeight; row++)
			{
				delete[] out_result.mMatrix[row];
			}
			delete[] out_result.mMatrix;
		}
		out_result.CreateMatrix(right.intWidth, left.intHeight);
	}

	if (type == Normal)
	{
		for (int row = 0; row < left.intHeight; row++)
		{
			for (int col = 0; col < right.intWidth; col++)
			{
				out_result.mMatrix[row][col] = 0;
				for (int i = 0; i < right.intHeight; i++)
				{
					out_result.mMatrix[row][col] += right.mMatrix[i][col] * left.mMatrix[row][i];
				}

			}
		}
	}
	else if (type == Strassen)
	{

	}
	else
	{

	}
}

void XMatrix::Multiply(XMatrix& out_result, const XMatrix::XSubMatrix& left, const XMatrix& right, MultiplyTypes type)
{
	if (left.refMatrix == NULL ||
		right.mMatrix == NULL ||
		right.intHeight != left.domain.width)
	{
		return;
	}

	//If not containable
	if (out_result.mMatrix == NULL ||
		out_result.intWidth != right.intWidth ||
		out_result.intHeight != left.domain.height)
	{
		if (out_result.mMatrix != NULL)
		{
			for (int row = 0; row < out_result.intHeight; row++)
			{
				delete[] out_result.mMatrix[row];
			}
			delete[] out_result.mMatrix;
		}
		out_result.CreateMatrix(right.intWidth, left.domain.height);
	}

	if (type == Normal)
	{
		for (int row = 0; row < left.domain.height; row++)
		{
			for (int col = 0; col < right.intWidth; col++)
			{
				out_result.mMatrix[row][col] = 0;
				for (int i = 0; i < right.intHeight; i++)
				{
					out_result.mMatrix[row][col] += right.mMatrix[i][col] * left.refMatrix->mMatrix[row][i];
				}

			}
		}
	}
	else if (type == Strassen)
	{

	}
	else
	{

	}
}


void XMatrix::Multiply(XMatrix& out_result, const XMatrix& left, const XMatrix::XSubMatrix& right, MultiplyTypes type)
{
	if (left.mMatrix == NULL ||
		right.refMatrix == NULL ||
		right.domain.height != left.intWidth)
	{
		return;
	}

	//If not containable
	if (out_result.mMatrix == NULL ||
		out_result.intWidth != right.domain.width ||
		out_result.intHeight != left.intHeight)
	{
		if (out_result.mMatrix != NULL)
		{
			for (int row = 0; row < out_result.intHeight; row++)
			{
				delete[] out_result.mMatrix[row];
			}
			delete[] out_result.mMatrix;
		}
		out_result.CreateMatrix(right.domain.width, left.intHeight);
	}

	if (type == Normal)
	{
		for (int row = 0; row < left.intHeight; row++)
		{
			for (int col = 0; col < right.domain.width; col++)
			{
				out_result.mMatrix[row][col] = 0;
				for (int i = 0; i < right.domain.height; i++)
				{
					out_result.mMatrix[row][col] += right.refMatrix->mMatrix[i][col] * left.mMatrix[row][i];
				}

			}
		}
	}
	else if (type == Strassen)
	{

	}
	else
	{

	}
}

void XMatrix::Multiply(XMatrix& out_result, const XMatrix::XSubMatrix& left, const XMatrix::XSubMatrix& right, MultiplyTypes type)
{
	if (left.refMatrix == NULL ||
		right.refMatrix == NULL ||
		right.domain.height != left.domain.width)
	{
		return;
	}

	//If not containable
	if (out_result.mMatrix == NULL ||
		out_result.intWidth != right.domain.width ||
		out_result.intHeight != left.domain.height)
	{
		if (out_result.mMatrix != NULL)
		{
			for (int row = 0; row < out_result.intHeight; row++)
			{
				delete[] out_result.mMatrix[row];
			}
			delete[] out_result.mMatrix;
		}
		out_result.CreateMatrix(right.domain.width, left.domain.height);
	}

	if (type == Normal)
	{
		for (int row = 0; row < left.domain.height; row++)
		{
			for (int col = 0; col < right.domain.width; col++)
			{
				out_result.mMatrix[row][col] = 0;
				for (int i = 0; i < right.domain.height; i++)
				{
					out_result.mMatrix[row][col] += right.refMatrix->mMatrix[i][col] * left.refMatrix->mMatrix[row][i];
				}

			}
		}
	}
	else if (type == Strassen)
	{

	}
	else
	{

	}
}


ostream& operator<< (ostream& output, const XMatrix& matrix)
{
	for (int row = 0; row < matrix.intHeight; row++)
	{
		for (int col = 0; col < matrix.intWidth; col++)
		{
			output << matrix.mMatrix[row][col] << " ";
		}
		output << endl;
	}
	output << endl;

	return output;
}
