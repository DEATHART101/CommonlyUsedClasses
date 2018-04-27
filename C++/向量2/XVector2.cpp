#include "XVector2.h"

XVector2::XVector2() :
	x(0.0f),
	y(0.0f)
{
}

XVector2::XVector2(float x, float y) :
	x(x),
	y(y)
{
}

XVector2::~XVector2()
{
}

float XVector2::DistanceSqr(const XVector2 & a, const XVector2 & b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

float XVector2::Distance(const XVector2 & a, const XVector2 & b)
{
	return sqrt((double)((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}

std::ostream & operator<<(std::ostream & output, XVector2 value)
{
	output << "(" << value.x << "," << value.y << ")";
	return output;
}
