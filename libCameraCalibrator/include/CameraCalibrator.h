#ifndef __CAMERACALIBRATOR_H
#define __CAMERACALIBRATOR_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <time.h>
#include <stdio.h>

using namespace cv;

/** Calculate intrinsic and distortion coeffs parameters of camera

	Usage: (for example, with CameraLocalProxy)
	
	1. Get instantiate of CameraLocalProxy and CameraCalibrator
	CameraLocalProxy camProxy0 = CameraLocalProxy(VIDEOINPUTTYPE_GENERIC, 0);
	CameraCalibrator cameraCalibrator = CameraCalibrator();

	2. Capture image continuously
		- capture image
		- calibrate this image
		- if calibration finished, save camera params

	while(1){
		waitKey(50);
		camProxy0.CaptureImage(0, image);
		int retval = cameraCalibrator.calibrate(image, true);
		if(retval == cameraCalibrator.FINISHED)
		{
			cameraCalibrator.setCameraResolution(image.size());
			cameraCalibrator.saveCameraParams("intrinsic.xml");
			break;
		}
		
		imshow("Image View", *image);
	}
*/
class CameraCalibrator 
{
private:
	Size boardSize;
	Size imageSize;
    float squareSize;
	float aspectRatio;
    Mat* cameraMatrix;
	Mat* distCoeffs;
	Size cameraResolution;
	
    int nframes;	// number of chessboard testing
	int flags;
    int delay;
	clock_t prevTimestamp;
    vector<vector<Point2f>> imagePoints;

public:
	enum { FINISHED = 1, NOT_FINISHED = 0, CALIBRATING_FAILED = -1 };
	/** Constructor */
	CameraCalibrator();
	/** Constructor */
	CameraCalibrator(Size _boardSize, int _nframes);
	
	/** Common init function for Constructors */
	void initDefaults(Size boardSize = Size(9,6), int nframes = 10);
	/** Find and draw chessboard corners, and store imagePoints for calibration */
	int calibrate(Mat* target, bool showResultOnImage = false);
	/** Calibration and calculate intrinsic and distortion coeffs parameters */
	bool runCalibration();
	/** Calculate corners of Chessboard */
	void calcChessboardCorners(vector<Point3f>& corners);
	
	/** Save intrinsic and distortion coeffs parameters to file */
	void saveCameraParams(const string& filename);
	/** Distort the input frame */
	void showImageAsDistorted(Mat* target);
	/** Get camera matrix (intrinsic matrix) */
	Mat* getCameraMatrix();
	/** Get distortion coeffs matrix */
	Mat* getDistortionCoefficients();

	void setCameraResolution(Size _cameraResolution);

	~CameraCalibrator();
};

#endif