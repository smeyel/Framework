#ifndef __JSONMESSAGE_H
#define __JSONMESSAGE_H

#include <iostream>
#include <winsock2.h>

#define MAXTYPENAMELENGTH 100

#define DEBUG_JSON_IF_UNKNOWN

typedef enum _messagetype
{
	Default,
	Ping,
	TakePicture,
	Sendlog,
	Jpeg,
	MeasurementLog,
	SendPosition
} JsonMessageTypeEnum;

class JsonMessage
{
protected:
	JsonMessageTypeEnum typecode;

	static void receiveIntoStream(std::ostream *targetStream, SOCKET sock, long bytenum);

public:
	JsonMessage();
	//JsonMessageTypeEnum getType();

	const static int typePingMessage = 0;

	static JsonMessage *parse(char *json);
	JsonMessageTypeEnum getMessageType();

	virtual void readAuxIfNeeded(int socket) { }

	virtual void writeJson(char *buffer) { }
	void writeAuxFile(char *filename);	// No need to override, uses writeAuxStream.
	virtual void writeAuxStream(std::ostream *targetStream) { }
	virtual void writeAux(int socket) { }

	static void DebugJson(char *json);

	static void readFieldInto(char *json, char *fieldname, char *resultBuffer);

	virtual void log();
};

#endif
