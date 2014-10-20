#ifndef __POSITIONMESSAGE_H
#define __POSITIONMESSAGE_H

#include "JsonMessage.h"

/** Message containing positions (and other data) of detected markers
*/
class PositionMessage : public JsonMessage
{
public:

	PositionMessage();
	PositionMessage(Json::Value root);

	virtual void log();
};

#endif
