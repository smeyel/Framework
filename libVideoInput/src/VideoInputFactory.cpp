#include "VideoInputFactory.h"

#include "VideoInputGeneric.h"
#ifdef _WIN32
#include "VideoInputPs3Eye.h"
#endif
#ifdef __gnu_linux__
#include "VideoInputV4L2.h"
#endif

//TODO videoInputType sould be an enumeration
VideoInput *VideoInputFactory::CreateVideoInput(int videoInputType, int CamID)
{
	switch (videoInputType)
	{
	case VIDEOINPUTTYPE_GENERIC:
        return new VideoInputGeneric(CamID);
	break;
#ifdef _WIN32
	case VIDEOINPUTTYPE_PS3EYE:
		return new VideoInputPs3Eye();
	break;
#endif
#ifdef __gnu_linux__
    case VIDEOINPUTTYPE_V4L2:
        return new VideoInputV4L2();
        break;
#endif
	}
	return NULL;
}

VideoInput *VideoInputFactory::CreateVideoInput(int videoInputType, const char* filename)
{
    switch (videoInputType)
    {
    case VIDEOINPUTTYPE_GENERIC:
        return new VideoInputGeneric(filename);
        break;
    default:
        //TODO exception?
        break;
    }
    return NULL;
}
