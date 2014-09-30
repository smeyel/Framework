//#ifndef _WIN32	// TODO: what should be the condition?
#ifndef __PLATFROMSPECIFICSLINUX_H
#define __PLATFROMSPECIFICSLINUX_H
#include "PlatformSpecifics.h"

class PlatformSpecificsLinux : PlatformSpecifics
{
	static PlatformSpecifics *instance;

	friend PlatformSpecifics;	// Allows it to instantiate
	PlatformSpecificsLinux();

	char lastIpAddressStr[100];//INET_ADDRSTRLEN];

public:
	virtual void SleepMs(long ms);
	virtual bool InitSocketSystem();
	virtual int Connect(const char *ip, int port);
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
//#endif // not WIN32
