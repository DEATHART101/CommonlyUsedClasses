/*
 * XMatrix.h
 *
 *  Created on: 2018Äê3ÔÂ7ÈÕ
 *      Author: Marina
 */

#ifndef XMATRIX_H_
#define XMATRIX_H_

#define NULL 0

typedef float mdata;

#include <iostream>
using namespace std;

class XMatrix
{
public:
	enum MultiplyTypes { Normal, Strassen };

private:
	struct XMSquare
	{
	public:
		int x;
		int y;
		int width;
		int height;

	public:
		XMSquare(int x, int y, int width, int height) :
			x(x),
			y(y),
			width(width),
			height(height)
		{

		}
	};

	class XSubMatrix
	{
	public:
		XMatrix * refMatrix;
		XMSquare domain;

	public:
		XSubMatrix(XMatrix* target, XMSquare domain) :
			refMatrix(target),
			domain(domain)
		{

		}

		XSubMatrix(XMatrix* target, int x, int y, int width, int height) :
			refMatrix(target),
			domain(XMSquare(x, y , width, height))
		{

		}

		XSubMatrix(const XSubMatrix& other) :
			refMatrix(other.refMatrix),
			domain(other.domain)
		{

		}

		void AllSet(mdata value)
		{
			for (int row = 0; row < domain.height; row++)
			{
				for (int col = 0; col < domain.width; col++)
				{
					refMatrix->mMatrix[row + domain.y][col + domain.x] = value;
				}
			}
		}

		void Plus(const XSubMatrix& other)
		{
			if (refMatrix == NULL ||
				other.refMatrix == NULL ||
				domain.width != other.domain.width ||
				domain.height != other.domain.height)
			{
				return;
			}

			for (int row = 0; row < domain.height; row++)
			{
				for (int col = 0; col < domain.width; col++)
				{
					refMatrix->mMatrix[row + domain.y][col + domain.x] += other.refMatrix->mMatrix[row + other.domain.y][col + other.domain.x];
				}
			}
		}

		void Plus(const XMatrix& other)
		{
			if (refMatrix == NULL ||
				other.mMatrix == NULL ||
				domain.width != other.intWidth ||
				domain.height != other.intHeight)
			{
				return;
			}

			for (int row = 0; row < domain.height; row++)
			{
				for (int col = 0; col < domain.width; col++)
				{
					refMatrix->mMatrix[row + domain.y][col + domain.x] += other.mMatrix[row][col];
				}
			}
		}

		void Minus(const XSubMatrix& other)
		{
			if (refMatrix == NULL ||
				other.refMatrix == NULL ||
				domain.width != other.domain.width ||
				domain.height != other.domain.height)
			{
				return;
			}

			for (int row = 0; row < domain.height; row++)
			{
				for (int col = 0; col < domain.width; col++)
				{
					refMatrix->mMatrix[row + domain.y][col + domain.x] -= other.refMatrix->mMatrix[row + other.domain.y][col + other.domain.x];
				}
			}
		}

		void Minus(const XMatrix& other)
		{
			if (refMatrix == NULL ||
				other.mMatrix == NULL ||
				domain.width != other.intWidth ||
				domain.height != other.intHeight)
			{
				return;
			}

			for (int row = 0; row < domain.height; row++)
			{
				for (int col = 0; col < domain.width; col++)
				{
					refMatrix->mMatrix[row + domain.y][col + domain.x] -= other.mMatrix[row][col];
				}
			}
		}

		XMatrix operator+ (const XSubMatrix& other) const
		{
			if (refMatrix == NULL ||
				other.refMatrix == NULL ||
				domain.height != other.domain.height ||
				domain.width != other.domain.width)
			{
				return XMatrix(NULL, 0, 0);
			}

			mdata** tempMatrix = XMatrix::CreateMatrixPointer(domain.width, domain.height);

			for (int row = 0; row < other.domain.height; row++)
			{
				for (int col = 0; col < domain.width; col++)
				{
					tempMatrix[row][col] = refMatrix->mMatrix[row][col] + other.refMatrix->mMatrix[row][col];
				}
			}

			return XMatrix(tempMatrix, domain.width, domain.height);
		}

		XMatrix operator- (const XSubMatrix& other) const
		{
			if (refMatrix == NULL ||
				other.refMatrix == NULL ||
				domain.height != other.domain.height ||
				domain.width != other.domain.width)
			{
				return XMatrix(NULL, domain.width, domain.height);
			}

			mdata** tempMatrix = XMatrix::CreateMatrixPointer(domain.width, domain.height);

			for (int row = 0; row < other.domain.height; row++)
			{
				for (int col = 0; col < domain.width; col++)
				{
					tempMatrix[row][col] = refMatrix->mMatrix[row][col] - other.refMatrix->mMatrix[row][col];
				}
			}

			return XMatrix(tempMatrix, domain.width, domain.height);
		}

		const XSubMatrix& operator= (const XSubMatrix& other)
		{
			refMatrix = other.refMatrix;
			domain = other.domain;

			return *this;
		}

		friend ostream& operator<< (ostream& output, const XSubMatrix& matrix)
		{
			if (matrix.refMatrix == NULL)
			{
				return output;
			}

			for (int row = 0; row < matrix.domain.height; row++)
			{
				for (int col = 0; col < matrix.domain.width; col++)
				{
					cout << matrix.refMatrix->GetData(row + matrix.domain.y, col + matrix.domain.x) << ' ';
				}
				cout << endl;
			}
			cout << endl;
		}
	};

private:
	mdata** mMatrix;
	int intWidth;
	int intHeight;

private:
	XMatrix(mdata** matrix, int matrixWidth, int matrixHeight);

public:
	XMatrix();
	XMatrix(int matrixWidth, int matrixHeight);
	XMatrix(const XMatrix& other);

	~XMatrix();

	const int& GetWidth() const;
	const int& GetHeight() const;


	void AllSet(mdata value);
	const XMatrix& SubMatrix(int x, int y, int width, int height) const;
	void SubMatrix(XMatrix& out_result, int x, int y, int width, int height) const;
	const XSubMatrix& SSubMatrix(int x, int y, int width, int height);
	void SetSubMatrix (const XMatrix& submatrix, int start_x, int start_y) const;
	const XMatrix& Plus(const XMatrix& other);
	const XMatrix& Minus(const XMatrix& other);
	void LeftMultiply(XMatrix& other, MultiplyTypes type = Normal);
	const mdata& GetData(int x, int y);

	const XMatrix& operator= (const XMatrix& other);
	const XMatrix& operator+ (const XMatrix& other) const;
	const XMatrix& operator- (const XMatrix& other) const;
	const XMatrix& operator* (const XMatrix& other) const;

private:
	void strassen(XMatrix& out_result, XMatrix& other);
	void l_assign(XMatrix& other);
	void CreateMatrix(int width, int height);
	
private:
	static mdata** CreateMatrixPointer(int width, int height);

	static void Multiply(XMatrix& out_result, const XMatrix& left, const XMatrix& right, MultiplyTypes type = Normal);
	static void Multiply(XMatrix& out_result, const XSubMatrix& left, const XSubMatrix& right, MultiplyTypes type = Normal);
	static void Multiply(XMatrix& out_result, const XSubMatrix& left, const XMatrix& right, MultiplyTypes type = Normal);
	static void Multiply(XMatrix& out_result, const XMatrix& left, const XSubMatrix& right, MultiplyTypes type = Normal);


public:
	friend ostream& operator<< (ostream& output, const XMatrix& matrix);

};



#endif /* XMATRIX_H_ */
