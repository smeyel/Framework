#ifndef __STDOUTLOGGER_H
#define __STDOUTLOGGER_H
#include<stdio.h>
#include<stdarg.h>
#include"Logger.h"

namespace LogConfigTime
{
	class StdoutLogger : public Logger
	{
	public:
		virtual void vlog(int _logLevel, const char *tag, const char *format, va_list argp);
	};
}

#endif

