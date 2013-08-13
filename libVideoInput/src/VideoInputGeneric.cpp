#include "VideoInputGeneric.h"

#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

VideoInputGeneric::VideoInputGeneric(int camID)
{
    capture = new VideoCapture(camID);
    if (!capture->isOpened())
    {
        cout << "Cannot open input camera, ID= " << camID << endl;
    }
}

VideoInputGeneric::VideoInputGeneric(const char *filename)
{
    capture = new VideoCapture(filename);
    if (!capture->isOpened())
    {
        cout << "Cannot open input file: " << filename << endl;
    }
}

VideoInputGeneric::~VideoInputGeneric()
{
    if (capture != NULL)
    {
        delete capture;
        capture = NULL;
    }
}

bool VideoInputGeneric::captureFrame(Mat &frame)
{
	*capture >> frame;
	if (frame.empty())
	{
		return false;
	}
	return true;
}

int VideoInputGeneric::IncrementCameraParameter(int param)
{
    (void)param;
    return -1;
}

int VideoInputGeneric::DecrementCameraParameter(int param)
{
    (void)param;
    return -1;
}

int VideoInputGeneric::SetNormalizedGain(int value)
{
    (void)value;
    return -1;
}

int VideoInputGeneric::SetNormalizedExposure(int value)
{
    (void)value;
    return -1;
}

int VideoInputGeneric::SetNormalizedWhiteBalance(int r, int g, int b)
{
    (void)r;
    (void)g;
    (void)b;
    return -1;
}

int VideoInputGeneric::SetFrameSize(unsigned width, unsigned height)
{
    (void)width;
    (void)height;
    return -1;
}
