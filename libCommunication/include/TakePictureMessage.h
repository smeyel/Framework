#ifndef __TAKEPICTUREMESSAGE_H
#define __TAKEPICTUREMESSAGE_H

#include "JsonMessage.h"

/** Command message to take a picture and return it.
	If desiredtimestamp is set (nonzero), the receiver should try to take the picture in that given time.

	The answer is a JpegMessage or MatImageMessage.
*/
class TakePictureMessage : public JsonMessage
{
	static const unsigned int DESIRED_TIMESTAMP_IDX = 0;

	long long desiredTimestamp;
public:

	TakePictureMessage();
	TakePictureMessage(Json::Value root);
	virtual void log();

	virtual void pack();
	virtual void unpack();

	/** Timestamp at which the picture should be taken.
		Zero value means as soon as possible.
		Timestamp is defined w.r.t the clock of the image source (like the smartphone).
	*/
	long long getDesiredTimestamp() { return desiredTimestamp; }
	void setDesiredTimestamp(long long desiredTimestamp) { this->desiredTimestamp = desiredTimestamp; }
};

#endif
