/*
 * XRandom.h
 *
 *  Created on: 2018Äê3ÔÂ14ÈÕ
 *      Author: Marina
 */

#ifndef XRANDOM_H_
#define XRANDOM_H_

#include<iostream>
#include<cstdlib>
#include <math.h>
#include<ctime>

class XRandom {
private:
	XRandom();
	virtual ~XRandom();

private:
	static bool m_inited;

public:
	// [left, right)
	static int Range(int left, int right);

	static float Range(float left, float right);
};

#endif /* XRANDOM_H_ */
