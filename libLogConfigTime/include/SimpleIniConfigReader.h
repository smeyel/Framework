#ifndef __SIMPLEINICONFIGREADER_H
#define __SIMPLEINICONFIGREADER_H
#include "SimpleIni.h"
#include"ConfigReader.h"

namespace LogConfigTime
{
	/** Reads the configuration from an ini file.
		If argc, and argv are provided, command line parameter --ini=FILENAME may be used to override the
			name of the ini file.
	*/
	class SimpleIniConfigReader : public ConfigReader
	{
		CSimpleIniA ini;

	public:
		SimpleIniConfigReader();
		SimpleIniConfigReader(const char *filename, const int argc, const char **argv);
		SimpleIniConfigReader(const char *filename);

		bool init(const char *filename, const int argc, const char **argv);
		virtual const bool getBoolValue(const char *section, const char *key, const int argc, const char **argv);
		virtual const int getIntValue(const char *section, const char *key, const int argc, const char **argv);
		virtual const char *getStringValue(const char *section, const char *key, const int argc, const char **argv);

		// The following functions are not included in the ConfigReader interface
		const std::string getStdStringValue(const char *section, const char *key, const int argc, const char **argv);
	};
}
#endif
