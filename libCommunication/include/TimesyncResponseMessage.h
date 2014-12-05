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

class FrameData {
public:
	int brightness;
	long long int timestamp;

	FrameData(int brightness, long long int timestamp) :
		brightness(brightness), timestamp(timestamp)
	{}
};

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
