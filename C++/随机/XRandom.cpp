/*
 * XRandom.cpp
 *
 *  Created on: 2018Äê3ÔÂ14ÈÕ
 *      Author: Marina
 */

#include "XRandom.h"

bool XRandom::m_inited = false;

XRandom::XRandom() {
	// TODO Auto-generated constructor stub

}

XRandom::~XRandom() {
	// TODO Auto-generated destructor stub
}

int XRandom::Range(int left, int right)
{
	if (!m_inited)
	{
		srand((unsigned)time(NULL));
		m_inited = true;
	}

	int random_times = (right - left) / (RAND_MAX + 1) +
			((right - left) % (RAND_MAX + 1) == 0 ? 0 : 1);

	int result = 1;
	int result_max = pow(RAND_MAX, random_times);
	for (int i = 0; i < random_times; i++)
	{
		result *= rand();
	}
	double porp = (double)result / (double)result_max;
	return left + (int)((right - left) * porp);
}

float XRandom::Range(float left, float right)
{
	if (!m_inited)
	{
		srand((unsigned)time(NULL));
		m_inited = true;
	}

	return 0.0f;
}
