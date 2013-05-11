#include <sstream>
#include "SendlogMessage.h"
#include "Logger.h"

SendlogMessage::SendlogMessage(char *json)
{
	typecode = Sendlog;
	parse(json);
}

bool SendlogMessage::parse(char *json)
{
	return true;
}

void SendlogMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","SendlogMessage()\n");
}
