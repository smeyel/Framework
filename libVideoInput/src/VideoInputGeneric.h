#ifndef __VIDEOINPUTGENERIC_H
#define __VIDEOINPUTGENERIC_H
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "VideoInput.h"

class VideoInputGeneric : public VideoInput
{
private:
	cv::VideoCapture *capture;
	
public:
    VideoInputGeneric() __attribute__ ((deprecated));
    VideoInputGeneric(int camID);
    VideoInputGeneric(const char *filename);

    ~VideoInputGeneric();
    void virtual init(int camID) __attribute__ ((deprecated));
    void virtual init(const char *filename) __attribute__ ((deprecated));

    bool virtual captureFrame(cv::Mat &frame);
	void virtual release();

    virtual int IncrementCameraParameter(int param);
    virtual int DecrementCameraParameter(int param);
    virtual int SetNormalizedGain(int value);
    virtual int SetNormalizedExposure(int value);
    virtual int SetNormalizedWhiteBalance(int r, int g, int b);
};

#endif
