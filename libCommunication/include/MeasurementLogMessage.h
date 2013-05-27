#ifndef __MEASUREMENTLOGMESSAGE_H
#define __MEASUREMENTLOGMESSAGE_H

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "JsonWithAuxMessage.h"

/** Message containing a measurement log as aux binary data.
	For example it can be a logfile, a CSV...
*/
class MeasurementLogMessage : public JsonWithAuxMessage
{
public:
	long long timestamp;

	MeasurementLogMessage(char *json);
	MeasurementLogMessage();

	bool parse(char *json);
	virtual void writeJson(char *buffer);

	virtual void log();
};

#endif