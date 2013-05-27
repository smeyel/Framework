#ifndef __TAKEPICTUREMESSAGE_H
#define __TAKEPICTUREMESSAGE_H

#include "JsonMessage.h"

/** Command message to take a picture and return it.
	If desiredtimestamp is set (nonzero), the receiver should try to take the picture in that given time.

	The answer is a JpegMessage or MatImageMessage.
*/
class TakePictureMessage : public JsonMessage
{
public:
	/** Timestamp at which the picture should be taken.
		Zero value means as soon as possible.
		Timestamp is defined w.r.t the clock of the image source (like the smartphone).
	*/
	long long desiredtimestamp;

	TakePictureMessage();
	TakePictureMessage(char *json);
	bool parse(char *json);
	virtual void log();

	virtual void writeJson(char *buffer);
};

#endif