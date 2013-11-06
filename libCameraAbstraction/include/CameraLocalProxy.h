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

	/** Returns internal VideoInput to provide access to camera parameters
		@return	The internal VideoInput
	*/
	VideoInput *getVideoInput()
	{
		return this->videoInput;
	}

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
	virtual bool CaptureImage(long long desiredTimestamp, Mat *target);

	/** If implemented by current video input, sets gain.
		@param	value	Gain value 0-100, -1 means automatic gain.
		@return			0 for success, -1 for not implemented.
	*/
	virtual int SetNormalizedGain(int value);

	/** If implemented by current video input, sets exposure.
		@param	value	exposure value 0-100, -1 means automatic exposure.
		@return			0 for success, -1 for not implemented.
	*/
	virtual int SetNormalizedExposure(int value);

	/** If implemented by current video input, sets white balance.
		If any of r, g, and b are -1, automatic white balance is selected.
		@param	r	white balance for red channel, 0-100.
		@param	g	white balance for green channel, 0-100.
		@param	b	white balance for blue channel, 0-100.
		@return		0 for success, -1 for not implemented.
	*/
	virtual int SetNormalizedWhiteBalance(int r, int g, int b);

	/** Jumps to a given frame. Works only with video files,
		has no effect otherwise.
		@param	frameIndex	Index of the desired next frame.
	*/
	virtual void JumpToFrame(int frameIndex);
};

#endif
