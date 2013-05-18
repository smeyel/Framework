#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include <sstream>
#include "MatImageMessage.h"
#include "Logger.h"

using namespace std;

MatImageMessage::MatImageMessage(char *json) : JsonWithAuxMessage()
{
	typecode = MatImage;
	timestamp = 0;
	height = 0;
	width = 0;
	mattype = 0;
	mat = NULL;

	parse(json);
}

MatImageMessage::MatImageMessage() : JsonWithAuxMessage()
{
	typecode = Jpeg;
	height = 0;
	width = 0;
	mattype = 0;
	timestamp = 0;
	mat = NULL;
}

bool MatImageMessage::parse(char *json)
{
	char timestampString[128];
	JsonMessage::readFieldInto(json,"timestamp",timestampString);
	timestamp = _atoi64(timestampString);
	
	char sizeString[32];
	JsonMessage::readFieldInto(json,"size",sizeString);
	size = atoi(sizeString);
		
	char widthString[16];
	JsonMessage::readFieldInto(json,"width",widthString);
	width = atoi(widthString);
		
	char heightString[16];
	JsonMessage::readFieldInto(json,"height",heightString);
	height = atoi(heightString);
		
	char mattypeString[16];
	JsonMessage::readFieldInto(json,"mattype",mattypeString);
	mattype = atoi(mattypeString);
		
	return true;
}

void MatImageMessage::writeJson(char *buffer)
{
	sprintf(buffer,"{ \"type\": \"matimage\", \"timestamp\": \"%lld\", \"size\": \"%d\", \"width\": \"%d\", \"height\": \"%d\", \"mattype\": \"%d\" }#",timestamp,size,width,height,mattype);
}

void MatImageMessage::Decode()
{
	if (mat) delete mat;
	mat = new cv::Mat(height,width,mattype,data.data());
}

// Does not copy the image information!
void MatImageMessage::Encode(cv::Mat *srcMat)
{
	if (!srcMat->isContinuous())
	{
		cout << "MatImageMessage::Encode: !isContinuous, not supported." << endl;
		return;
	}
	if (srcMat->channels() != 4)
	{
		cout << "MatImageMessage::Encode: channels()=="<<srcMat->channels()<<" != 4, not supported." << endl;
		return;
	}
	if (srcMat->elemSize() != 4)
	{
		cout << "MatImageMessage::Encode: elemSize()=="<<srcMat->elemSize()<<" != 4, not supported." << endl;
		return;
	}
	if (srcMat->type() != CV_8UC4)
	{
		cout << "MatImageMessage::Encode: srcMat->type()=="<<srcMat->type()<<" != CV_8UC4, not supported." << endl;
		return;
	}

	height = srcMat->rows;
	width = srcMat->cols;
	mattype = srcMat->type();
	size = width*height*4;
	data.assign(srcMat->data, srcMat->data+size);
	//data = srcMat->data;
}

void MatImageMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_INFO,"Message","MatImageMessage( timestamp=%lld size=%d, width=%d, height=%d, mattype=%d }#",timestamp,size,width,height,mattype);
}
