#include <sstream>
#include <stdio.h>
#include <string.h> // memset
#include <cstdlib> // atoi

#include "PropertyMessage.h"
#include "Logger.h"

#include <cstdio>
#include <string>

using namespace std;


PropertyMessage::PropertyMessage() : JsonMessage(Property)
{
}

PropertyMessage::PropertyMessage(Json::Value root) : JsonMessage(root, Property)
{
	unpack();
}

void PropertyMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(
			LogConfigTime::Logger::LOGLEVEL_INFO,
			"Message",
			"PropertyMessage(%s,%s)\n",
			key.c_str(), value.c_str()
	);
}

void PropertyMessage::pack() {
	JsonMessage::pack();
	// TODO No 'key' and 'value' fields in RAR protocol...
	root["key"] = key;
	root["value"] = value;
}

void PropertyMessage::unpack() {
	// TODO No 'key' and 'value' fields in RAR protocol...
	key = root["key"].asString();
	value = root["value"].asString();
}

void PropertyMessage::set(const std::string key, const std::string value) {
	this->key = key;
	this->value = value;
}

void PropertyMessage::set(const std::string key, const int value) {
	this->key = key;
	char tmp[20];
	sprintf(tmp,"%d",value);
	this->value = std::string(tmp);
}

void PropertyMessage::set(const std::string key, const double value) {
	this->key = key;
	char tmp[20];
	sprintf(tmp,"%lf",value);
	this->value = std::string(tmp);
}

int PropertyMessage::getIntValue() {
	if (value.size() > 0) {
		return atoi(value.c_str());
	}
	return 0;
}

std::string PropertyMessage::getStringValue() {
	return value;
}

double PropertyMessage::getDoubleValue() {
	if (value.size() > 0) {
		return atof(value.c_str());
	}
	return 0.0;
}
