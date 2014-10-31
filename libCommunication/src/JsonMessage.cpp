#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "PlatformSpecifics.h"	// Handles socket-related includes as well

#include "JsonMessage.h"
#include "JpegMessage.h"
#include "PingMessage.h"
#include "PongMessage.h"
#include "SendlogMessage.h"
#include "TakePictureMessage.h"
#include "MeasurementLogMessage.h"
#include "SendPositionMessage.h"
#include "PositionMessage.h"
#include "MatImageMessage.h"
#include "TextMessage.h"
#include "PropertyMessage.h"

#include "Logger.h"

// Used for debugging JSON messages
#include <sstream>
#include <iostream>

#define RCVBUFSIZE 4096
#define MAXJSONFIELDNAMELENGTH 128

JsonMessage::JsonMessage(): typecode(Default)
{
	timestamp = 0;
}

JsonMessage::JsonMessage(Json::Value root) : typecode(Default), root(root)
{
	unpack();
}

JsonMessage::JsonMessage(JsonMessageTypeEnum typecode) : typecode(typecode)
{
	timestamp = 0;
}

JsonMessage::JsonMessage(Json::Value root, JsonMessageTypeEnum typecode)
		: typecode(typecode), root(root)
{
	unpack();
}

void JsonMessage::writeJson(char *buffer) {
	pack();
	const char* msg = fWriter.write(root).c_str();
	strncpy(buffer, msg, strlen(msg));
	buffer[strlen(msg)] = '#';
}

JsonMessageTypeEnum JsonMessage::getMessageType()
{
	return typecode;
}

JsonMessage *JsonMessage::parse(const char *json)
{
	Json::Value container;
	Json::Reader reader;
	reader.parse(json, container);

	using namespace std;
	cout << "Parse " << container << endl;

	if (container.isArray()) {
		if (container.size() > 1) {
			cout << "Warning! Received message contains more than one items!!! Only dealing with the first one for now." << endl;
		}
		return parse(container[0]);
	} else {
		return parse(container);
	}

	return NULL;
}

JsonMessage *JsonMessage::parse(Json::Value& root)
{
	const char* subject = root[Types::Subject::KEY].asCString();

	// Let the corresponding class parse it
	if (!strcmp(subject, Types::Subject::PING))
	{
		return new PingMessage(root);
	}
	else if (!strcmp(subject, Types::Subject::PONG))
	{
		return new PongMessage(root);
	}
	else if (!strcmp(subject, Types::Subject::TAKE_PICTURE))
	{
		return new TakePictureMessage(root);
	}
	else if (!strcmp(subject, Types::Subject::LOG)) // TODO differentiate on action (query|info)
	{
		const char* action = root[Types::Action::KEY].asCString();
		if (!strcmp(action, Types::Action::QUERY))
		{
			return new SendlogMessage(root);
		}
		else if (!strcmp(action, Types::Action::INFO))
		{
	//			return new SendlogResponseMessage(root);
			return NULL; // TODO implement
		}
	}
	else if (!strcmp(subject, Types::Subject::CAMERA_IMAGE))
	{
		return new JpegMessage(root);
	}
	else if (!strcmp(subject, Types::Subject::LOG))
	{
		return new MeasurementLogMessage(root);
	}
	else if (!strcmp(subject, Types::Subject::POSITION))
	{
		return new SendPositionMessage(root);
	}
	else if (!strcmp(subject, Types::Subject::MAT_IMAGE))
	{
		return new MatImageMessage(root);
	}
	else if (!strcmp(subject, Types::Subject::TEXT))
	{
		return new TextMessage(root);
	}
	else if (!strcmp(subject, Types::Subject::PROPERTY))
	{
		return new PropertyMessage(root);
	}

	std::cout << "Unknown subject: " << subject << "! Creating a plain JsonMessage." << std::endl;
	return new JsonMessage(root);
}

void JsonMessage::DebugJson(const char *json)
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

std::ostream& operator<<(std::ostream& o, JsonMessage* msg) {
	msg->pack();
	o << msg->root;
	return o;
}

void JsonMessage::pack() {
	root[Types::Misc::KEY_TIMESTAMP] = timestamp;
}

void JsonMessage::unpack() {
	timestamp = root[Types::Misc::KEY_TIMESTAMP].asInt64();
}

std::string JsonMessage::toString(bool compact) {
	pack();
	if (compact) {
		return fWriter.write(root) + '#';
	} else {
		return sWriter.write(root);
	}
}
