#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SendPositionMessage.h"
#include "Logger.h"

SendPositionMessage::SendPositionMessage(char *json)
{
	typecode = SendPosition;
	desiredtimestamp = 0;
	sendImage=0;
	parse(json);
}

SendPositionMessage::SendPositionMessage()
{
	typecode = SendPosition;
	desiredtimestamp = 0;
	sendImage=0;
}

bool SendPositionMessage::parse(char *json)
{
	// find desiredtimestamp
	char *typePtr = strstr(json,"\"desiredtimestamp\":");
	if (!typePtr) return NULL;
	char *beginPtr = strstr(typePtr+18,"\"") + 1;
	if (!beginPtr) return NULL;
	char *endPtr = strstr(beginPtr,"\"");
	if (!endPtr) return NULL;
	char timestampString[100];
	memset(timestampString,0,100);
	strncpy(timestampString,beginPtr,endPtr-beginPtr);

	desiredtimestamp = PlatformSpecifics::getInstance()->atoll(timestampString);

	char buffer[11];
	JsonMessage::readFieldInto(json,"sendimage",buffer);
	sendImage = atoi(buffer);

	return true;
}

void SendPositionMessage::writeJson(char *buffer)
{
	sprintf(buffer,"{ \"type\": \"requestposition\", \"desiredtimestamp\": \"%lld\", \"sendimage\": \"%d\" }#",
		desiredtimestamp,sendImage);
}

void SendPositionMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","SendPositionMessage( desiredtimestamp=%lld sendimage=%d )",
		desiredtimestamp,sendImage);
}
