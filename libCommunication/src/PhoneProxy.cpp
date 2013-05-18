#include <iostream>
#include <fstream>
#include <stdio.h>
#include <winsock2.h>
#include <io.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "PhoneProxy.h"

using namespace std;

static void error_exit(char *errorMessage) {

    fprintf(stderr,"%s: %d\n", errorMessage, WSAGetLastError());
    exit(EXIT_FAILURE);
}

// ----------------------------------------------------------------------------

void PhoneProxy::RequestPhoto(long long desiredTimeStamp)
{
    char buffer[100];
    int len;
	sprintf(buffer,"{ \"type\": \"takepicture\", \"desiredtimestamp\": \"%lld\" }#",desiredTimeStamp);
    len = strlen(buffer);

    if (send(sock, buffer, len, 0) != len)
        error_exit("send() has sent a different number of bytes than excepted !!!!");
	return;
}

void PhoneProxy::RequestPing()
{
	char *cmd = "{ \"type\": \"ping\", \"desiredtimestamp\": \"0\" }#";
    int len;
    len = strlen(cmd);

    if (send(sock, cmd, len, 0) != len)
        error_exit("send() has sent a different number of bytes than expected !!!!");
	return;
}

void PhoneProxy::RequestLog()
{
	char *cmd = "{ \"type\": \"sendlog\", \"desiredtimestamp\": \"0\" }#";
    int len;
    len = strlen(cmd);

    if (send(sock, cmd, len, 0) != len)
        error_exit("send() has sent a different number of bytes than expected !!!!");
	return;
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
	while ((received = recv(sock, &c, 1, 0)) > 0) 
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
	int received = recv(sock, buffer, RCVBUFSIZE, 0);
	cout << "DEBUG RECEIVE:" << endl << buffer << endl << "END RECEIVE" << endl;
	return;
}

void PhoneProxy::Connect(char *ip, int port)
{
	struct sockaddr_in server;
    struct hostent *host_info;
    unsigned long addr;
	int iResult;

	//cout << "Connecting..." << endl;

    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD (1, 1);
    if (WSAStartup (wVersionRequested, &wsaData) != 0)
        error_exit( "Initialisation of Winsock failed");
    //else
        //printf("Winsock Initialised\n");

    sock = socket( AF_INET, SOCK_STREAM, 0 );

    if (sock < 0)
        error_exit( "Socket error");

    memset( &server, 0, sizeof (server));
    if ((addr = inet_addr( ip)) != INADDR_NONE) {
        memcpy( (char *)&server.sin_addr, &addr, sizeof(addr));
    }
    else {
        host_info = gethostbyname(ip);
        if (NULL == host_info)
            error_exit("Unknown Server");
        memcpy( (char *)&server.sin_addr,
                host_info->h_addr, host_info->h_length );
    }

    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    if(connect(sock,(struct sockaddr*)&server,sizeof(server)) <0)
        error_exit("Connection to the server failed");
}

void PhoneProxy::Disconnect()
{
	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();
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
		received = recv(sock, receiveBuffer, RCVBUFSIZE, 0);
		receivedTotalBytes += received;
		if (targetStream!=NULL)
		{
			(*targetStream).write(receiveBuffer, received); 
		}
	}
}


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
		long long timestamp = _atoi64(tmpS);

		char *posSize = strstr(buffer,"size");
		char *posSizeValue = posSize + 7;
		char *posSizeValueEnd = strstr(posSizeValue,"\"");
		len = posSizeValueEnd-posSizeValue;
		strncpy(tmpS, posSizeValue, len );
		*(tmpS+len) = 0;
		int jpegSize = _atoi64(tmpS);

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
		long long timestamp = _atoi64(tmpS);

		char *posSize = strstr(buffer,"size");
		char *posSizeValue = posSize + 7;
		char *posSizeValueEnd = strstr(posSizeValue,"\"");
		len = posSizeValueEnd-posSizeValue;
		strncpy(tmpS, posSizeValue, len );
		*(tmpS+len) = 0;
		int logSize = _atoi64(tmpS);

		// Save for external use...
		lastReceivedTimeStamp = timestamp;

		//cout << "Receiving LOG. Timestamp=" << timestamp << ", size=" << logSize << endl;
		receiveIntoStream(targetStream,sock,logSize);

		//cout << "LOG received successfully. Sent size=" << logSize << ", received size=" << jpegBytes << endl;
	}

}

