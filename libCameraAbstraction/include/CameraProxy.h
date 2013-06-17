#ifndef __CAMERAPROXY_H
#define __CAMERAPROXY_H

#include "camera.h"
#include "chessboarddetector.h"
#include "TimeMeasurement.h"

/** Wraps a PhoneProxy, a Camera and a ChessboardDetector.
	Provides a convenient interface for basic operations with remote cameras.
	Exposes internal (wrapped) objects for full functionality access.

	To initialize:
	- Instantiate CameraProxy
	- You may want to set chessboarddetector.chessboard (but default 9x6 board with 36mm grid size is already set)
	- You may want to set camera->cameraID, camera->isStationary and call camera->loadCalibrationData()
	- Init the connection w.r.t the used derived class (for example call Connect() in CameraRemoteProxy).

	Usage: beside the many other functions, most important ones are the following:
	- CaptureImage()
	- TryCalibration()
	- CaptureAndTryCalibration() wraps the pervious two
	- CaptureUntilCalibrated()

	Shutting down:
	- Call Disconnect()

	To measure response time of remote cameras, the PerformCaptureSpeedMeasurement_A() function is an all-in-one
	speed measurement method. Call this after proper initialization and disconnect after that.
	Other functions operations should not be performed with this measurement to clean timing statistics clean.
*/
class CameraProxy
{
private:
	// Defaults set by constructor, may be overridden
	Camera *default_camera;	// Do not forget: cameraID, isStationary, loadCalibrationData
	ChessboardDetector *default_chessboarddetector;	// May overwrite its chessboard...

	// used by constructors
	void initDefaults();

public:
	/** Internal time measurements of CameraProxy */
	LogConfigTime::TimeMeasurement *timeMeasurement;

	/** Pointer to the used Camera. Initialized with a default one. */
	Camera *camera;
	/** Pointer to the used ChessboardDetector. Initialized with a default one. */
	ChessboardDetector *chessboarddetector;

	/** Pointer to the last captured image.
		@warning Do not access this while a new frame is received! Not thread safe!
	*/
	Mat *lastImageTaken;	// Not thread safe!
	/** The timestamp of the last received image.
		@warning Do not access this while a new frame is received! Not thread safe!
	*/
	long long lastImageTakenTimestamp;

	/** Constrcutor setting default PhoneProxy, Camera and ChessboardDetector */
	CameraProxy();
	/** Constrcutor setting custom PhoneProxy and Camera, using default ChessboardDetector
		@param aCamera		Camera to be used
	*/
	CameraProxy(Camera *aCamera);
	/** Destructor */
	~CameraProxy();

	// CaptureImage
	/** Captures an image. Result can be accessed via lastImageTaken 
		@param desiredTimestamp	The desired timestamp of the image
		@warning Derived classes have to unhide this method to use it!
	*/
	void CaptureImage(long long desiredTimestamp=0); // calls CaptureImage(lastImageTaken)
	/** Captures an image. Does not modify the image lastImageTaken.
		If target==lastImageTaken, it sets lastImageTakenTimestamp. Otherwise the received
		timestamp cannot be accessed.
		@param desiredTimestamp	The desired timestamp of the image
		@param target			The Mat the captured image is stored in.

		@warning Derived classes have to override this method to implement the actual image capture.
	*/

	virtual void CaptureImage(long long desiredTimestamp, Mat *target) = 0;

	/** Try to find a chessboard on the last taken image (accessible via lastImageTaken)
		@param showResultOnImage	If true, calibration data is written on the image.
		@returns	Returns true is calibration was successful (chassboard points were found).
	*/
	bool TryCalibration(bool showResultOnImage=false);	// calls TryCalibration(lastImageTaken,showResultOnImage)
	/** Try to find a chessboard on the given image
		@param image				The image where the chessboard is looked for.
		@param showResultOnImage	If true, calibration data is written on the image.
		@returns	Returns true is calibration was successful (chassboard points were found).
	*/
	bool TryCalibration(Mat *image, bool showResultOnImage=false);	// does not use lastImageTaken

	/** Capture image and try to find chessboard
		@param	showResultOnImage	If true, result is shown on the image.
		@returns	Returns true if the calibration was successful.
	*/
	bool CaptureAndTryCalibration(bool showResultOnImage=false);

	/** Captures images until calibration is successful.
		@param maxFrameNum	Maximal number of frames to try with.
		@returns	Returns true if calibration was successful.
	*/
	bool CaptureUntilCalibrated(int maxFrameNum);

	/** Return Ray3D for given image coordinates.
		Wrapper for Camera->pointImg2World().
		@param pImg	Point in image coordinates
		@returns	Returns the 3D ray corresponding pImg.
		@warning The camera has to be calibrated before using this function!
	*/
	Ray pointImg2World(Point2f pImg);

	/** If implemented by current video input, sets gain.
		@param	value	Gain value 0-100, -1 means automatic gain.
		@return			0 for success, -1 for not implemented.
	*/
	virtual int SetNormalizedGain(int value) { return -1; }

	/** If implemented by current video input, sets exposure.
		@param	value	exposure value 0-100, -1 means automatic exposure.
		@return			0 for success, -1 for not implemented.
	*/
	virtual int SetNormalizedExposure(int value) { return -1; }

	/** If implemented by current video input, sets white balance.
		If any of r, g, and b are -1, automatic white balance is selected.
		@param	r	white balance for red channel, 0-100.
		@param	g	white balance for green channel, 0-100.
		@param	b	white balance for blue channel, 0-100.
		@return		0 for success, -1 for not implemented.
	*/
	virtual int SetNormalizedWhiteBalance(int r, int g, int b) { return -1; }
};

#endif
