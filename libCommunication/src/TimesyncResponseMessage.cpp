/*
 * TimesyncResponseMessage.cpp
 *
 *  Created on: Dec 4, 2014
 *      Author: akos
 */

#include "TimesyncResponseMessage.h"

TimesyncResponseMessage::TimesyncResponseMessage() :
		JsonMessage(Timesync)
{

}

TimesyncResponseMessage::TimesyncResponseMessage(Json::Value root) :
		JsonMessage(root, Timesync)
{
	unpack();
}

TimesyncResponseMessage::~TimesyncResponseMessage()
{
}

void TimesyncResponseMessage::pack() {
	JsonMessage::pack();
	root[Types::Misc::KEY_VALUES] = new Json::Value(Json::arrayValue);
	Json::Value jValues = root[Types::Misc::KEY_VALUES];

	for (int i = 0; i < values.size(); ++i) {
		jValues.append(values[i].brightness);
		jValues.append(values[i].timestamp);
	}
}

void TimesyncResponseMessage::unpack() {
	values.clear();

	Json::Value jValues = root[Types::Misc::KEY_VALUES];
	for (int i = 0; i < jValues.size(); i += 2) {
		values.push_back(FrameData(jValues[i].asInt(), jValues[i+1].asInt64()));
	}
}
