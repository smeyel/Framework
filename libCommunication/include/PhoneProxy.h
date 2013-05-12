#ifndef __PHONEPROXY_H
#define __PHONEPROXY_H

#include <winsock2.h>

using namespace std;

#define RCVBUFSIZE 8192//32768//16384//8192

class PhoneProxy
{
private:
	SOCKET sock;

	void receiveIntoStream(ostream *targetStream, SOCKET sock, long bytenum);

public:
	long long lastReceivedTimeStamp;	// Used to query the timestamp of the last reception
	PhoneProxy()
	{
		lastReceivedTimeStamp=0;
	}

	void Connect(char *ip, int port);
	void Disconnect();

	void RequestPhoto(long long desiredTimeStamp);
	void RequestPosition();
	void RequestPing();
	void Receive(char *filename);
	void Receive(ostream *targetStream);
	void ReceiveDebug();
	void RequestLog();

private:
	void ProcessIncomingJSON(int sock,char *buffer, ostream *targetStream);
};

static void error_exit(char *errorMessage);

#endif __PHONEPROXY_H