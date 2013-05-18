#ifndef __SENDPOSITIONMESSAGE_H
#define __SENDPOSITIONMESSAGE_H

#include "JsonMessage.h"

class SendPositionMessage : public JsonMessage
{
public:
	long long desiredtimestamp;

	SendPositionMessage();
	SendPositionMessage(char *json);
	bool parse(char *json);
	virtual void log();
	virtual void writeJson(char *buffer);
};

#endif
