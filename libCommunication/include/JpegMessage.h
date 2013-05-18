#ifndef __JPEGMESSAGE_H
#define __JPEGMESSAGE_H

#include <iostream>
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
	JpegMessage();
	~JpegMessage()
	{
		if (data)
		{
			delete data;
		}
	}

	bool parse(char *json);
	virtual void writeJson(char *buffer);
	virtual void writeAuxStream(std::ostream *targetStream);

	virtual void readAuxIfNeeded(int socket);

	void Decode(cv::Mat *targetMat);

	virtual void log();
};

#endif