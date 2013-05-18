//#include <winsock2.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>


#include <sstream>
//#include "picojson.h"
#include "MeasurementLogMessage.h"
#include "Logger.h"

using namespace std;

MeasurementLogMessage::MeasurementLogMessage(char *json) : JsonWithAuxMessage()
{
	typecode = MeasurementLog;
	timestamp = 0;
	parse(json);
}

MeasurementLogMessage::MeasurementLogMessage() : JsonWithAuxMessage()
{
	typecode = MeasurementLog;
	timestamp = 0;
}

bool MeasurementLogMessage::parse(char *json)
{
	char timestampString[128];
	JsonMessage::readFieldInto(json,"timestamp",timestampString);
	timestamp = _atoi64(timestampString);
		
	char jpegsizeString[128];
	JsonMessage::readFieldInto(json,"size",jpegsizeString);
	size = atoi(jpegsizeString);
		
	return true;
}

void MeasurementLogMessage::writeJson(char *buffer)
{
	sprintf(buffer,"{ \"type\": \"measurementlog\", \"timestamp\": \"%lld\", \"size\": \"%d\" }#",timestamp,size);
}

void MeasurementLogMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","MeasurementLogMessage( timestamp=%lld size=%d )",timestamp,size);
}
