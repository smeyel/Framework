#ifndef __LOGGER_H
#define __LOGGER_H

namespace LogConfigTime
{
	class Logger
	{
	protected:
		static Logger *instance;
		int loglevel;
	public:
		const static int LOGLEVEL_ERROR = 10;
		const static int LOGLEVEL_WARNING = 5;
		const static int LOGLEVEL_DEBUG = 3;
		const static int LOGLEVEL_VERBOSE = 1;
		const static int LOGLEVEL_INFO = 0;

		Logger();
		void SetLogLevel(int iLogLevel);
		int GetLogLevel(void);
		virtual void Log(int aLogLevel, const char *tag, const char *format, ...)=0;
		static Logger *getInstance(void);
	};
}

#endif
