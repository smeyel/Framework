#ifndef __PINGMESSAGE_H
#define __PINGMESSAGE_H

#include "JsonMessage.h"

/** Ping command and answer message.

	The answer is usually another ping message.
*/
class PingMessage : public JsonMessage
{
public:
	PingMessage(Json::Value root);
	PingMessage(char *json);
	PingMessage();
	virtual ~PingMessage() {}
	bool parse(char *json);
	virtual void log();
};


#endif
