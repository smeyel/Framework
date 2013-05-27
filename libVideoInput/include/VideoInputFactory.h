#ifndef __VIDEOINPUTFACTORY_H
#define __VIDEOINPUTFACTORY_H
#include "VideoInput.h"

/** Generic video input provided by OpenCV.
*/
#define VIDEOINPUTTYPE_GENERIC	0

/** Video input wrapping more advanced interfaces of Ps3Eye cameras.
*/
#define VIDEOINPUTTYPE_PS3EYE	1

/** Factory class to create various VideoInput implementations.
*/
class VideoInputFactory
{
public:
	/** Creates a video input with given type.
		@param	VideoInputType	Type, use the VIDEOINPUTTYPE_... defines like VIDEOINPUTTYPE_GENERIC or VIDEOINPUTTYPE_PS3EYE.
	*/
	static VideoInput *CreateVideoInput(int VideoInputType);
};

#endif
