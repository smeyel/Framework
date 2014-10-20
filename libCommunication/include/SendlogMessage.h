#ifndef __SENDLOGMESSAGE_H
#define __SENDLOGMESSAGE_H

#include "JsonMessage.h"

/** Command message to send the measurement log.

	Answer is usually a MeasurementLogMessage.
*/
class SendlogMessage : public JsonMessage
{
public:
	SendlogMessage();
	SendlogMessage(Json::Value root);

	virtual void log();
};

#endif
