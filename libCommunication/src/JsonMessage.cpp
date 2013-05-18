#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <winsock2.h>

#include "JsonMessage.h"
#include "JpegMessage.h"
#include "PingMessage.h"
#include "SendlogMessage.h"
#include "TakePictureMessage.h"
#include "MeasurementLogMessage.h"

#include "Logger.h"

// Used for debugging JSON messages
#include <sstream>
#include "picojson.h"

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

#ifdef DEBUG_JSON_IF_UNKNOWN
	DebugJson(json);
#endif

	return NULL;
}

void JsonMessage::writeAuxFile(char *filename)
{
	std::ofstream targetStream;
	targetStream.open(filename,std::ofstream::binary);
	this->writeAuxStream(&targetStream);
	targetStream.flush();
	targetStream.close();
}

void JsonMessage::DebugJson(char *json)
{
	std::istringstream jsonStream(json);

	picojson::value v;
  
	// read json value from stream
	jsonStream >> v;
	if (jsonStream.fail()) {
		std::cerr << picojson::get_last_error() << std::endl;
		return;
	}
  
	// dump json object
	std::cout << "---- dump input ----" << std::endl;
	std::cout << v << std::endl;

	// accessors
	std::cout << "---- analyzing input ----" << std::endl;
	if (v.is<picojson::null>()) {
		std::cout << "input is null" << std::endl;
	} else if (v.is<bool>()) {
		std::cout << "input is " << (v.get<bool>() ? "true" : "false") << std::endl;
	} else if (v.is<double>()) {
		std::cout << "input is " << v.get<double>() << std::endl;
	} else if (v.is<std::string>()) {
		std::cout << "input is " << v.get<std::string>() << std::endl;
	} else if (v.is<picojson::array>()) {
		std::cout << "input is an array" << std::endl;
		const picojson::array& a = v.get<picojson::array>();
		for (picojson::array::const_iterator i = a.begin(); i != a.end(); ++i)
		{
			std::cout << "  " << *i << std::endl;
		}
	}
	else if (v.is<picojson::object>()) {
		std::cout << "input is an object" << std::endl;
		const picojson::object& o = v.get<picojson::object>();
		for (picojson::object::const_iterator i = o.begin(); i != o.end(); ++i)
		{
			std::cout << i->first << "  " << i->second << std::endl;
		}
	}
	return;
}

void JsonMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","JsonMessage()\n");
}

// targetStream may be NULL, given number of bytes will be read anyway.
void JsonMessage::receiveIntoStream(std::ostream *targetStream, SOCKET sock, long bytenum)
{
	char receiveBuffer[RCVBUFSIZE];
	long receivedTotalBytes = 0;
	int received;
	while (receivedTotalBytes < bytenum)
	{
		received = recv(sock, receiveBuffer, RCVBUFSIZE, 0);
		receivedTotalBytes += received;
		if (targetStream!=NULL)
		{
			(*targetStream).write(receiveBuffer, received); 
		}
	}
}

void JsonMessage::readFieldInto(char *json, char *fieldname, char *resultBuffer)
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
