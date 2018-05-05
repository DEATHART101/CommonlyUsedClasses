#ifndef FLEXIBLEINTEGER_H_
#define FLEXIBLEINTEGER_H_

#include <iostream>
#include <string>
#include <math.h>

// 最大长度
#define MAX_LENGTH 1000 

// 定义比较函数
#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

using namespace std;

typedef class FlexibleInteger
{
private:
	int m_NumberLength;				// 位长
	int m_NumberBits[MAX_LENGTH];	// 位数组
	bool m_Positive;				// 符号标志

public:
	// 构造函数
	FlexibleInteger();
	FlexibleInteger(const char*);
	FlexibleInteger(const int&);
	~FlexibleInteger();

public:
	void Clear();						// 清零函数
	void Print(ostream& output) const;	// 显示函数
	string ToString() const;
				
public:
	// 赋值函数
	FlexibleInteger operator=(const char*);
	FlexibleInteger operator=(int);
	FlexibleInteger operator=(const string);

	// 四则运算
	FlexibleInteger operator+(const FlexibleInteger &) const;	// 和函数
	FlexibleInteger operator-(const FlexibleInteger &) const;	// 差函数
	FlexibleInteger operator*(const FlexibleInteger &) const;	// 积函数
	FlexibleInteger operator/(const FlexibleInteger&) const;	// 除函数

	// 比较
	bool operator> (const FlexibleInteger&) const;
	bool operator< (const FlexibleInteger&) const;
	bool operator== (const FlexibleInteger&) const;
	bool operator!= (const FlexibleInteger&) const;
	bool operator<= (const FlexibleInteger&) const;
	bool operator>= (const FlexibleInteger&) const;

public:
	friend istream& operator>>(istream &, FlexibleInteger &);		// 数据接收函数
	friend ostream& operator<<(ostream &, const FlexibleInteger &);	// 数据输出函数

private:
	void fixlength();
	
} fint;

#endif