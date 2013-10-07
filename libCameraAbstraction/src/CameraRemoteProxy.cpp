#include <fstream>
#include "CameraRemoteProxy.h"

#include "Logger.h"
#include "JpegMessage.h"
#include "MatImageMessage.h"
#include "SendPositionMessage.h"
#include "TextMessage.h"

#include "MeasurementLogMessage.h"

using namespace LogConfigTime;

CameraRemoteProxy::CameraRemoteProxy(PhoneProxy *aPhoneProxy, Camera *aCamera)
	 : CameraProxy(aCamera)
{
	initDefaults();
	// Override phoneproxy from the default
	phoneproxy = aPhoneProxy;
}

CameraRemoteProxy::CameraRemoteProxy()
{
	initDefaults();
}

void CameraRemoteProxy::initDefaults()
{
	phoneproxy = default_phoneproxy = new PhoneProxy();
}

CameraRemoteProxy::~CameraRemoteProxy()
{
	if (phoneproxy)
	{
		phoneproxy->Disconnect();
	}
	if (phoneproxy == default_phoneproxy)
		phoneproxy = NULL;
	if (default_phoneproxy)
		delete default_phoneproxy;
	default_phoneproxy=NULL;
}

void CameraRemoteProxy::Connect(const char *ip, int port)
{
	phoneproxy->Connect(ip,port);
}

void CameraRemoteProxy::Disconnect()
{
	phoneproxy->Disconnect();
}


// CaptureImage
bool CameraRemoteProxy::CaptureImage(long long desiredTimestamp, Mat *target)
{
	// Request image from the phone
	phoneproxy->RequestPhoto(desiredTimestamp);
	// Receiving picture
	JsonMessage *msg = NULL;
	bool isImgValid = false;
	msg = phoneproxy->ReceiveNew();
	if (msg->getMessageType() == Jpeg)
	{
		JpegMessage *jpegMsg = NULL;
		jpegMsg = (JpegMessage *)msg;
		jpegMsg->Decode(target);

		if (target==lastImageTaken)
			lastImageTakenTimestamp = jpegMsg->timestamp;
			
		if(target->type()==CV_8UC4)	// Convert frames from CV_8UC4 to CV_8UC3
			cvtColor(*target,*target,CV_BGRA2BGR);

		isImgValid = true;
	}
	else if (msg->getMessageType() == MatImage)
	{
		MatImageMessage *matimgMsg = NULL;
		matimgMsg = (MatImageMessage *)msg;
		if (matimgMsg->size != 0)
		{
			matimgMsg->Decode();
			matimgMsg->getMat()->copyTo(*target);	// TODO: avoid this copy...

			if (target==lastImageTaken)
				lastImageTakenTimestamp = matimgMsg->timestamp;

			isImgValid = true;
		}
	}
	else
	{
		cout << "Error... received something else than JPEG or MAT... see the log for details!" << endl;
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"M2Host","Received something else than JPEG image:\n");
		msg->log();
	}
	delete msg;
	msg = NULL;
	return isImgValid;
}


// Warning: results are influenced by remote side settings, like send Jpeg or Mat!
// TODO: disable for local camera! (Or even better, modify for that!)
void CameraRemoteProxy::PerformCaptureSpeedMeasurement_A(int frameNumber, const char *resultfilename)
{
	if (!resultfilename)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"CameraProxy","PerformCaptureSpeedMeasurement_A result filename not set.\n");
		return;
	}

	// Create local time measurement object
	const int TimeFrame				= 0;	// Processing a frame
	const int TimeSend				= 1;	// Send image request and wait for reception
	const int TimeWaitAndReceive	= 2;	// Receiving the image
	const int TimeProcessImage		= 3;
	const int TimeFullExecution		= 4;	// Full execution time of the measurement

	TimeMeasurement timeMeasurement;
	timeMeasurement.setMeasurementName("CameraProxy.PerformCaptureSpeedMeasurement_A");
	timeMeasurement.setname(TimeFrame, "TimeFrameAll");
	timeMeasurement.setname(TimeSend, "TimeSend");
	timeMeasurement.setname(TimeWaitAndReceive, "TimeWaitAndReceive");
	timeMeasurement.setname(TimeProcessImage, "TimeProcessImage");
	timeMeasurement.setname(TimeFullExecution, "TimeFullExecution");
	timeMeasurement.init();

	timeMeasurement.start(TimeFullExecution);
	for(int i=0; i<frameNumber; i++)
	{
		timeMeasurement.start(TimeFrame);
		// Asking for a picture (as soon as possible)
		timeMeasurement.start(TimeSend);
		phoneproxy->RequestPhoto(0);	// TODO: Do not use phoneproxy if not needed! Local cam should work, too!
		timeMeasurement.finish(TimeSend);

		// Receiving picture
		timeMeasurement.start(TimeWaitAndReceive);

		JsonMessage *msg = NULL;
		Mat img;
		bool isImgValid = false;
		msg = phoneproxy->ReceiveNew();
		timeMeasurement.finish(TimeWaitAndReceive);
		timeMeasurement.start(TimeProcessImage);
		if (msg->getMessageType() == Jpeg)
		{
			JpegMessage *jpegMsg = NULL;
			jpegMsg = (JpegMessage *)msg;
			jpegMsg->Decode(&img);
			isImgValid = true;
		}
		else if (msg->getMessageType() == MatImage)
		{
			MatImageMessage *matimgMsg = NULL;
			matimgMsg = (MatImageMessage *)msg;
			if (matimgMsg->size != 0)
			{
				matimgMsg->Decode();
				matimgMsg->getMat()->copyTo(img);	// TODO: avoid this copy...
				isImgValid = true;
			}
		}
		else
		{
			cout << "Error... received something else than JPEG... see the log for details!" << endl;
			Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"M2Host","Received something else than JPEG image:\n");
			msg->log();
		}
		delete msg;
		msg = NULL;
		timeMeasurement.finish(TimeProcessImage);
		timeMeasurement.finish(TimeFrame);
	}
	timeMeasurement.finish(TimeFullExecution);

	ofstream resultFile;
	resultFile.open(resultfilename, ios::out | ios::app );
	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	resultFile << "===== PerformCaptureSpeedMeasurement_A results =====" << endl << "Measurement time: " << (now->tm_year + 1900) << "-" << (now->tm_mon + 1) << "-" << now->tm_mday
		<< " " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << endl;

	resultFile << "--- REMOTE measurement log ---" << endl;

	// Write remote measurement log into file
	phoneproxy->RequestLog();
	JsonMessage *msg = NULL;
	MeasurementLogMessage *logMsg = NULL;
	Mat img;
	msg = phoneproxy->ReceiveNew();
	if (msg->getMessageType() == MeasurementLog)
	{
		logMsg = (MeasurementLogMessage *)msg;
		logMsg->writeAuxStream(&resultFile);
	}
	else
	{
		cout << "Error... received something else than a measurement log... see the log for details!" << endl;
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"M2Host","Received something else than JPEG image:\n");
		msg->log();
	}

	// Append local measurement log to the file
	resultFile << "--- LOCAL PerformCaptureSpeedMeasurement_A measurement log ---" << endl;
	timeMeasurement.showresults(&resultFile);
	resultFile << "--- Further details:" << endl;
	resultFile << "max fps: " << timeMeasurement.getmaxfps(TimeFrame) << endl;
	resultFile << "Number of processed frames: " << frameNumber << endl;
	resultFile << "--- LOCAL PhoneProxy time measurement log ---" << endl;
	phoneproxy->timeMeasurement.showresults(&resultFile);

	resultFile.flush();
	resultFile.close();

	Logger::getInstance()->Log(Logger::LOGLEVEL_INFO,"CameraProxy","PerformCaptureSpeedMeasurement_A results written to %s\n.",resultfilename);
}

TextMessage *CameraRemoteProxy::SingleTrackMarker(long long desiredtimestamp, bool askImage, Mat *imageTarget)
{
	// Request image from the phone
	SendPositionMessage *sendPosMsg = new SendPositionMessage();
	sendPosMsg->desiredtimestamp = desiredtimestamp;
	sendPosMsg->sendImage = askImage ? 1 : 0;
	phoneproxy->Send(sendPosMsg);

	// Receiving TextMessage answer
	JsonMessage *msg = NULL;
	msg = phoneproxy->ReceiveNew();
	TextMessage *answer = NULL;
	if (msg->getMessageType() == Text)
	{
		answer = (TextMessage*)msg;
	}
	else
	{
		cout << "Error... received something else than TextMessage... see the log for details!" << endl;
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"CameraRemoteProxy","Received something else than TextMessage:\n");
		msg->log();
	}

	// If asked for image, receiving image
	if (askImage)
	{
		bool isImgValid = false;
		msg = phoneproxy->ReceiveNew();
		if (msg->getMessageType() == Jpeg)
		{
			JpegMessage *jpegMsg = NULL;
			jpegMsg = (JpegMessage *)msg;
			jpegMsg->Decode(imageTarget);

			if (imageTarget==lastImageTaken)
				lastImageTakenTimestamp = jpegMsg->timestamp;
			
			if(imageTarget->type()==CV_8UC4)	// Convert frames from CV_8UC4 to CV_8UC3
				cvtColor(*imageTarget,*imageTarget,CV_BGRA2BGR);

			isImgValid = true;
		}
		else if (msg->getMessageType() == MatImage)
		{
			MatImageMessage *matimgMsg = NULL;
			matimgMsg = (MatImageMessage *)msg;
			if (matimgMsg->size != 0)
			{
				matimgMsg->Decode();
				matimgMsg->getMat()->copyTo(*imageTarget);	// TODO: avoid this copy...

				if (imageTarget==lastImageTaken)
					lastImageTakenTimestamp = matimgMsg->timestamp;

				isImgValid = true;
			}
		}
		else
		{
			cout << "Error... received something else than JPEG or MAT... see the log for details!" << endl;
			Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"CameraRemoteProxy","Received something else than JPEG image:\n");
			msg->log();
		}
		delete msg;
		msg = NULL;
	}
	return answer;
}
