#include "CameraLocalProxy.h"
#include "Logger.h"

#include "PlatformSpecifics.h"

using namespace LogConfigTime;

CameraLocalProxy::CameraLocalProxy(VideoInput *aVideoInput, Camera *aCamera)
	 : CameraProxy(aCamera)
{
	initDefaults();
	// Override phoneproxy from the default
	videoInput = aVideoInput;
}

CameraLocalProxy::CameraLocalProxy()
{
	initDefaults();
}

CameraLocalProxy::CameraLocalProxy(int videoInputType, int camID)
{
	initDefaults();
	initFromCameraID(videoInputType, camID);
}

CameraLocalProxy::CameraLocalProxy(const char *filename)
{
	initDefaults();
	initFromFile(filename);
}

void CameraLocalProxy::initDefaults()
{
	videoInput = NULL;
}

CameraLocalProxy::~CameraLocalProxy()
{
	if (videoInput)
	{
		videoInput->release();
	}
	videoInput=NULL;
}

// Factory wrappers
void CameraLocalProxy::initFromCameraID(int videoInputType, int camID)
{
    videoInput = VideoInputFactory::CreateVideoInput(videoInputType, camID);

	videoInput->SetNormalizedGain(-1);
	videoInput->SetNormalizedExposure(-1);
	videoInput->SetNormalizedWhiteBalance(-1,-1,-1);
}

void CameraLocalProxy::initFromFile(const char *filename)
{
    videoInput = VideoInputFactory::CreateVideoInput(VIDEOINPUTTYPE_GENERIC, filename);
}

// CaptureImage
void CameraLocalProxy::CaptureImage(long long desiredTimestamp, Mat *target)
{
	if (!videoInput)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"CameraLocalProxy","CaptureImage: VideoInput not set!\n.");
		return;
	}

	if (desiredTimestamp>0)
	{
		long long currentTimeStamp = timeMeasurement->getTimeStamp();
		while( currentTimeStamp < desiredTimestamp )
		{
			// Sleep length: calculated time minus 50ms for safety...
			long sleepMilliSec = (long)((desiredTimestamp - currentTimeStamp) / 1000 - 50);
			Logger::getInstance()->Log(Logger::LOGLEVEL_VERBOSE,"CameraLocalProxy","Waiting, sleepMilliSec = %Ld\n",sleepMilliSec);
			if (sleepMilliSec > 0)
			{
				PlatformSpecifics::getInstance()->SleepMs(sleepMilliSec);
			}
			currentTimeStamp = timeMeasurement->getTimeStamp();
		}
	}

	videoInput->captureFrame(*target);

	if (target==lastImageTaken)
		lastImageTakenTimestamp = timeMeasurement->getTimeStamp();
}
