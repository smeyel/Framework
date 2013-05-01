#include "SimpleIniConfigReader.h"

using namespace LogConfigTime;

SimpleIniConfigReader::SimpleIniConfigReader(const char *filename)
{
	init(filename);
}

bool SimpleIniConfigReader::init(const char *filename)
{
    // load from a data file
    SI_Error rc = ini.LoadFile(filename);
	return (rc >= 0);
}

const bool SimpleIniConfigReader::getBoolValue(const char *section, const char *key)
{
	return ini.GetBoolValue(section,key,false,NULL);
}

const int SimpleIniConfigReader::getIntValue(const char *section, const char *key)
{
	return (int)ini.GetLongValue(section,key,false,NULL);
}

const std::string SimpleIniConfigReader::getStdStringValue(const char *section, const char *key)
{
	return std::string(ini.GetValue(section,key,"",NULL));
}

const char *SimpleIniConfigReader::getStringValue(const char *section, const char *key)
{
	return ini.GetValue(section,key,"",NULL);
}
