#ifndef __JPEGMESSAGE_H
#define __JPEGMESSAGE_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "JsonMessage.h"

class JpegMessage : public JsonMessage
{
public:
	long long timestamp;
	int size;
	std::vector<uchar> *data;

	JpegMessage(char *json);
	~JpegMessage()
	{
		if (data)
		{
			delete data;
		}
	}

	bool parse(char *json);
	virtual void readAuxIfNeeded(int socket);

	void Decode(cv::Mat *targetMat);

	virtual void log();
};

#endif