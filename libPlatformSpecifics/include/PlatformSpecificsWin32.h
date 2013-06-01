#ifdef _WIN32
#ifndef __PLATFROMSPECIFICSWIN32_H
#define __PLATFROMSPECIFICSWIN32_H
#include "PlatformSpecifics.h"

class PlatformSpecificsWin32 : public PlatformSpecifics
{
	static PlatformSpecifics *instance;

	friend PlatformSpecifics;	// Allows it to instantiate
	PlatformSpecificsWin32();

	char lastIpAddressStr[100];//INET_ADDRSTRLEN];


public:
	virtual void SleepMs(long ms);
	virtual bool InitSocketSystem();
	virtual void CloseSocket(int socket);
	virtual void ShutdownSocketSystem();
	virtual long long atoll(const char *str);

	virtual int accept(int serversocket);
	virtual const char *GetLastRemoteIp();
	virtual int CreateServerSocket(int port);
	virtual void ListenServerSocket(int serversocket);
	virtual int send(int socket, const char *buff, int size, int flags);
	virtual int recv(int socket, char *buff, int size, int flags);
};

#endif
#endif //	_WIN32
