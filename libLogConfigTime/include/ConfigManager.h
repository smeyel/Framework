#ifndef __CONFIGMANAGER_H_
#define __CONFIGMANAGER_H_
#include "ConfigReader.h"

namespace LogConfigTime
{
	class ConfigManager
	{
	public:
		bool read(ConfigReader *reader);
	};
}

#endif
