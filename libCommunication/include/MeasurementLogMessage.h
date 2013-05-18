#ifndef __MEASUREMENTLOGMESSAGE_H
#define __MEASUREMENTLOGMESSAGE_H

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "JsonMessage.h"

class MeasurementLogMessage : public JsonMessage
{
public:
	long long timestamp;
	int size;
	std::vector<uchar> *data;

	MeasurementLogMessage(char *json);
	MeasurementLogMessage();
	~MeasurementLogMessage()
	{
		if (data)
		{
			delete data;
		}
	}

	bool parse(char *json);
	virtual void writeJson(char *buffer);
	virtual void writeAuxStream(std::ostream *targetStream);
	virtual void readAuxIfNeeded(int socket);

	virtual void log();
};

#endif