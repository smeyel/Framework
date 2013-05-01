#include "FileLogger.h"

using namespace LogConfigTime;

FileLogger::FileLogger(char *filename)
{
	F = fopen(filename,"at");
}

void FileLogger::close()
{
	fflush(F);
	fclose(F);
}

void FileLogger::Log(int aLogLevel, const char *tag, const char *format, ...)
{
	if (aLogLevel >= loglevel)
	{
		va_list args;
		va_start (args, format);
		vfprintf (F, format, args);
		va_end (args);
	}
}
