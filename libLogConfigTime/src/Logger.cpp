#include<stdio.h>
#include<STDARG.H>
#include"Logger.h"

using namespace LogConfigTime;

Logger *Logger::instance=NULL;

Logger::Logger()
{
	instance=this;
	loglevel = LOGLEVEL_WARNING;
}

void Logger::SetLogLevel(int iLogLevel)
{
	loglevel = iLogLevel;
}

int Logger::GetLogLevel(void)
{
	return loglevel;
}

Logger *Logger::getInstance(void)
{
	return instance;
}
