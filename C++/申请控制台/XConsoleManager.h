#ifndef XCONSOLEMANAGER_H_
#define XCONSOLEMANAGER_H_

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

class XConsoleManager
{
private:
	XConsoleManager();
	~XConsoleManager();

public:
	static bool InitConsole();
	static void UnInitConsole();
};

#endif

