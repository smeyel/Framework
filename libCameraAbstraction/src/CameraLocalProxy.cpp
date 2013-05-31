#include "CameraLocalProxy.h"
#include "Logger.h"

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
}

void CameraLocalProxy::initFromFile(const char *filename)
{
	videoInput = VideoInputFactory::CreateVideoInput(VIDEOINPUTTYPE_GENERIC);
	videoInput->init(filename);
}

// CaptureImage
void CameraLocalProxy::CaptureImage(long long desiredTimestamp, Mat *target)
{
	// TODO: support for desiredtimestamp
	if (videoInput)
	{
		videoInput->captureFrame(*target);
	}
	else
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"CameraLocalProxy","CaptureImage: VideoInput not set!\n.");
		return;
	}
}
