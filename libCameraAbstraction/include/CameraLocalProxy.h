#ifndef __CAMERALOCALPROXY_H
#define __CAMERALOCALPROXY_H
#include "CameraProxy.h"
#include "VideoInput.h"

#include "VideoInputFactory.h"

class CameraLocalProxy : public CameraProxy
{
private:
	VideoInput *videoInput;

	// used by constructors
	void initDefaults();

public:
	/** Constrcutor setting default PhoneProxy, Camera and ChessboardDetector */
	CameraLocalProxy();
	/** Constrcutor setting custom PhoneProxy and Camera, using default ChessboardDetector
		@param aPhoneProxy	PhoneProxy to be used
		@param aCamera		Camera to be used
	*/
	CameraLocalProxy(VideoInput *aVideoInput, Camera *aCamera);
	
	/** Constrcutor creating VideoInput of given type for given camera ID
		@param videoInputType	Type, use the VIDEOINPUTTYPE_... defines like VIDEOINPUTTYPE_GENERIC or VIDEOINPUTTYPE_PS3EYE.
								See VideoInputFactory for details.
		@param camID			ID of camera to be used
	*/
	CameraLocalProxy(int videoInputType, int camID);

	/** Constrcutor creating VideoInput for given input file
		@param filename		Name of the file. Can be image or video.
	*/
	CameraLocalProxy(const char *filename);
	
	/** Destructor */
	~CameraLocalProxy();

	/** Initializes a VideoInput for use with given type for given camera ID.
		This method wraps the VideoInputFactory call.
		@param videoInputType	Type, use the VIDEOINPUTTYPE_... defines like VIDEOINPUTTYPE_GENERIC or VIDEOINPUTTYPE_PS3EYE.
								See VideoInputFactory for details.
		@param camID			ID of camera to be used
	*/
	void initFromCameraID(int videoInputType, int camID);

	/** Constrcutor creating VideoInput for given input file
		This method wraps the VideoInputFactory call.
		@param filename		Name of the file. Can be image or video.
	*/
	void initFromFile(const char *filename);

	using CameraProxy::CaptureImage;

	/** Captures an image. Does not modify the image lastImageTaken.
		If target==lastImageTaken, it sets lastImageTakenTimestamp. Otherwise the received
		timestamp cannot be accessed. This condition is true if it was called via CameraProxy::CaptureImage()
		@param desiredTimestamp	The desired timestamp of the image
		@param target			The Mat the captured image is stored in.
	*/
	virtual void CaptureImage(long long desiredTimestamp, Mat *target);
};

#endif
