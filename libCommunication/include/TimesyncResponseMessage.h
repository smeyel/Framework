/*
 * TimesyncResponseMessage.h
 *
 *  Created on: Dec 4, 2014
 *      Author: akos
 */

#ifndef TIMESYNCRESPONSEMESSAGE_H_
#define TIMESYNCRESPONSEMESSAGE_H_

#include "JsonMessage.h"

#include <vector>

/**
 * Simple class to store brightness values and corresponding timestamps.
 */
class FrameData {
public:
	int brightness;
	long long int timestamp;

	FrameData(int brightness, long long int timestamp) :
		brightness(brightness), timestamp(timestamp)
	{}
};

/**
 * Message containing time synchronization measurement results.
 * Stores data in a list of FrameData objects.
 */
class TimesyncResponseMessage: public JsonMessage {

	std::vector<FrameData> values;

public:
	TimesyncResponseMessage();
	TimesyncResponseMessage(Json::Value root);
	virtual ~TimesyncResponseMessage();

	virtual void pack();
	virtual void unpack();

	const std::vector<FrameData>& getValues() const {
		return values;
	}

	void setValues(const std::vector<FrameData>& values) {
		this->values = values;
	}
};

#endif /* TIMESYNCRESPONSEMESSAGE_H_ */
