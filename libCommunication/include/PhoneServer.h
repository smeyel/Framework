#ifndef __PHONESERVER_H
#define __PHONESERVER_H

#ifdef WIN32
#include <winsock2.h>
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "PhoneProxy.h"
#include "JsonMessage.h"

using namespace std;

#define RCVBUFSIZE 8192//32768//16384//8192

class PhoneServer : public PhoneProxy
{
private:

public:
	SOCKET serversock;	// public only for debug...

	PhoneServer() : PhoneProxy()
	{
		lastReceivedTimeStamp=0;
	}

	void InitServer(int port);
	void ListenServerSocket();
	void DisconnectServer();

};

#endif
