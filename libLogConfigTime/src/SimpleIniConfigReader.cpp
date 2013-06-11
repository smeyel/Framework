#include <iostream>
#include "SimpleIniConfigReader.h"

using namespace LogConfigTime;

SimpleIniConfigReader::SimpleIniConfigReader(const char *filename, const int argc, const char **argv)
{
	init(filename, argc, argv);
}

SimpleIniConfigReader::SimpleIniConfigReader(const char *filename)
{
	init(filename, 0, NULL);
}

bool SimpleIniConfigReader::init(const char *filename, const int argc, const char **argv)
{
	// Check for filename override
	const char *newfilename = getOverrideValueIfExists(NULL,"ini",argc,argv);
	if (newfilename)
	{
		std::cout << "INI overridden to " << filename << std::endl;
		filename=newfilename;
	}
    // load from a data file
    SI_Error rc = ini.LoadFile(filename);
	return (rc >= 0);
}

const bool SimpleIniConfigReader::getBoolValue(const char *section, const char *key, const int argc, const char **argv)
{
	const char *value = getOverrideValueIfExists(section,key,argc,argv);
	if (value)
	{
		return (atoi(value)!=0);
	}
	else
	{
		return ini.GetBoolValue(section,key,false,NULL);
	}
}

const int SimpleIniConfigReader::getIntValue(const char *section, const char *key, const int argc, const char **argv)
{
	const char *value = getOverrideValueIfExists(section,key,argc,argv);
	if (value)
	{
		return atoi(value);
	}
	else
	{
		return (int)ini.GetLongValue(section,key,false,NULL);
	}
}

const std::string SimpleIniConfigReader::getStdStringValue(const char *section, const char *key, const int argc, const char **argv)
{
	const char *value = getOverrideValueIfExists(section,key,argc,argv);
	if (value)
	{
		return std::string(value);
	}
	else
	{
		return std::string(ini.GetValue(section,key,"",NULL));
	}
}

const char *SimpleIniConfigReader::getStringValue(const char *section, const char *key, const int argc, const char **argv)
{
	const char *value = getOverrideValueIfExists(section,key,argc,argv);
	if (value)
	{
		return value;
	}
	else
	{
		return ini.GetValue(section,key,"",NULL);
	}
}
