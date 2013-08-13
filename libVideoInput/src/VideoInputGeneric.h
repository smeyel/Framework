#ifndef __VIDEOINPUTGENERIC_H
#define __VIDEOINPUTGENERIC_H
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "VideoInput.h"

class VideoInputGeneric : public VideoInput
{
protected:
	cv::VideoCapture *capture;
	
public:
    VideoInputGeneric(int camID);
    VideoInputGeneric(const char *filename);

    virtual ~VideoInputGeneric();

    bool virtual captureFrame(cv::Mat &frame);

    virtual int IncrementCameraParameter(int param);
    virtual int DecrementCameraParameter(int param);
    virtual int SetNormalizedGain(int value);
    virtual int SetNormalizedExposure(int value);
    virtual int SetNormalizedWhiteBalance(int r, int g, int b);
    virtual int SetFrameSize(unsigned width, unsigned height);
};

#endif
