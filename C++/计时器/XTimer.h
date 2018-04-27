#ifndef XTIMER_H
#define XTIMER_H

#include <time.h>

class XTimer
{
private:
	clock_t start;
	int rounds;

public:
	XTimer();
	~XTimer();

	void StartTimer();
	void StartTimer(int rounds);
	//Return in ms
	int CountTimer();
	float CoundTimer_DividedByRounds();
};

#endif

