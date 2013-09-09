#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "PlatformSpecifics.h"	// Handles socket-related includes as well

#include "JsonMessage.h"
#include "JpegMessage.h"
#include "PingMessage.h"
#include "SendlogMessage.h"
#include "TakePictureMessage.h"
#include "MeasurementLogMessage.h"
#include "SendPositionMessage.h"
#include "MatImageMessage.h"
#include "TextMessage.h"
#include "PropertyMessage.h"

#include "Logger.h"

// Used for debugging JSON messages
#include <sstream>

#define RCVBUFSIZE 4096
#define MAXJSONFIELDNAMELENGTH 128

JsonMessage::JsonMessage()
{
	typecode = Default;
}

JsonMessageTypeEnum JsonMessage::getMessageType()
{
	return typecode;
}

JsonMessage *JsonMessage::parse(char *json)
{
	char typeString[MAXTYPENAMELENGTH];
	JsonMessage::readFieldInto(json,"type",typeString);
		
	// Let the corresponding class parse it
	if (!strcmp(typeString,"ping"))
	{
		return new PingMessage(json);
	}
	else if (!strcmp(typeString,"takepicture"))
	{
		return new TakePictureMessage(json);
	}
	else if (!strcmp(typeString,"sendlog"))
	{
		return new SendlogMessage(json);
	}
	else if (!strcmp(typeString,"JPEG"))
	{
		return new JpegMessage(json);
	}
	else if (!strcmp(typeString,"measurementlog"))
	{
		return new MeasurementLogMessage(json);
	}
	else if (!strcmp(typeString,"requestposition"))
	{
		return new SendPositionMessage(json);
	}
	else if (!strcmp(typeString,"matimage"))
	{
		return new MatImageMessage(json);
	}
	else if (!strcmp(typeString,"text"))
	{
		return new TextMessage(json);
	}
	else if (!strcmp(typeString,"property"))
	{
		return new PropertyMessage(json);
	}

#ifdef DEBUG_JSON_IF_UNKNOWN
	DebugJson(json);
#endif

	return NULL;
}

void JsonMessage::DebugJson(char *json)
{
	std::cout << "JSON BEGIN" << std::endl << json << "JSON END" << std::endl;
}

void JsonMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","JsonMessage()\n");
}

// targetStream may be NULL, given number of bytes will be read anyway.
void JsonMessage::receiveIntoStream(std::ostream *targetStream, int sock, long bytenum)
{
	char receiveBuffer[RCVBUFSIZE];
	long receivedTotalBytes = 0;
	int received;
	while (receivedTotalBytes < bytenum)
	{
		received = PlatformSpecifics::getInstance()->recv(sock, receiveBuffer, RCVBUFSIZE, 0);
		receivedTotalBytes += received;
		if (targetStream!=NULL)
		{
			(*targetStream).write(receiveBuffer, received); 
		}
	}
}

void JsonMessage::readFieldInto(char *json, const char *fieldname, char *resultBuffer)
{
	char buff[MAXJSONFIELDNAMELENGTH];

	*resultBuffer=0;	// Default result
	sprintf(buff,"\"%s\":",fieldname);
	char *basepos = strstr(json,buff);
	if (!basepos) return;
	int len = strlen(buff);
	char *beginPtr = strstr(basepos+len,"\"") + 1;
	if (!beginPtr) return;
	char *endPtr = strstr(beginPtr,"\"");
	if (!endPtr) return;

	memcpy(resultBuffer,beginPtr,endPtr-beginPtr);
	*(resultBuffer+(endPtr-beginPtr)) = 0;
	return;
}
