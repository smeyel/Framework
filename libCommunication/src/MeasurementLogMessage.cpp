//#include <winsock2.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include <stdio.h>
#include <sstream>
//#include "picojson.h"
#include "MeasurementLogMessage.h"
#include "Logger.h"

using namespace std;


MeasurementLogMessage::MeasurementLogMessage() : JsonWithAuxMessage(MeasurementLog)
{
	root[Types::Action::KEY] = Types::Action::INFO;
	root[Types::Subject::KEY] = Types::Subject::LOG;
}

MeasurementLogMessage::MeasurementLogMessage(Json::Value root) : JsonWithAuxMessage(root, MeasurementLog)
{
}



void MeasurementLogMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(
			LogConfigTime::Logger::LOGLEVEL_INFO,
			"Message",
			"MeasurementLogMessage( timestamp=%lld size=%d )", getTimestamp(), getAuxSize()
	);
}
