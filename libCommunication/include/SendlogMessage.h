#ifndef __SENDLOGMESSAGE_H
#define __SENDLOGMESSAGE_H

#include "JsonMessage.h"

class SendlogMessage : public JsonMessage
{
public:
	SendlogMessage(char *json);
	bool parse(char *json);
	virtual void log();
};

#endif
