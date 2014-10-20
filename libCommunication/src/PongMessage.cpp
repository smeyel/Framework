/*
 * PongMessage.cpp
 *
 *  Created on: Oct 19, 2014
 *      Author: akos
 */

#include "PongMessage.h"

#include "Logger.h"

PongMessage::PongMessage() : JsonMessage(Pong)
{
	root[Types::Subject::KEY] = Types::Subject::PONG;
	root[Types::Action::KEY] = Types::Action::INFO;
}

PongMessage::PongMessage(Json::Value root) : JsonMessage(root, Pong)
{
}

void PongMessage::log() {
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO, "Message", "PongMessage()\n");
}

