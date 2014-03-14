#include "CameraCalibrator.h"

CameraCalibrator::CameraCalibrator()
{
	CameraCalibrator::initDefaults();
}

CameraCalibrator::CameraCalibrator(Size _boardSize, int _nframes)
{
	CameraCalibrator::initDefaults(_boardSize, _nframes);
}

CameraCalibrator::~CameraCalibrator()
{
	if(cameraMatrix)
		delete cameraMatrix;
	cameraMatrix = NULL;

	if(distCoeffs)
		delete distCoeffs;
	distCoeffs = NULL;
}

void CameraCalibrator::initDefaults(Size _boardSize, int _nframes)
{
	cameraMatrix = new Mat(3, 3, CV_64F);
	distCoeffs = new Mat(8, 1, CV_64F);
	boardSize = _boardSize;
	nframes = _nframes;
	prevTimestamp = 0;
	delay = 1000;
	flags = 0;
	squareSize = 1.f;
	aspectRatio = 1.f;
	imagePoints.clear();
}

int CameraCalibrator::calibrate(Mat* target, bool showResultOnImage)
{
	Mat viewGray;
	vector<Point2f> pointbuf;

	imageSize = target->size();
	cvtColor(*target, viewGray, COLOR_BGR2GRAY);
	bool found = findChessboardCorners(*target, boardSize, pointbuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);

	if(found)
	{
		cornerSubPix(viewGray, pointbuf, Size(11,11), Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
			
		if(showResultOnImage)
			drawChessboardCorners(*target, boardSize, Mat(pointbuf), found);

		if(clock() - prevTimestamp > delay*1e-3*CLOCKS_PER_SEC)
		{
			imagePoints.push_back(pointbuf);
			prevTimestamp = clock();
		}
	}

	if(imagePoints.size() >= (unsigned)nframes )
	{
		return runCalibration() ? FINISHED : CALIBRATING_FAILED;
	}

	return NOT_FINISHED;
}

bool CameraCalibrator::runCalibration()
{
	cameraMatrix->eye(3, 3, CV_64F);
	if( flags & CV_CALIB_FIX_ASPECT_RATIO ){
		cameraMatrix->at<double>(0,0) = aspectRatio;
	}

	distCoeffs->zeros(8, 1, CV_64F);

	vector<vector<Point3f>> objectPoints(1);
	calcChessboardCorners(objectPoints[0]);

	objectPoints.resize(imagePoints.size(),objectPoints[0]);
	
	vector<Mat> rvecs, tvecs;
	calibrateCamera(objectPoints, imagePoints, imageSize, *cameraMatrix, *distCoeffs, rvecs, tvecs, flags|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);

	return (checkRange(*cameraMatrix) && checkRange(*distCoeffs));
}

void CameraCalibrator::calcChessboardCorners(vector<Point3f>& corners)
{
	corners.resize(0);

	for( int i = 0; i < boardSize.height; i++ )
		for( int j = 0; j < boardSize.width; j++ )
			corners.push_back(Point3f(float(j*squareSize), float(i*squareSize), 0));
}

void CameraCalibrator::saveCameraParams(const string& filename)
{
	FileStorage fs( filename, FileStorage::WRITE );
	fs << "Camera_Matrix" << *cameraMatrix;
	fs << "Distortion_Coefficients" << *distCoeffs;
	fs << "Camera_Resolution" << "{" << "Width" << cameraResolution.width << "Height" << cameraResolution.height << "}";
}

void CameraCalibrator::showImageAsDistorted(Mat* target)
{
	Mat temp = target->clone();
	undistort(temp, *target, *cameraMatrix, *distCoeffs);
}

Mat* CameraCalibrator::getCameraMatrix()
{
	if(cameraMatrix)
		return cameraMatrix;
	else
		return NULL;
}

Mat* CameraCalibrator::getDistortionCoefficients()
{
	if(distCoeffs)
		return distCoeffs;
	else
		return NULL;
}

void CameraCalibrator::setCameraResolution(Size _cameraResolution)
{
	cameraResolution = _cameraResolution;
}