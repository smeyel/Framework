#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#include "PlatformSpecificsWin32.h"

PlatformSpecificsWin32::PlatformSpecificsWin32()
{
}

void PlatformSpecificsWin32::SleepMs(long ms)
{
	Sleep(ms);
}

bool PlatformSpecificsWin32::InitSocketSystem()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD (1, 1);
	if (WSAStartup (wVersionRequested, &wsaData) != 0)
	{
		return false;
	}
	return true;
}

void PlatformSpecificsWin32::CloseSocket(int socket)
{
	closesocket(socket);
}

void PlatformSpecificsWin32::ShutdownSocketSystem()
{
	WSACleanup();
}

long long PlatformSpecificsWin32::atoll(const char *str)
{
	return _atoi64(str);
}

int PlatformSpecificsWin32::accept(int serversocket)
{
	struct sockaddr_in addr;
	SOCKET clientsock = ::accept(serversocket, (struct sockaddr *) &addr, NULL);
	inet_ntop( AF_INET, &addr.sin_addr, lastIpAddressStr, INET_ADDRSTRLEN );
	return (int)clientsock;

}

const char *PlatformSpecificsWin32::GetLastRemoteIp()
{
	return lastIpAddressStr;
}

int PlatformSpecificsWin32::CreateServerSocket(int port)
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

void PlatformSpecificsWin32::ListenServerSocket(int serversocket)
{
	listen(serversocket,5);
}

int PlatformSpecificsWin32::send(int socket, const char *buff, int size, int flags)
{
	return ::send(socket,buff, size, flags);
}

int PlatformSpecificsWin32::recv(int socket, char *buff, int size, int flags)
{
	return ::recv(socket,buff,size,flags);
}

#endif