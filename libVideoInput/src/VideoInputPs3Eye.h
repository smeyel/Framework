#ifndef __VIDEOINPUTPS3EYE_H
#define __VIDEOINPUTPS3EYE_H
#include "VideoInput.h"
#include <opencv2/core/core.hpp>
#include "CLEyeMulticam.h"

#include <windows.h>

class VideoInputPs3Eye : public VideoInput
{
	GUID _cameraGUID;
	CLEyeCameraInstance _cam;
	CLEyeCameraColorMode _mode;
	CLEyeCameraResolution _resolution;
	float _fps;

	// Just for asserts
	int w, h;

	cv::Mat internalFrame;	// Used by capture during conversion BGRA->BGR

	int GetCameraParameterCode(int param);

public:
	void virtual init(int camID);

	/** Captures a frame.
		@warning As the PS3Eye returns CV_8UC4 (BGRA) images, a conversion to BGR image is performed.
	*/
	bool virtual captureFrame(cv::Mat &frame);
	void virtual release();

	/** Increments given camera parameter by 10
		@param parameter	The parameter to increment.
							Typical parameters: CLEYE_GAIN CLEYE_EXPOSURE CLEYE_ZOOM
		@returns			The new value of the parameter
	*/
	int IncrementCameraParameter(int param);

	/** Decrements given camera parameter by 10
		@param parameter	The parameter to increment.
							Typical parameters: CLEYE_GAIN CLEYE_EXPOSURE CLEYE_ZOOM
		@returns			The new value of the parameter
	*/
	int DecrementCameraParameter(int param);

	/** Sets Gain (0-100, -1 means auto)
	*/
	virtual int SetNormalizedGain(int value);
	/** Sets Exposure (0-100, -1 means auto)
	*/
	virtual int SetNormalizedExposure(int value);
	/** Sets White balance (0-100, -1 means auto)
	*/
	virtual int SetNormalizedWhiteBalance(int r, int g, int b);
};

#endif
