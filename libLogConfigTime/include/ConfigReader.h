#ifndef __CONFIGREADER_H
#define __CONFIGREADER_H

namespace LogConfigTime
{
	class ConfigReader
	{
	protected:
		static ConfigReader *instance;
	public:

		ConfigReader()
		{
			instance=this;
		}

		static ConfigReader *getInstance(void)
		{
			return instance;
		}

		virtual const bool getBoolValue(const char *section, const char *key)=0;
		virtual const int getIntValue(const char *section, const char *key)=0;
		virtual const char *getStringValue(const char *section, const char *key)=0;
	};
}

#endif
