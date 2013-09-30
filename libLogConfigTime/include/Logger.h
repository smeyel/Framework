#ifndef __LOGGER_H
#define __LOGGER_H

#include <stdarg.h>

namespace LogConfigTime
{
	class Logger
	{
	protected:
		static Logger *instance;
		int loglevel;
	public:
		const static int LOGLEVEL_FATAL = 7;
		const static int LOGLEVEL_ERROR = 6;
		const static int LOGLEVEL_WARNING = 5;
		const static int LOGLEVEL_INFO = 4;
		const static int LOGLEVEL_DEBUG = 3;
		const static int LOGLEVEL_VERBOSE = 2;

		Logger();
		virtual ~Logger ();
		void SetLogLevel(int iLogLevel);
		int GetLogLevel(void);
		
		/** Variant of log that takes not a variable argument list but a single va_list pointer.
		 *  Logging should be implemented here, and log method is a simple wrapper around vlog */
		virtual void vlog(int _logLevel, const char *tag, const char *format, va_list argp) = 0;
		
		void Log(int aLogLevel, const char *tag, const char *format, ...);
		static Logger *getInstance(void);
	};
}

#endif
