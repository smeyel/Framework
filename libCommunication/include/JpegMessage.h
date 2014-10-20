#ifndef __JPEGMESSAGE_H
#define __JPEGMESSAGE_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "JsonWithAuxMessage.h"

/** Message containing a binary JPEG file.
*/
class JpegMessage : public JsonWithAuxMessage
{
private:
	std::vector<uchar> jpegCompressorTempVect;
public:
	/** Timestamp of the image w.r.t. the clock of the image source.
	*/

	JpegMessage();
	JpegMessage(Json::Value root);

	void Decode(cv::Mat *targetMat);
	void Encode(cv::Mat *srcMat);

	virtual void log();
};

#endif
