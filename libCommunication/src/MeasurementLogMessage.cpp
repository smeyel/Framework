//#include <winsock2.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>


#include <sstream>
//#include "picojson.h"
#include "MeasurementLogMessage.h"
#include "Logger.h"

using namespace std;

MeasurementLogMessage::MeasurementLogMessage(char *json)
{
	typecode = MeasurementLog;
	data = NULL;
	size = 0;
	timestamp = 0;
	parse(json);
}

MeasurementLogMessage::MeasurementLogMessage()
{
	typecode = MeasurementLog;
	data = NULL;
	size = 0;
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

void MeasurementLogMessage::writeAuxStream(std::ostream *targetStream)
{
	if (!data) return;
	targetStream->write((const char*)data->data(),data->size());
}


void MeasurementLogMessage::readAuxIfNeeded(int socket)
{
	stringstream ss;
	JsonMessage::receiveIntoStream(&ss,socket,size);

	ss.seekp(0, ios::end);
	stringstream::pos_type logSize = ss.tellp();
	ss.seekg(0, ios::beg);
	//cout << "JPEG size:" << jpegSize << endl;

	data = new vector<uchar>();
	for(int i=0; i<logSize; i++)
	{
		char ch;
		ss.read(&ch,1);
		data->push_back(ch);
	}
}

void MeasurementLogMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","MeasurementLogMessage( timestamp=%lld size=%d )",timestamp,size);
}
