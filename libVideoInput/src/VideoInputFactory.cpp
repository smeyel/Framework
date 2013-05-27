#include "VideoInputFactory.h"

#include "VideoInputGeneric.h"
#ifdef _WIN32
#include "VideoInputPs3Eye.h"
#endif

#define VIDEOINPUTTYPE_GENERIC	0
#define VIDEOINPUTTYPE_PS3EYE	1

VideoInput *VideoInputFactory::CreateVideoInput(int videoInputType)
{
	switch (videoInputType)
	{
	case VIDEOINPUTTYPE_GENERIC:
		return new VideoInputGeneric();
	break;
#ifdef _WIN32
	case VIDEOINPUTTYPE_PS3EYE:
		return new VideoInputPs3Eye();
	break;
#endif
	}
	return NULL;
}
