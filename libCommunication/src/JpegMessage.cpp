//#include <winsock2.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>


#include <sstream>
#include <stdio.h>
//#include "picojson.h"
#include "JpegMessage.h"
#include "Logger.h"

using namespace std;

JpegMessage::JpegMessage(char *json) : JsonWithAuxMessage()
{
	typecode = Jpeg;
	timestamp = 0;
	parse(json);
}

JpegMessage::JpegMessage() : JsonWithAuxMessage()
{
	typecode = Jpeg;
	timestamp = 0;
}

bool JpegMessage::parse(char *json)
{
	char timestampString[128];
	JsonMessage::readFieldInto(json,"timestamp",timestampString);
	timestamp = atoll(timestampString);
		
	char jpegsizeString[128];
	JsonMessage::readFieldInto(json,"size",jpegsizeString);
	size = atoi(jpegsizeString);
		
	return true;
}

void JpegMessage::writeJson(char *buffer)
{
	sprintf(buffer,"{ \"type\": \"JPEG\", \"timestamp\": \"%lld\", \"size\": \"%d\" }#",timestamp,size);
}

void JpegMessage::Decode(cv::Mat *targetMat)
{
	if (size==0)
	{
		cout << "JpegMessage::Decode: Error, size==0" << endl;
		return;
	}
	cv::imdecode(cv::Mat(data),CV_LOAD_IMAGE_COLOR,targetMat); 
}

void JpegMessage::Encode(cv::Mat *srcMat)
{
	vector<int> param = vector<int>(2);
	param[0]=CV_IMWRITE_JPEG_QUALITY;
	param[1]=95;//default(95) 0-100

	imencode(".jpg",*srcMat,data,param);
	size = data.size();
}

void JpegMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","JpegMessage( timestamp=%lld jpegsize=%d )",timestamp,size);
}
