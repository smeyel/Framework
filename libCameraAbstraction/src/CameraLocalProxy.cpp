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
	videoInput = VideoInputFactory::CreateVideoInput(videoInputType);
	videoInput->init(camID);

	videoInput->SetNormalizedGain(-1);
	videoInput->SetNormalizedExposure(-1);
	videoInput->SetNormalizedWhiteBalance(-1,-1,-1);
}

void CameraLocalProxy::initFromFile(const char *filename)
{
	videoInput = VideoInputFactory::CreateVideoInput(VIDEOINPUTTYPE_GENERIC);
	videoInput->init(filename);
}

// CaptureImage
bool CameraLocalProxy::CaptureImage(long long desiredTimestamp, Mat *target)
{
	if (!videoInput)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"CameraLocalProxy","CaptureImage: VideoInput not set!\n.");
		return false;
	}

	if (desiredTimestamp>0)
	{
		long long currentTimeStamp = timeMeasurement->getTimeStamp();
		while( currentTimeStamp < desiredTimestamp )
		{
			// Sleep length: calculated time minus 50ms for safety...
			long sleepMilliSec = (long)((desiredTimestamp - currentTimeStamp) / 1000 - 1000);	// Sleep stops 1 sec before desired timestamp
			Logger::getInstance()->Log(Logger::LOGLEVEL_VERBOSE,"CameraLocalProxy","Waiting, sleepMilliSec = %Ld\n",sleepMilliSec);
			if (sleepMilliSec > 0)
			{
				PlatformSpecifics::getInstance()->SleepMs(sleepMilliSec);
			}
			currentTimeStamp = timeMeasurement->getTimeStamp();
		}
	}

	bool isSuccessful = videoInput->captureFrame(*target);

	if (target==lastImageTaken)
		lastImageTakenTimestamp = timeMeasurement->getTimeStamp();

	return isSuccessful;
}

int CameraLocalProxy::SetNormalizedGain(int value)
{
	return videoInput->SetNormalizedGain(value);
}

int CameraLocalProxy::SetNormalizedExposure(int value)
{
	return videoInput->SetNormalizedExposure(value);
}

int CameraLocalProxy::SetNormalizedWhiteBalance(int r, int g, int b)
{
	return videoInput->SetNormalizedWhiteBalance(r,g,b);
}

void CameraLocalProxy::JumpToFrame(int frameIndex)
{
	videoInput->JumpToFrame(frameIndex);
}

