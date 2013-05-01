#ifndef __SIMPLEINICONFIGREADER_H
#define __SIMPLEINICONFIGREADER_H
#include "SimpleIni.h"
#include"ConfigReader.h"

namespace LogConfigTime
{
	class SimpleIniConfigReader : public ConfigReader
	{
		CSimpleIniA ini;

	public:
		SimpleIniConfigReader();
		SimpleIniConfigReader(const char *filename);

		bool init(const char *filename);
		virtual const bool getBoolValue(const char *section, const char *key);
		virtual const int getIntValue(const char *section, const char *key);
		virtual const char *getStringValue(const char *section, const char *key);

		// The following functions are not included in the ConfigReader interface
		const std::string getStdStringValue(const char *section, const char *key);
	};
}
#endif
