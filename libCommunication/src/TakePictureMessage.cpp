#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "TakePictureMessage.h"
#include "Logger.h"

TakePictureMessage::TakePictureMessage() : JsonMessage(TakePicture), desiredTimestamp(0)
{
	root[Types::Action::KEY] = Types::Action::COMMAND;
	root[Types::Subject::KEY] = Types::Subject::TAKE_PICTURE;
}

TakePictureMessage::TakePictureMessage(Json::Value root) : JsonMessage(root, TakePicture), desiredTimestamp(0)
{
	unpack();
}

void TakePictureMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(
			LogConfigTime::Logger::LOGLEVEL_INFO,
			"Message",
			"TakePictureMessage( desiredtimestamp=%lld )",getDesiredTimestamp()
	);
}

void TakePictureMessage::pack() {
	JsonMessage::pack();
	Json::Value& values = root[Types::Misc::KEY_VALUES];
	values[DESIRED_TIMESTAMP_IDX] = desiredTimestamp;
}

void TakePictureMessage::unpack() {
	Json::Value& values = root[Types::Misc::KEY_VALUES];
	desiredTimestamp = values[DESIRED_TIMESTAMP_IDX].asInt64();
}
