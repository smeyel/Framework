#ifndef __PHONEPROXY_H
#define __PHONEPROXY_H

#include <winsock2.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "JsonMessage.h"

using namespace std;

#define RCVBUFSIZE 8192//32768//16384//8192

class PhoneProxy
{
private:
	SOCKET sock;

	void receiveIntoStream(ostream *targetStream, SOCKET sock, long bytenum);	// Deprecated, handled by JsonMessage...

public:
	long long lastReceivedTimeStamp;	// Used to query the timestamp of the last reception
	PhoneProxy()
	{
		lastReceivedTimeStamp=0;
	}

	void Connect(char *ip, int port);
	void Disconnect();

	// Wrappers for Send()
	void RequestPhoto(long long desiredTimeStamp);
	void RequestPing();
	void RequestLog();
	
	void Receive(char *filename);	// Deprecated, use ReceiveNew()
	void Receive(ostream *targetStream);	// Deprecated, use ReceiveNew()
	void ReceiveJpeg(cv::Mat *targetMat);	// Deprecated, use ReceiveNew()
	void ReceiveDebug();	// Deprecated, use ReceiveNew()

	// New interface
	void Send(JsonMessage *msg);
	JsonMessage *ReceiveNew();

private:
	void ProcessIncomingJSON(int sock,char *buffer, ostream *targetStream);	// Deprecated, handled by JsonMessage...
};

static void error_exit(char *errorMessage);

#endif __PHONEPROXY_H