#ifndef __MATIMAGEMESSAGE_H
#define __MATIMAGEMESSAGE_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "JsonWithAuxMessage.h"

class MatImageMessage : public JsonWithAuxMessage
{
private:
	cv::Mat *mat;
public:
	long long timestamp;

	int height;
	int width;
	int mattype;

	MatImageMessage(char *json);
	MatImageMessage();

	bool parse(char *json);
	virtual void writeJson(char *buffer);

	void Decode();
	void Encode(cv::Mat *srcMat);

	cv::Mat *getMat()
	{
		// Decode() creates that...
		return mat;
	}

	virtual void log();
};

#endif