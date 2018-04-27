#include "XTimer.h"


XTimer::XTimer() :
	start(-1)
{

}


XTimer::~XTimer()
{
}

void XTimer::StartTimer()
{
	start = clock();
	rounds = -1;
}

void XTimer::StartTimer(int rounds)
{
	start = clock();
	rounds = rounds;
}

int XTimer::CountTimer()
{
	if (start == -1)
	{
		return -1;
	}

	return clock() - start;
}

float XTimer::CoundTimer_DividedByRounds()
{
	if (start == -1 || rounds == -1)
	{
		return -1;
	}

	return (float)(clock() - start) / rounds;
}


