#ifndef __PHONEPROXY_H
#define __PHONEPROXY_H
#ifdef WIN32
#include <winsock2.h>
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "JsonMessage.h"
#include "TimeMeasurement.h"

using namespace std;
using namespace LogConfigTime;

#define RCVBUFSIZE 8192//32768//16384//8192

class PhoneProxy
{
private:
	SOCKET sock;

	class TimeMeasurementCodeDefs
	{
	public:
		const static int ReceiveNew		= 1;
		const static int ReceiveNew_WaitAndReceiveJson	= 2;
		const static int ReceiveNew_ParseJson	= 3;
		const static int ReceiveNew_ReceiveAux	= 4;

		static void setnames(TimeMeasurement *measurement)
		{
			measurement->setMeasurementName("PhoneProxy internal time measurements");

			measurement->setname(ReceiveNew,"ReceiveNew");
			measurement->setname(ReceiveNew_WaitAndReceiveJson,"ReceiveNew_WaitAndReceiveJson");
			measurement->setname(ReceiveNew_ParseJson,"ReceiveNew_ParseJson");
			measurement->setname(ReceiveNew_ReceiveAux,"ReceiveNew_ReceiveAux");
		}
	};

	void receiveIntoStream(ostream *targetStream, SOCKET sock, long bytenum);	// Deprecated, handled by JsonMessage...

public:
	// Time measurement
	TimeMeasurement timeMeasurement;

	long long lastReceivedTimeStamp;	// Used to query the timestamp of the last reception
	PhoneProxy()
	{
		lastReceivedTimeStamp=0;
		// Prepare time measurement
		timeMeasurement.init();
		TimeMeasurementCodeDefs::setnames(&timeMeasurement);
	}

	void Connect(char *ip, int port);
	void Disconnect();

	// Wrappers for Send()
	void RequestPhoto(long long desiredTimeStamp);
	void RequestPosition();
	void RequestPing();
	void RequestLog();
	
	// Deprecated functions
	void Receive(char *filename);	// Deprecated, use ReceiveNew()
	void Receive(ostream *targetStream);	// Deprecated, use ReceiveNew()
	void ReceiveJpeg(cv::Mat *targetMat);	// Deprecated, use ReceiveNew()
	void ReceiveDebug();	// Deprecated, use ReceiveNew()

	// New interface
	void Send(JsonMessage *msg);
	JsonMessage *ReceiveNew();

	void SetSock(SOCKET newsock)
	{
		// Used on server side to define connection socket after accept()
		sock = newsock;
	}

private:
	void ProcessIncomingJSON(int sock,char *buffer, ostream *targetStream);	// Deprecated, handled by JsonMessage...
};

static void error_exit(char *errorMessage);

#endif /*__PHONEPROXY_H*/