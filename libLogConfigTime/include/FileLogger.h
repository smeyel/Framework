#ifndef __FILELOGGER_H
#define __FILELOGGER_H
#include<stdio.h>
#include<STDARG.H>
#include"Logger.h"

namespace LogConfigTime
{
	class FileLogger : public Logger
	{
		FILE *F;
	public:
		FileLogger(const char *filename);
		void close();
		virtual void Log(int aLogLevel, const char *tag, const char *format, ...);
	};
}

#endif

