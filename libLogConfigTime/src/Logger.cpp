#include<stdio.h>
#include<stdarg.h>
#include"Logger.h"

using namespace LogConfigTime;

Logger *Logger::instance=NULL;

Logger::Logger()
{
	instance=this;
	loglevel = LOGLEVEL_WARNING;
}

Logger::~Logger() {}

void Logger::SetLogLevel(int iLogLevel)
{
	loglevel = iLogLevel;
}

int Logger::GetLogLevel(void)
{
	return loglevel;
}

void Logger::Log(int aLogLevel, const char *tag, const char *format, ...)
{
	va_list argp;
	va_start (argp, format);

	vlog(aLogLevel, tag, format, argp);
	
	va_end (argp);
}

Logger *Logger::getInstance(void)
{
	return instance;
}
