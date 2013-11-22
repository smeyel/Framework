#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "PositionMessage.h"
#include "Logger.h"

PositionMessage::PositionMessage(char *json)
{
	typecode = Position;
	timestamp = 0;
	parse(json);
}

PositionMessage::PositionMessage()
{
	typecode = Position;
	timestamp = 0;
}

bool PositionMessage::parse(char *json)
{
	char timestampString[128];
	JsonMessage::readFieldInto(json,"timestamp",timestampString);
	timestamp = PlatformSpecifics::getInstance()->atoll(timestampString);

	char markerDatas[1024];
	JsonMessage::readFieldInto(json,"trackerdatas",markerDatas);

	std::cout << markerDatas;

	return true;
}

void PositionMessage::writeJson(char *buffer)
{
	sprintf(buffer,"{ \"type\": \"position\", \"size\": \"0\", \"timestamp\": \"%lld\", \"trackerdatas\": \"\" }#",
		timestamp);
}

void PositionMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","PositionMessage( timestamp=%lld )",
		timestamp);
}
