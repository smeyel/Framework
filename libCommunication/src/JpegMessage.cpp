//#include <winsock2.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>


#include <sstream>
//#include "picojson.h"
#include "JpegMessage.h"
#include "Logger.h"

using namespace std;

JpegMessage::JpegMessage(char *json)
{
	typecode = Jpeg;
	data = NULL;
	size = 0;
	timestamp = 0;
	parse(json);
}

bool JpegMessage::parse(char *json)
{
	char timestampString[128];
	JsonMessage::readFieldInto(json,"timestamp",timestampString);
	timestamp = _atoi64(timestampString);
		
	char jpegsizeString[128];
	JsonMessage::readFieldInto(json,"size",jpegsizeString);
	size = atoi(jpegsizeString);
		
	return true;
}

void JpegMessage::readAuxIfNeeded(int socket)
{
	stringstream ss;
	JsonMessage::receiveIntoStream(&ss,socket,size);

	ss.seekp(0, ios::end);
	stringstream::pos_type jpegSize = ss.tellp();
	ss.seekg(0, ios::beg);
	//cout << "JPEG size:" << jpegSize << endl;

	data = new vector<uchar>();
	for(int i=0; i<jpegSize; i++)
	{
		char ch;
		ss.read(&ch,1);
		data->push_back(ch);
	}
}

void JpegMessage::Decode(cv::Mat *targetMat)
{
	cv::imdecode(cv::Mat(*data),CV_LOAD_IMAGE_COLOR,targetMat); 
}

void JpegMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","JpegMessage( timestamp=%lld jpegsize=%d )",timestamp,size);
}
