#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "VideoInputPs3Eye.h"
#include "VideoInputPs3EyeParameters.h"

#include "CLEyeMulticam.h"

using namespace cv;
using namespace std;

void VideoInputPs3Eye::init(int camID)
{
	_cameraGUID = CLEyeGetCameraUUID(camID);
/*	printf("Camera GUID: [%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x]\n",
					_cameraGUID.Data1, _cameraGUID.Data2, _cameraGUID.Data3,
					_cameraGUID.Data4[0], _cameraGUID.Data4[1], _cameraGUID.Data4[2],
					_cameraGUID.Data4[3], _cameraGUID.Data4[4], _cameraGUID.Data4[5],
					_cameraGUID.Data4[6], _cameraGUID.Data4[7]); */

	_mode = CLEYE_COLOR_PROCESSED;	// CLEYE_MONO_PROCESSED
	_resolution = CLEYE_VGA;		// CLEYE_QVGA
	_fps = 30.0;

	//IplImage *pCapImage;
	//PBYTE pCapBuffer = NULL;
	// Create camera instance
	_cam = CLEyeCreateCamera(_cameraGUID, _mode, _resolution, _fps);
	CV_Assert(_cam != NULL);
	if(_cam == NULL)		return;
	// Get camera frame dimensions
	CLEyeCameraGetFrameDimensions(_cam, w, h);
		
	// Set some camera parameters
	CLEyeSetCameraParameter(_cam, CLEYE_GAIN, 0);
	CLEyeSetCameraParameter(_cam, CLEYE_EXPOSURE, 511);
	CLEyeSetCameraParameter(_cam, CLEYE_ZOOM, (int)(100.0));
	CLEyeSetCameraParameter(_cam, CLEYE_ROTATION, (int)(0.0));

	// Start capturing
	CLEyeCameraStart(_cam);
}

bool VideoInputPs3Eye::captureFrame(Mat &frame)
{
	if (internalFrame.type()!=CV_8UC4)
	{
		internalFrame.release();
		// Allocate matrix if not done yet...
		internalFrame.create(h,w,CV_8UC4);
	}
	CV_Assert(internalFrame.rows == h);
	CV_Assert(internalFrame.cols == w);
	//CV_Assert(frame.type() == (_mode == CLEYE_COLOR_PROCESSED ? CV_8UC4 : CV_8UC1 ));

	CLEyeCameraGetFrame(_cam, internalFrame.data);

	cvtColor(internalFrame,frame,CV_BGRA2BGR);

	return true;
}

void VideoInputPs3Eye::release()
{
	if (_cam != NULL)
	{
		// Stop camera capture
		CLEyeCameraStop(_cam);
		// Destroy camera object
		CLEyeDestroyCamera(_cam);
		_cam = NULL;
	}
}

int VideoInputPs3Eye::GetCameraParameterCode(int param)
{
	int ClEyeSpecificParamCode = -1;
	switch (param)
	{
	case VIDEOINPUTPS3EYEPARAMETERS_GAIN:
		ClEyeSpecificParamCode = CLEYE_GAIN;
		break;
	case VIDEOINPUTPS3EYEPARAMETERS_EXPOSURE:
		ClEyeSpecificParamCode = CLEYE_EXPOSURE;
		break;
	}
	return ClEyeSpecificParamCode;
}


int VideoInputPs3Eye::IncrementCameraParameter(int param)
{
	if (!_cam)
	{
		return 0;
	}
	param = GetCameraParameterCode(param);
	CLEyeSetCameraParameter(_cam, (CLEyeCameraParameter)param, CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param)+10);
	return CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param);
}

int VideoInputPs3Eye::DecrementCameraParameter(int param)
{
	if (!_cam)
	{
		return 0;
	}
	param = GetCameraParameterCode(param);
	CLEyeSetCameraParameter(_cam, (CLEyeCameraParameter)param, CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param)-10);
	return CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param);
}

int VideoInputPs3Eye::SetNormalizedGain(int value)
{
	if (value==-1)
	{
		CLEyeSetCameraParameter(_cam, CLEYE_AUTO_GAIN, 1);
	}
	else
	{
		CLEyeSetCameraParameter(_cam, CLEYE_AUTO_GAIN, 0);
		CLEyeSetCameraParameter(_cam, CLEYE_GAIN, (value*79)/100);
	}
	return 0;
}

int VideoInputPs3Eye::SetNormalizedExposure(int value)
{
	if (value==-1)
	{
		CLEyeSetCameraParameter(_cam, CLEYE_AUTO_EXPOSURE, 1);
	}
	else
	{
		CLEyeSetCameraParameter(_cam, CLEYE_AUTO_EXPOSURE, 0);
		CLEyeSetCameraParameter(_cam, CLEYE_EXPOSURE, (value*511)/100);
	}
	return 0;
}

int VideoInputPs3Eye::SetNormalizedWhiteBalance(int r, int g, int b)
{
	if (r==-1 || g==-1 || b==-1)
	{
		CLEyeSetCameraParameter(_cam, CLEYE_AUTO_WHITEBALANCE, 1);
	}
	else
	{
		CLEyeSetCameraParameter(_cam, CLEYE_AUTO_WHITEBALANCE, 0);
		CLEyeSetCameraParameter(_cam, CLEYE_WHITEBALANCE_RED, (r*255)/100);
		CLEyeSetCameraParameter(_cam, CLEYE_WHITEBALANCE_GREEN, (g*255)/100);
		CLEyeSetCameraParameter(_cam, CLEYE_WHITEBALANCE_BLUE, (b*255)/100);
	}
	return 0;
}

