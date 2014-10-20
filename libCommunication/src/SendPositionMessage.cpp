#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SendPositionMessage.h"
#include "Logger.h"


#include <iostream>
using namespace std;


SendPositionMessage::SendPositionMessage()
		: JsonMessage(SendPosition), desiredTimestamp(0), sendImage(0)
{
	std::cout << sWriter.write(root) << std::endl;
}

SendPositionMessage::SendPositionMessage(Json::Value root)
		: JsonMessage(root, SendPosition)
{
	unpack();
}


void SendPositionMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(
			LogConfigTime::Logger::LOGLEVEL_INFO,
			"Message",
			"SendPositionMessage( desiredtimestamp=%lld sendimage=%d )",
			getDesiredTimestamp(), getSendImage()
	);
}

void SendPositionMessage::pack() {
	cout<< "SendPositionMessage::pack()" << endl;
	JsonMessage::pack();
	Json::Value& values = root[Types::Misc::KEY_VALUES];
	values[DESIRED_TIMESTAMP_IDX] = desiredTimestamp;
	values[SENDIMAGE_IDX] = sendImage;
}

void SendPositionMessage::unpack() {
	cout<< "SendPositionMessage::unpack()" << endl;
	Json::Value& values = root[Types::Misc::KEY_VALUES];
	desiredTimestamp = values[DESIRED_TIMESTAMP_IDX].asInt64();
	sendImage = values[SENDIMAGE_IDX].asInt();
}
