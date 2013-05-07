#ifndef __PHONEPROXY_H
#define __PHONEPROXY_H

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <winsock.h>
#include <io.h>

#include <windows.h>

using namespace std;

#define RCVBUFSIZE 8192//32768//16384//8192

class PhoneProxy
{
private:
	SOCKET sock;

public:
	void Connect(char *ip, int port);
	void Disconnect();

	void RequestPhoto(long long desiredTimeStamp);
	void RequestPing();
	void Receive(char *filename);	// For PONG, filename has no effect.
	void ReceiveDebug();
	void RequestLog();

private:
	void ProcessIncomingJSON(int sock,char *buffer, char *filename);
};

static void error_exit(char *errorMessage);

#endif __PHONEPROXY_H