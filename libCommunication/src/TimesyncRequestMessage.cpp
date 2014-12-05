/*
 * TimesyncRequestMessage.cpp
 *
 *  Created on: Dec 4, 2014
 *      Author: akos
 */

#include "TimesyncRequestMessage.h"

TimesyncRequestMessage::TimesyncRequestMessage() :
		JsonMessage(Timesync)
{
	root[Types::Subject::KEY] = Types::Subject::TIMESYNC;
	root[Types::Action::KEY] = Types::Action::COMMAND;
}

TimesyncRequestMessage::TimesyncRequestMessage(Json::Value root) :
		JsonMessage(root, Timesync)
{
}

TimesyncRequestMessage::~TimesyncRequestMessage()
{
}

