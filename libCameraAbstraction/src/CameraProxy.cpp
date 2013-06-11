#include <fstream>
#include "CameraProxy.h"

#include "Logger.h"
#include "JpegMessage.h"
#include "MatImageMessage.h"

#include "MeasurementLogMessage.h"

using namespace LogConfigTime;

CameraProxy::CameraProxy(Camera *aCamera)
{
	initDefaults();
	// Override camera from the default
	camera = aCamera;
}

CameraProxy::CameraProxy()
{
	initDefaults();
}

void CameraProxy::initDefaults()
{
	lastImageTaken = new Mat();

	camera = default_camera = new Camera();
	chessboarddetector = default_chessboarddetector = new ChessboardDetector(Size(9,6),36.1F);

	timeMeasurement = new TimeMeasurement();
	timeMeasurement->init();
}

CameraProxy::~CameraProxy()
{
	if (lastImageTaken)
		delete lastImageTaken;
	lastImageTaken=NULL;

	if (camera == default_camera)
		camera = NULL;
	if (default_camera)
		delete default_camera;
	default_camera=NULL;

	if (chessboarddetector == default_chessboarddetector)
		chessboarddetector = NULL;
	if (default_chessboarddetector)
		delete default_chessboarddetector;
	default_chessboarddetector=NULL;

	if (timeMeasurement)
	{
		delete timeMeasurement;
	}
	timeMeasurement=NULL;
}

void CameraProxy::CaptureImage(long long desiredTimestamp)
{
	// Calls virtual (and now abstract) capture function
	this->CaptureImage(desiredTimestamp, lastImageTaken);
}

// Capture image and try to find chessboard
bool CameraProxy::CaptureAndTryCalibration(bool showResultOnImage)
{
	CaptureImage();
	return TryCalibration(showResultOnImage);
}

bool CameraProxy::TryCalibration(bool showResultOnImage)
{
	return TryCalibration(lastImageTaken, showResultOnImage);
}

bool CameraProxy::TryCalibration(Mat *image, bool showResultOnImage)
{
	if (chessboarddetector->findChessboardInFrame(*image))
	{
		camera->calculateExtrinsicParams(chessboarddetector->chessboard.corners,chessboarddetector->pointBuf);

		// Show calibration data on the frame
		if (showResultOnImage)
		{
			drawChessboardCorners(*image,Size(9,6),chessboarddetector->pointBuf,true);
			Matx44f T = camera->GetT();
			for(int i=0; i<16; i++)
			{
				char txt[50];
				sprintf(txt, "%4.2lf",T.val[i] );
				putText( *lastImageTaken, string(txt), cvPoint( 25+(i%4)*75, 20+(i/4)*20 ), FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(255,255,0) );
			}
		}
		return true;
	}
	if (showResultOnImage)
	{
		putText( *lastImageTaken, string("Cannot find chessboard"), cvPoint( 25, 20 ), FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(255,100,0) );
	}
	return false;
}


// Ask for images until calibration is successful
bool CameraProxy::CaptureUntilCalibrated(int maxFrameNum)
{
	for(int i=0; i<maxFrameNum; i++)
	{
		if (CaptureAndTryCalibration())
			return true;
	}
	return false;
}

// Return Ray3D for given image coordinates
Ray CameraProxy::pointImg2World(Point2f pImg)
{
	return camera->pointImg2World(pImg);
}
