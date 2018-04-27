#include "XConsoleManager.h"

XConsoleManager::XConsoleManager()
{
}


XConsoleManager::~XConsoleManager()
{
}

bool XConsoleManager::InitConsole()
{
	AllocConsole();
	if (freopen("CONOUT$", "w+t", stdout) == NULL)
	{
		return false;
	}
	if (freopen("CONIN$", "r+t", stdin) == NULL)
	{
		return false;
	}
	return true;
}

void XConsoleManager::UnInitConsole()
{
	FreeConsole();
}
