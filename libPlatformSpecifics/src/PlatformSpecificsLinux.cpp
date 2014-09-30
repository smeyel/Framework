#ifndef _WIN32	// TODO: better condition
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> // sleep
#include <netdb.h>      // Needed for the socket functions

#include <cstdio> // itoa, NULL
#include <string.h> // memset
#include <cstdlib> // atoll

#include "PlatformSpecificsLinux.h"

PlatformSpecificsLinux::PlatformSpecificsLinux()
{
}

void PlatformSpecificsLinux::SleepMs(long ms)
{
	usleep(1000 * ms);
}

bool PlatformSpecificsLinux::InitSocketSystem()
{
	// no need to initialize anything.
	return true;
}

int PlatformSpecificsLinux::Connect(const char *ip, int port)
{
	// port # to char*
	char charport[6];
	sprintf(charport, "%d", port);
	
	//source: http://codebase.eu/tutorial/linux-socket-programming-c/
	int status;
	struct addrinfo host_info;
	struct addrinfo* host_info_list;
	
	memset(&host_info, 0, sizeof host_info);
	
	host_info.ai_family = AF_UNSPEC;
	host_info.ai_socktype = SOCK_STREAM;
	
	status = getaddrinfo(ip, charport, &host_info, &host_info_list);
	
	if (status != 0) return status; //std::cout << "getaddrinfo error" << gai_strerror(status) ;
	
	int socketfd ; // The socket descripter
	socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (socketfd == -1)  return -1; //std::cout << "socket error " ;
	
	status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
	if (status == -1)  return -1; //std::cout << "connect error" ;
	
	// cleanup
	freeaddrinfo(host_info_list);
	
	return socketfd;
}


void PlatformSpecificsLinux::CloseSocket(int socket)
{
	close(socket);
}

void PlatformSpecificsLinux::ShutdownSocketSystem()
{
	// nothing to do here
}

long long PlatformSpecificsLinux::atoll(const char *str)
{
	return ::atoll(str);
}

int PlatformSpecificsLinux::accept(int serversocket)
{
	int new_sd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    new_sd = ::accept(serversocket, (struct sockaddr *)&their_addr, &addr_size);
    
    return new_sd;
}

const char *PlatformSpecificsLinux::GetLastRemoteIp()
{
	return lastIpAddressStr;
}

int PlatformSpecificsLinux::CreateServerSocket(int port)
{
	// port # to char*
	char charport[6];
	sprintf(charport, "%d", port);
	
	//source: http://codebase.eu/tutorial/linux-socket-programming-c/
	int status;
	struct addrinfo host_info;
	struct addrinfo* host_info_list;
	
	memset(&host_info, 0, sizeof host_info);
	
	host_info.ai_family = AF_UNSPEC;
	host_info.ai_socktype = SOCK_STREAM;
	host_info.ai_flags = AI_PASSIVE;     // IP Wildcard
	
	status = getaddrinfo(NULL, charport, &host_info, &host_info_list);
	
	int socketfd ; // The socket descripter
    socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                      host_info_list->ai_protocol);
    if (socketfd == -1) return -1; // std::cout << "socket error " ;
	
	// reuse socket address
	int yes = 1;
	status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	
	status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
	if (status == -1) return -1; //std::cout << "bind error" << std::endl ;
	
	// cleanup
	freeaddrinfo(host_info_list);
	
	return socketfd;
}

void PlatformSpecificsLinux::ListenServerSocket(int serversocket)
{
	listen(serversocket,5);
}

int PlatformSpecificsLinux::send(int socket, const char *buff, int size, int flags)
{
	return ::send(socket, buff, size, flags);
}

int PlatformSpecificsLinux::recv(int socket, char *buff, int size, int flags)
{
	return ::recv(socket, buff, size, flags);
}

#endif
