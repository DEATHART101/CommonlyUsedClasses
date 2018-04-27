#pragma once
#include <math.h>
#include <iostream>

struct XVector2
{
public:
	float x;
	float y;

public:
	XVector2();
	XVector2(float x, float y);
	~XVector2();

public:
	static float DistanceSqr(const XVector2& a, const XVector2& b);

	static float Distance(const XVector2& a, const XVector2& b);

	friend std::ostream& operator<< (std::ostream& output, XVector2 value);
};

