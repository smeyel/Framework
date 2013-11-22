#ifndef __POSITIONMESSAGE_H
#define __POSITIONMESSAGE_H

#include "JsonMessage.h"

/** Message containing positions (and other data) of detected markers
*/
class PositionMessage : public JsonMessage
{
public:
	long long timestamp;
	int size;

	PositionMessage();
	PositionMessage(char *json);

	bool parse(char *json);
	virtual void writeJson(char *buffer);

	virtual void log();
};

#endif
