#ifndef FLEXIBLEINTEGER_H_
#define FLEXIBLEINTEGER_H_

#include <iostream>
#include <string>
#include <math.h>

// ��󳤶�
#define MAX_LENGTH 1000 

// ����ȽϺ���
#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

using namespace std;

typedef class FlexibleInteger
{
private:
	int m_NumberLength;				// λ��
	int m_NumberBits[MAX_LENGTH];	// λ����
	bool m_Positive;				// ���ű�־

public:
	// ���캯��
	FlexibleInteger();
	FlexibleInteger(const char*);
	FlexibleInteger(const int&);
	~FlexibleInteger();

public:
	void Clear();						// ���㺯��
	void Print(ostream& output) const;	// ��ʾ����
	string ToString() const;
				
public:
	// ��ֵ����
	FlexibleInteger operator=(const char*);
	FlexibleInteger operator=(int);
	FlexibleInteger operator=(const string);

	// ��������
	FlexibleInteger operator+(const FlexibleInteger &) const;	// �ͺ���
	FlexibleInteger operator-(const FlexibleInteger &) const;	// ���
	FlexibleInteger operator*(const FlexibleInteger &) const;	// ������
	FlexibleInteger operator/(const FlexibleInteger&) const;	// ������

	// �Ƚ�
	bool operator> (const FlexibleInteger&) const;
	bool operator< (const FlexibleInteger&) const;
	bool operator== (const FlexibleInteger&) const;
	bool operator!= (const FlexibleInteger&) const;
	bool operator<= (const FlexibleInteger&) const;
	bool operator>= (const FlexibleInteger&) const;

public:
	friend istream& operator>>(istream &, FlexibleInteger &);		// ���ݽ��պ���
	friend ostream& operator<<(ostream &, const FlexibleInteger &);	// �����������

private:
	void fixlength();
	
} fint;

#endif