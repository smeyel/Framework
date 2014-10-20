#include <sstream>
#include <stdio.h>
#include <string.h> // memset

#include "TextMessage.h"
#include "Logger.h"

TextMessage::TextMessage() : JsonMessage(Text)
{
}

TextMessage::TextMessage(Json::Value root) : JsonMessage(root, Text)
{
	unpack();
}

void TextMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(
			LogConfigTime::Logger::LOGLEVEL_INFO,
			"Message",
			"StringMessage(%s)\n",
			content.c_str()
	);
}

void TextMessage::pack() {
	JsonMessage::pack();
	root[Types::Misc::KEY_TEXT] = content;
}

void TextMessage::unpack() {
	content = root[Types::Misc::KEY_TEXT].asString();
}

void TextMessage::setContent(const std::string& content) {
	this->content = content;
}
