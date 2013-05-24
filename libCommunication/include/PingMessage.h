#ifndef __PINGMESSAGE_H
#define __PINGMESSAGE_H

#include "JsonMessage.h"

/** Ping command and answer message.

	The answer is usually another ping message.
*/
class PingMessage : public JsonMessage
{
public:
	PingMessage(char *json);
	PingMessage();
	bool parse(char *json);
	virtual void writeJson(char *buffer);
	virtual void log();
};

#endif
