#ifndef __SENDLOGMESSAGE_H
#define __SENDLOGMESSAGE_H

#include "JsonMessage.h"

class SendlogMessage : public JsonMessage
{
public:
	SendlogMessage(char *json);
	SendlogMessage();
	bool parse(char *json);
	virtual void writeJson(char *buffer);

	virtual void log();
};

#endif
