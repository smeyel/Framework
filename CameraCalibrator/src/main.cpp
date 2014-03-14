/*
	Camera Calibrator tool based on OpenCV's calibration.cpp
	This software generates XML file about intrinsic parameters of camera,
	and camera resolution.
	Possible to use to handling local, remote camera, or input video file.

	Check calibrator.ini to get more informations about input parameters.

	Content of generated file like this:

	<?xml version="1.0"?>
	<opencv_storage>
	<Camera_Matrix type_id="opencv-matrix">
	<rows>3</rows>
	<cols>3</cols>
	<dt>d</dt>
	<data>
	3.0749079793585574e+003 0. 3.1322948590189850e+002 0.
	2.9781864386704992e+003 2.9018233278937947e+002 0. 0. 1.</data></Camera_Matrix>
	<Distortion_Coefficients type_id="opencv-matrix">
	<rows>5</rows>
	<cols>1</cols>
	<dt>d</dt>
	<data>
	1.8462543382837258e+000 -2.0673098913317656e+002
	2.4035094391108300e-003 1.0746096773667177e-002
	-1.6441591174526495e+000</data></Distortion_Coefficients>
	<Camera_Resolution>
	<Width>640</Width>
	<Height>480</Height></Camera_Resolution>
	</opencv_storage>

*/
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include "CameraLocalProxy.h"
#include "CameraCalibrator.h"
#include "CameraRemoteProxy.h"

#include "SimpleIniConfigReader.h"

#include <cctype>
#include <stdio.h>
#include <string.h>
#include <time.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	char* iniFile = "calibrator.ini";

	SimpleIniConfigReader *SIreader = new SimpleIniConfigReader(iniFile);
	ConfigReader *reader = SIreader;

	string videoInput = reader->getStringValue("Main", "videoInput");
	string intrinsicParamsFile = reader->getStringValue("Main", "cameraIntrinsicParameters");

	CameraProxy *cameraProxy = NULL;
	CameraRemoteProxy *cameraRemoteProxy = NULL;
	CameraLocalProxy *cameraLocalProxy = NULL;

	if (videoInput.compare("LOCAL") == 0)
	{
		int cameraId = reader->getIntValue("LocalCamera", "cameraId");
		string localCameraType = reader->getStringValue("LocalCamera", "videoInput");

		if (localCameraType.compare("GENERAL") == 0)
		{
			cameraProxy = cameraLocalProxy = new CameraLocalProxy(VIDEOINPUTTYPE_GENERIC, cameraId);
		}
		else
		{
			cameraProxy = cameraLocalProxy = new CameraLocalProxy(VIDEOINPUTTYPE_PS3EYE, cameraId);
		}

	}
	else if (videoInput.compare("REMOTE") == 0)
	{
		string ipAddress = reader->getStringValue("RemoteCamera", "ipAddress");
		int port = reader->getIntValue("RemoteCamera", "port");

		cameraProxy = cameraRemoteProxy = new CameraRemoteProxy();
		cameraRemoteProxy->Connect(ipAddress.c_str(), port);
	}
	else
	{
		string videoFilePath = reader->getStringValue("File", "videoFilePath");
		cameraProxy = cameraLocalProxy = new CameraLocalProxy(videoFilePath.c_str());
	}

	Size boardSize;
	boardSize.width = reader->getIntValue("Chessboard", "width");
	boardSize.height = reader->getIntValue("Chessboard", "height");
	int frames = reader->getIntValue("Chessboard", "frames");

	Mat image = Mat();
	Mat temp = Mat();
	int c;
	bool isCalibrationSuccess = false;
	cout << "If you want to exit, press 'c'." << endl;

	namedWindow("Image View");
	CameraCalibrator cameraCalibrator = CameraCalibrator(boardSize, frames);

	while (1)
	{
		// don't flood mobile device with capture image requests
		(videoInput.compare("REMOTE") == 0) ? (c = waitKey(3000)) : (c = waitKey(50));
		if (c == 'c')
			break;

		cameraProxy->CaptureImage(0, &image);

		int retval = cameraCalibrator.calibrate(&image, true);
		if (retval == cameraCalibrator.FINISHED)
		{
			cameraCalibrator.setCameraResolution(image.size());
			cameraCalibrator.saveCameraParams(intrinsicParamsFile);
			isCalibrationSuccess = true;
			break;
		}

		imshow("Image View", image);
	}

	if (isCalibrationSuccess)
	{
		cout << "Calibrated successfully!" << endl;
		cout << "If you want to show undistorted image, press 'd', or press 'c' to exit" << endl;

		bool distorted = false;
		while (1)
		{
			c = waitKey(50);
			if (c == 'c')
				break;

			if (c == 'd')
				distorted = !distorted;

			if (distorted)
			{
				image.copyTo(temp);
				cameraCalibrator.showImageAsDistorted(&temp);
				imshow("Image View", temp);
			}
			else
			{
				imshow("Image View", image);
			}
		}
	}

	if (cameraLocalProxy)
		delete cameraLocalProxy;

	if (cameraRemoteProxy)
	{
		cameraRemoteProxy->Disconnect();
		delete cameraRemoteProxy;
	}

	return 0;
}