#ifndef __FILELOGGER_H
#define __FILELOGGER_H
#include<stdio.h>
#include<stdarg.h>
#include"Logger.h"

namespace LogConfigTime
{
	class FileLogger : public Logger
	{
		FILE *F;
	public:
		FileLogger(const char *filename);
		virtual ~FileLogger();
		void close();
		virtual void vlog(int _logLevel, const char *tag, const char *format, va_list argp);
	};
}

#endif

