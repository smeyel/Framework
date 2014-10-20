#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include <sstream>
#include "MatImageMessage.h"
#include "Logger.h"
#include <stdio.h>

using namespace std;

MatImageMessage::MatImageMessage() : JsonWithAuxMessage(Jpeg), mat(NULL)
{
	root[Types::Action::KEY] = Types::Action::INFO;
	root[Types::Subject::KEY] = Types::Subject::MAT_IMAGE;
}

MatImageMessage::MatImageMessage(Json::Value root) : JsonWithAuxMessage(root, Jpeg), mat(NULL)
{
}

MatImageMessage::~MatImageMessage()
{
	delete mat;
}

void MatImageMessage::Decode()
{
	if (mat) delete mat;
	mat = new cv::Mat(getHeight(), getWidth(), getMatType(), data.data());
}

// Does not copy the image information!
void MatImageMessage::Encode(cv::Mat *srcMat)
{
	if (!srcMat->isContinuous())
	{
		cout << "MatImageMessage::Encode: !isContinuous, not supported." << endl;
		return;
	}
	if ((srcMat->channels() != 4) && (srcMat->channels() != 3))
	{
		cout << "MatImageMessage::Encode: channels()=="<<srcMat->channels()<<" != 3 or 4, not supported." << endl;
		return;
	}
	if ((srcMat->elemSize() != 4) && (srcMat->elemSize() != 3))
	{
		cout << "MatImageMessage::Encode: elemSize()=="<<srcMat->elemSize()<<" != 3 or 4, not supported." << endl;
		return;
	}
	if ((srcMat->type() != CV_8UC4) && (srcMat->type() != CV_8UC3))
	{
		cout << "MatImageMessage::Encode: srcMat->type()=="<<srcMat->type()<<" != CV_8UC3 or CV_8UC4, not supported." << endl;
		return;
	}

	setHeight(srcMat->rows);
	setWidth(srcMat->cols);
	setMatType(srcMat->type());
	int size = getWidth() * getHeight() * srcMat->elemSize();
	data.assign(srcMat->data, srcMat->data+size);
	//data = srcMat->data;
}

void MatImageMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(
			LogConfigTime::Logger::LOGLEVEL_INFO,
			"Message",
			"MatImageMessage( timestamp=%lld size=%d, width=%d, height=%d, mattype=%d }#",
					getTimestamp(), getAuxSize(), getWidth(), getHeight(), getMatType()
	);
}
