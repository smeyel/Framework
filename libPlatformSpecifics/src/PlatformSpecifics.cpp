#include "PlatformSpecifics.h"
#ifdef _WIN32
#include "PlatformSpecificsWin32.h"
#elif __gnu_linux__
#include "PlatformSpecificsLinux.h"
#endif

PlatformSpecifics *PlatformSpecifics::instance;

PlatformSpecifics *PlatformSpecifics::getInstance()
{
	if (!instance)
	{
#ifdef _WIN32
		instance = (PlatformSpecifics*)new PlatformSpecificsWin32();
#elif __gnu_linux__
		instance = (PlatformSpecifics*)new PlatformSpecificsLinux();
#endif
	}
	return instance;
}

