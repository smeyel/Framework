#ifndef __JSONMESSAGE_H
#define __JSONMESSAGE_H

#define MAXTYPENAMELENGTH 100

#define DEBUG_JSON_IF_UNKNOWN

typedef enum _messagetype
{
	Default,
	Ping,
	TakePicture
} JsonMessageTypeEnum;

class JsonMessage
{
protected:
	JsonMessageTypeEnum typecode;

public:
	JsonMessage();
	JsonMessageTypeEnum getType();

	const static int typePingMessage = 0;

	static JsonMessage *parse(char *json);
	JsonMessageTypeEnum getMessageType();

	static void DebugJson(char *json);

	virtual void log();
};

#endif
