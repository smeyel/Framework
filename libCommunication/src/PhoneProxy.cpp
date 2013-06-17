#include <iostream>
#include <fstream>
#include <stdio.h>

#include "PlatformSpecifics.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "PhoneProxy.h"

#include "TakePictureMessage.h"
#include "SendlogMessage.h"
#include "PingMessage.h"
#include "SendPositionMessage.h"

#include "PlatformSpecifics.h"

#define MAXJSONSIZE 4096
#define MAXTYPENAMELENGTH 128

using namespace std;

void error_exit(const char *errorMessage)
{
#ifdef _WIN32
    fprintf(stderr,"%s: %d\n", errorMessage, WSAGetLastError());
#endif
    exit(EXIT_FAILURE);
}

// ----------------------------------------------------------------------------

void PhoneProxy::RequestPhoto(long long desiredTimeStamp)
{
	TakePictureMessage msg;
	msg.desiredtimestamp = desiredTimeStamp;
	Send(&msg);
}

void PhoneProxy::RequestPosition()
{
	SendPositionMessage msg;
	Send(&msg);
}

void PhoneProxy::RequestPing()
{
	PingMessage msg;
	Send(&msg);
}

void PhoneProxy::RequestLog()
{
	SendlogMessage msg;
	Send(&msg);
}

void PhoneProxy::Receive(char *filename)
{
	ofstream targetStream;
	targetStream.open(filename,ofstream::binary);
	Receive(&targetStream);
	targetStream.flush();
	targetStream.close();
}

void PhoneProxy::ReceiveJpeg(cv::Mat *targetMat)
{
	stringstream ss;
	Receive(&ss);

	// Decoding JPEG
	ss.seekp(0, ios::end);
	stringstream::pos_type jpegSize = ss.tellp();
	ss.seekg(0, ios::beg);
	//cout << "JPEG size:" << jpegSize << endl;

	vector<uchar> jpeg;
	for(int i=0; i<jpegSize; i++)
	{
		char ch;
		ss.read(&ch,1);
		jpeg.push_back(ch);
	}
	
	cv::imdecode(cv::Mat(jpeg),CV_LOAD_IMAGE_COLOR,targetMat); 
}

void PhoneProxy::Receive(ostream *targetStream)
{
	// Receive response
	int totalBytes = 0;
	char buffer[RCVBUFSIZE] = "";
	int received = 0;

	// Read JSON part (1 byte at once)
	char c;
	char *bufPtr = buffer;
	*bufPtr = 0;
	while ((received = PlatformSpecifics::getInstance()->recv(sock, &c, 1, 0)) > 0) 
	{
		if (c != '#')	// Not at end of JSON
		{
			*bufPtr = c;
			bufPtr++;
			*bufPtr = 0;
		}
		else
		{
			if (bufPtr-buffer>2)	// Do not stop for UTF-8 initial 2 bytes
				break;
		}
	}
	ProcessIncomingJSON(sock,buffer+1,targetStream);
}

void PhoneProxy::ReceiveDebug()
{
	// Receive response
	char buffer[RCVBUFSIZE] = "";
	int received = PlatformSpecifics::getInstance()->recv(sock, buffer, RCVBUFSIZE, 0);
	cout << "DEBUG RECEIVE:" << endl << buffer << endl << "END RECEIVE" << endl;
	return;
}

void PhoneProxy::Connect(const char *ip, int port)
{
	//cout << "Connecting..." << endl;
	if (!PlatformSpecifics::getInstance()->InitSocketSystem())
	{
        error_exit( "Initialisation of Winsock failed");
	}

	sock = PlatformSpecifics::getInstance()->Connect(ip,port);

	if(sock<0)
	{
        error_exit( "Connecting failed");
	}
}

void PhoneProxy::Disconnect()
{
	PlatformSpecifics::getInstance()->CloseSocket(sock);
	PlatformSpecifics::getInstance()->ShutdownSocketSystem();
	sock = -1;
}

// targetStream may be NULL, given number of bytes will be read anyway.
void PhoneProxy::receiveIntoStream(ostream *targetStream, SOCKET sock, long bytenum)
{
	char receiveBuffer[RCVBUFSIZE];
	long receivedTotalBytes = 0;
	int received;
	while (receivedTotalBytes < bytenum)
	{
		received = PlatformSpecifics::getInstance()->recv(sock, receiveBuffer, RCVBUFSIZE, 0);
		receivedTotalBytes += received;
		if (targetStream!=NULL)
		{
			(*targetStream).write(receiveBuffer, received); 
		}
	}
}

// TODO: deprecated???
void PhoneProxy::ProcessIncomingJSON(int sock,char *buffer, ostream *targetStream)
{
	//cout << "JSON received:" << endl << buffer << endl << "End of JSON" << endl;

	// Parse JSON
	char *posJPEG = strstr(buffer,"JPEG");
	char *posTimeStamp = strstr(buffer,"timestamp");
	char *posPong = strstr(buffer,"pong");
	char *posLog = strstr(buffer, "measurementlog");

	if (posPong)
	{
		//cout << "PONG received, nothing left to do..." << endl;
	}
	else if (posJPEG)
	{
		char tmpS[100];
		char *posTimeStampValue = posTimeStamp + 12;
		char *posTimeStampValueEnd = strstr(posTimeStampValue,"\"");
		int len = posTimeStampValueEnd-posTimeStampValue;
		strncpy(tmpS, posTimeStampValue, len );
		*(tmpS+len) = 0;
		long long timestamp = PlatformSpecifics::getInstance()->atoll(tmpS);

		char *posSize = strstr(buffer,"size");
		char *posSizeValue = posSize + 7;
		char *posSizeValueEnd = strstr(posSizeValue,"\"");
		len = posSizeValueEnd-posSizeValue;
		strncpy(tmpS, posSizeValue, len );
		*(tmpS+len) = 0;
		int jpegSize = atoi(tmpS);

		// Save for external use...
		lastReceivedTimeStamp = timestamp;

		//cout << "Receiving JPEG. Timestamp=" << timestamp << ", size=" << jpegSize << endl;
	
		receiveIntoStream(targetStream,sock,jpegSize);
		//cout << "JPEG received successfully. Sent size=" << jpegSize << ", timestamp=" << timestamp << endl;
	} else if(posLog)
	{
		char tmpS[100];
		char *posTimeStampValue = posTimeStamp + 12;
		char *posTimeStampValueEnd = strstr(posTimeStampValue,"\"");
		int len = posTimeStampValueEnd-posTimeStampValue;
		strncpy(tmpS, posTimeStampValue, len );
		*(tmpS+len) = 0;
		long long timestamp = PlatformSpecifics::getInstance()->atoll(tmpS);

		char *posSize = strstr(buffer,"size");
		char *posSizeValue = posSize + 7;
		char *posSizeValueEnd = strstr(posSizeValue,"\"");
		len = posSizeValueEnd-posSizeValue;
		strncpy(tmpS, posSizeValue, len );
		*(tmpS+len) = 0;
		int logSize = PlatformSpecifics::getInstance()->atoll(tmpS);

		// Save for external use...
		lastReceivedTimeStamp = timestamp;

		//cout << "Receiving LOG. Timestamp=" << timestamp << ", size=" << logSize << endl;
		receiveIntoStream(targetStream,sock,logSize);

		//cout << "LOG received successfully. Sent size=" << logSize << ", received size=" << jpegBytes << endl;
	}
}

void PhoneProxy::Send(JsonMessage *msg)
{
    char buffer[MAXJSONSIZE];
	msg->writeJson(buffer);
    int len = strlen(buffer);
	if (PlatformSpecifics::getInstance()->send(sock, buffer, len, 0) != len)
        error_exit("send() has sent a different number of bytes than excepted !!!!");
	msg->writeAux(sock);
	return;
}

JsonMessage *PhoneProxy::ReceiveNew()
{
	timeMeasurement.start(PhoneProxy::TimeMeasurementCodeDefs::ReceiveNew);
	// Receive response
	int totalBytes = 0;
	char buffer[MAXJSONSIZE] = "";
	int received = 0;

	// Read JSON part (1 byte at once)
	char c;
	char *bufPtr = buffer;
	*bufPtr = 0;
	timeMeasurement.start(PhoneProxy::TimeMeasurementCodeDefs::ReceiveNew_WaitAndReceiveJson);
	do
	{
		received = PlatformSpecifics::getInstance()->recv(sock, &c, 1, 0);
		if (received==0)
		{
			// Error: JSON not finished, but input ended...
			cout << "ERROR: Connection closed, JSON not finished properly..." << endl;
			return NULL;
		}
		else if (received<0)
		{
			// Socket read error
#ifdef _WIN32
            cout << "recv() failed: " << WSAGetLastError() << endl;
#endif
			return NULL;
		}

		if (c != '#' && c != 0)	// Not at end of JSON
		{
			*bufPtr = c;
			bufPtr++;
			*bufPtr = 0;
		}
		else
		{
			if (bufPtr-buffer>2)	// Do not stop for UTF-8 initial 2 bytes
				break;
		}
	} while (received > 0);

	timeMeasurement.finish(PhoneProxy::TimeMeasurementCodeDefs::ReceiveNew_WaitAndReceiveJson);

	char *jsonBuffer = buffer;

	//cout << "ProcessIncomingJson: " << endl << jsonBuffer << endl << "End of JSON" << endl;
	if (*buffer==0)
	{
		// Empty message, possibly the connection was closed from the remote side.
		cout << "Connection closed (empty message), JSON not finished properly..." << endl;
		return NULL;
	}

	// Instantiate appropriate JsonMessage
	timeMeasurement.start(PhoneProxy::TimeMeasurementCodeDefs::ReceiveNew_ParseJson);
	JsonMessage *jsonMsg = JsonMessage::parse(jsonBuffer);
	timeMeasurement.finish(PhoneProxy::TimeMeasurementCodeDefs::ReceiveNew_ParseJson);

	// Ask the message to read its auxiliary data from the socket, if any.
	timeMeasurement.start(PhoneProxy::TimeMeasurementCodeDefs::ReceiveNew_ReceiveAux);
	jsonMsg->readAuxIfNeeded(sock);
	timeMeasurement.finish(PhoneProxy::TimeMeasurementCodeDefs::ReceiveNew_ReceiveAux);

	timeMeasurement.finish(PhoneProxy::TimeMeasurementCodeDefs::ReceiveNew);
	return jsonMsg;
}
