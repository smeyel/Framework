#ifndef __SENDPOSITIONMESSAGE_H
#define __SENDPOSITIONMESSAGE_H

#include "JsonMessage.h"

/** Command message to send position information.
	A desired timestamp may be defined.
*/
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
