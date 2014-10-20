#include <sstream>
#include <stdio.h>
#include "PingMessage.h"
#include "Logger.h"

PingMessage::PingMessage(char *json)
{
	typecode = Ping;
	parse(json);
}

PingMessage::PingMessage() : JsonMessage(Ping)
{
	root[Types::Subject::KEY] = Types::Subject::PING;
	root[Types::Action::KEY] = Types::Action::QUERY;
}

bool PingMessage::parse(char *json)
{
	return true;
}

PingMessage::PingMessage(Json::Value root) : JsonMessage(root, Ping) {
}

void PingMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","PingMessage()\n");
}
