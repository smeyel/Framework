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
	int InitSocket();
	void CloseSocket(SOCKET socket);

public:
	SOCKET serversock;	// public only for debug...

	PhoneServer() : PhoneProxy()
	{
		lastReceivedTimeStamp=0;
	}

	int InitServer(int port);
	void ListenServerSocket();
	void DisconnectServer();

	/** Registers the server on a SMEyeL node registry server.
		(Do not have to call InitServer before this. This only registers the servers presence.)
		The registrationURL is sent to the server defined by registryHost on port 80 as a HTTP GET request.

		@param	registryHost	Address (IP or URL) of the server, like "avalon.aut.bme.hu".
		@param	registrationURL	Path on the server for the registration script, like
								"~kristof/smeyel/smeyel_reg.php?IP=127.0.0.1:6000". (Without starting "/".)
		@return				0 for success, <0 for error codes.
	*/
	int RegisterNode(const char *registryHost, const char *registrationURL);
};

#endif
