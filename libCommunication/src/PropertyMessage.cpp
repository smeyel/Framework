#include <sstream>
#include <stdio.h>
#include <string.h> // memset
#include <cstdlib> // atoi

#include "PropertyMessage.h"
#include "Logger.h"

PropertyMessage::PropertyMessage(char *json)
{
	typecode = Property;
	memset(key,0,PROPERTY_KEY_SIZE);
	memset(value,0,PROPERTY_VALUE_SIZE);
	parse(json);
}

PropertyMessage::PropertyMessage()
{
	typecode = Property;
}

bool PropertyMessage::parse(char *json)
{
	JsonMessage::readFieldInto(json,"key",key);
	JsonMessage::readFieldInto(json,"value",value);
	return true;
}

void PropertyMessage::writeJson(char *buffer)
{
	sprintf(buffer,"{ \"type\": \"property\", \"key\": \"%s\", \"value\": \"%s\" }#",key,value);
}

void PropertyMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","PropertyMessage(%s,%s)\n",key,value);
}

void PropertyMessage::set(const char *srcKey, const char *srcValue)
{
	strcpy(this->key,srcKey);
	strcpy(this->value,srcValue);
}

void PropertyMessage::set(const char *srcKey, int srcValue)
{
	strcpy(this->key,srcKey);
	sprintf(this->value,"%d",srcValue);
}

int PropertyMessage::getIntValue()
{
	return atoi(this->value);
}
