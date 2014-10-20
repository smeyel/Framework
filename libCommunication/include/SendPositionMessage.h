#ifndef __SENDPOSITIONMESSAGE_H
#define __SENDPOSITIONMESSAGE_H

#include "JsonMessage.h"

/** Command message to send position information.
	A desired timestamp may be defined.
*/
class SendPositionMessage : public JsonMessage
{

	static const unsigned int DESIRED_TIMESTAMP_IDX = 0;
	static const unsigned int SENDIMAGE_IDX = 1;

	long long desiredTimestamp;
	int sendImage;
public:

	SendPositionMessage();
	SendPositionMessage(Json::Value root);

	virtual void log();

	virtual void pack();
	virtual void unpack();

	/** Timestamp at which the picture should be taken.
		Zero value means as soon as possible.
		Timestamp is defined w.r.t the clock of the image source (like the smartphone).
	*/
	long long getDesiredTimestamp() { return desiredTimestamp; }
	void setDesiredTimestamp(long long desiredTimestamp) { this->desiredTimestamp = desiredTimestamp; }

	/** If nonzero, a successive JpegMessage is also sent (if supported) */
	int getSendImage() { return sendImage; }
	void setSendImage(int sendImage) { this->sendImage = sendImage; }
};

#endif
