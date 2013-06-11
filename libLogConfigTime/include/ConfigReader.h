#ifndef __CONFIGREADER_H
#define __CONFIGREADER_H
#include <stdio.h>

namespace LogConfigTime
{
	/** Used to read the configuration from a given source.
		Abstract class, derived classes allow access to the various sources, line an *.ini file.
		If argc and argv are given (originating from main()), command lines may be used to override
			the read settings with parameters --SECTIONNAME/KEYNAME=VALUE.
	*/
	class ConfigReader
	{
	protected:
		static ConfigReader *instance;

		const char *getOverrideValueIfExists(const char *sectionname, const char *keyname, const int argc, const char **argv);
	public:

		ConfigReader()
		{
			instance=this;
		}

		static ConfigReader *getInstance(void)
		{
			return instance;
		}

		virtual const bool getBoolValue(const char *section, const char *key, const int argc=0, const char **argv=NULL)=0;
		virtual const int getIntValue(const char *section, const char *key, const int argc=0, const char **argv=NULL)=0;
		virtual const char *getStringValue(const char *section, const char *key, const int argc=0, const char **argv=NULL)=0;
	};
}

#endif
