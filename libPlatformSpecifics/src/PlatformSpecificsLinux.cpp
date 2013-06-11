#ifndef __gnu_linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "PlatformSpecificsLinux.h"

PlatformSpecificsLinux::PlatformSpecificsLinux()
{
}

void PlatformSpecificsLinux::SleepMs(long ms)
{
	if (usleep((useconds_t)ms*1000) == EINVAL)
		throw "usleep returned EINVAL. Sleeping smaller than 1s is not supported on this platform";
}

bool PlatformSpecificsLinux::InitSocketSystem()
{
	return true;
}

int PlatformSpecificsWin32::Connect(const char *ip, int port)
{
#error TODO How to connect?
/*    struct sockaddr_in server;
    struct hostent *host_info;
    unsigned long addr;

	int sock = socket( AF_INET, SOCK_STREAM, 0 );

    if (sock < 0)
        return sock;

    memset( &server, 0, sizeof (server));
    if ((addr = inet_addr( ip)) != INADDR_NONE) {
        memcpy( (char *)&server.sin_addr, &addr, sizeof(addr));
    }
    else {
        host_info = gethostbyname(ip);
        if (NULL == host_info)
            return -1;	//error_exit("Unknown Server");
        memcpy( (char *)&server.sin_addr,
                host_info->h_addr, host_info->h_length );
    }

    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    if(connect(sock,(struct sockaddr*)&server,sizeof(server)) <0)
		return -2;
        //error_exit("Connection to the server failed");

	return sock;*/
}


void PlatformSpecificsLinux::CloseSocket(int socket)
{
	close(socket);
}

void PlatformSpecificsLinux::ShutdownSocketSystem()
{
}

long long PlatformSpecificsLinux::atoll(const char *str)
{
	return atoll(str);
}

int PlatformSpecificsLinux::accept(int serversocket)
{
	struct sockaddr_in addr;
	SOCKET clientsock = ::accept(serversocket, (struct sockaddr *) &addr, NULL);
	inet_ntop( AF_INET, &addr.sin_addr, lastIpAddressStr, INET_ADDRSTRLEN );
	return (int)clientsock;

}

const char *PlatformSpecificsLinux::GetLastRemoteIp()
{
	return lastIpAddressStr;
}

int PlatformSpecificsLinux::CreateServerSocket(int port)
{
	struct sockaddr_in server;

    SOCKET serversock = socket( AF_INET, SOCK_STREAM, 0 );
    if (serversock < 0)
	{
		return -2;
	}

    memset( &server, 0, sizeof (server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	// Binding the socket
	if (bind(serversock, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		return -3;
	}

	return (int)serversock;
}

void PlatformSpecificsLinux::ListenServerSocket(int serversocket)
{
	listen(serversocket,5);
}

int PlatformSpecificsLinux::send(int socket, const char *buff, int size, int flags)
{
	return ::send(socket,buff, size, flags);
}

int PlatformSpecificsLinux::recv(int socket, char *buff, int size, int flags)
{
	return ::recv(socket,buff,size,flags);
}

#endif
