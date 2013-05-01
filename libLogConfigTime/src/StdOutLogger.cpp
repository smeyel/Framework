#include "StdOutLogger.h"

using namespace LogConfigTime;

void StdoutLogger::Log(int aLogLevel, const char *tag, const char *format, ...)
{
	if (aLogLevel >= loglevel)
	{
		va_list args;
		va_start (args, format);
		vfprintf (stdout, format, args);
		va_end (args);
	}
}
