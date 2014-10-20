//#include <winsock2.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>


#include <sstream>
#include <stdio.h>
#include "JpegMessage.h"
#include "Logger.h"

using namespace std;

JpegMessage::JpegMessage() : JsonWithAuxMessage(Jpeg)
{
	root[Types::Action::KEY] = Types::Action::INFO;
	root[Types::Subject::KEY] = Types::Subject::CAMERA_IMAGE;
}

JpegMessage::JpegMessage(Json::Value root) : JsonWithAuxMessage(root, Jpeg)
{
}

void JpegMessage::Decode(cv::Mat *targetMat)
{
	if (getAuxSize()==0)
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
}

void JpegMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(
			LogConfigTime::Logger::LOGLEVEL_INFO,
			"Message",
			"JpegMessage( timestamp=%lld jpegsize=%d )", getTimestamp(), getAuxSize()
	);
}
