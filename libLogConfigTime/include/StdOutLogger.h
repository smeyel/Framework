#ifndef __STDOUTLOGGER_H
#define __STDOUTLOGGER_H
#include<stdio.h>
#include<STDARG.H>
#include"Logger.h"

namespace LogConfigTime
{
	class StdoutLogger : public Logger
	{
	public:
		virtual void Log(int aLogLevel, const char *tag, const char *format, ...);
	};
}

#endif
