#ifndef __CAMERAREMOTEPROXY_H
#define __CAMERAREMOTEPROXY_H
#include "CameraProxy.h"
#include "PhoneProxy.h"
#include "TextMessage.h"

/** Before using the remote camera, call Connect()!
*/
class CameraRemoteProxy : public CameraProxy
{
private:
	// Defaults set by constructor, may be overridden
	PhoneProxy *default_phoneproxy;

	// used by constructors
	void initDefaults();

public:
	/** Pointer to the used PhoneProxy. Initialized with a default one. */
	PhoneProxy *phoneproxy;

	/** Constrcutor setting default PhoneProxy, Camera and ChessboardDetector */
	CameraRemoteProxy();
	/** Constrcutor setting custom PhoneProxy and Camera, using default ChessboardDetector
		@param aPhoneProxy	PhoneProxy to be used
		@param aCamera		Camera to be used
	*/
	CameraRemoteProxy(PhoneProxy *aPhoneProxy, Camera *aCamera);
	/** Destructor */
	~CameraRemoteProxy();

	/** Wrapper for PhoneProxy->Connect() for convenience
		@param	ip		IPv4 address of the remote camera, as a string like "127.0.0.1"
		@param	port	TCP port the remote camera is listening to
	*/
	void Connect(const char *ip, int port);
	/** Wrapper for PhoneProxy->Disconnect() for convenience */
	void Disconnect();

	using CameraProxy::CaptureImage;
	/** Captures an image. Does not modify the image lastImageTaken.
		If target==lastImageTaken, it sets lastImageTakenTimestamp. Otherwise the received
		timestamp cannot be accessed. This condition is true if it was called via CameraProxy::CaptureImage()
		@param desiredTimestamp	The desired timestamp of the image
		@param target			The Mat the captured image is stored in.
	*/
	virtual bool CaptureImage(long long desiredTimestamp, Mat *target);

	/** Image transfer speed measurement
		Use only alone (no prior or later operations) to keep the statistics clean
		Use Connect before and Disconnect after calling it.
		@param frameNumber	The number of frames to capture after each other as fast as possible.
		@param resultfilename	The name of the file the resuls are written to.
	*/
	void PerformCaptureSpeedMeasurement_A(int frameNumber=100, const char *resultfilename = NULL);

	TextMessage *SingleTrackMarker(long long desiredtimestamp, bool askImage, Mat *imageTarget);

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

};

#endif
