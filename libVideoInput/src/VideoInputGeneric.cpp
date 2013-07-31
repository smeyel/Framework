#include "VideoInputGeneric.h"

#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

VideoInputGeneric::VideoInputGeneric()
{

}

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
    release();
}

void VideoInputGeneric::init(int camID)
{
	capture = new VideoCapture(camID);
	if (!capture->isOpened())
	{
		cout << "Cannot open input camera, ID= " << camID << endl;
	}
}

void VideoInputGeneric::init(const char *filename)
{
	capture = new VideoCapture(filename);
	if (!capture->isOpened())
	{
		cout << "Cannot open input file: " << filename << endl;
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

void VideoInputGeneric::release()
{
	if (capture != NULL)
	{
		delete capture;
		capture = NULL;
	}
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

