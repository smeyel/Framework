#include "Logger.h"
#include "PhoneServer.h"

using namespace LogConfigTime;

void PhoneServer::InitServer(int port)
{
	struct sockaddr_in server;
    struct hostent *host_info;
    unsigned long addr;
	int iResult;

#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD (1, 1);
    if (WSAStartup (wVersionRequested, &wsaData) != 0)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"CamClient","Error: Cannot initialize winsock!\n");
		exit(1);
	}
#else
#error TODO: Socket init not implemented for this platform!
#endif

    serversock = socket( AF_INET, SOCK_STREAM, 0 );
    if (serversock < 0)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"CamClient","Error: Cannot create server socket!\n");
		exit(2);
	}

    memset( &server, 0, sizeof (server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	// Binding the socket
	if (bind(serversock, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"CamClient","Error: Cannot bind server socket!\n");
		exit(3);
	}
	Logger::getInstance()->Log(Logger::LOGLEVEL_INFO,"CamClient","Server socket initialized on port %d\n",port);
}

void PhoneServer::ListenServerSocket()
{
	listen(serversock,5);
	Logger::getInstance()->Log(Logger::LOGLEVEL_INFO,"CamClient","Server socket now listening.\n");
}

void PhoneServer::DisconnectServer()
{
	shutdown(serversock, SD_SEND);
#ifdef WIN32
	closesocket(serversock);
	WSACleanup();
#else
#error TODO: Socket disconnect is not implemented for this platform.
#endif

	serversock = -1;
}
