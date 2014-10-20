#include <sstream>
#include <stdio.h>
#include "SendlogMessage.h"
#include "Logger.h"

SendlogMessage::SendlogMessage() :JsonMessage(Sendlog)
{
	typecode = Sendlog;
}

SendlogMessage::SendlogMessage(Json::Value root) : JsonMessage(root, Sendlog)
{
}

void SendlogMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","SendlogMessage()\n");
}
