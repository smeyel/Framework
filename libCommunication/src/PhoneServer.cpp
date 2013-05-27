#include "Logger.h"
#include "PhoneServer.h"

using namespace LogConfigTime;

int PhoneServer::InitServer(int port)
{
	struct sockaddr_in server;

	if (InitSocket())
	{
		return -1;
	}

    serversock = socket( AF_INET, SOCK_STREAM, 0 );
    if (serversock < 0)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"PhoneServer","Error: Cannot create server socket!\n");
		return -2;
	}

    memset( &server, 0, sizeof (server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	// Binding the socket
	if (bind(serversock, (struct sockaddr *) &server, sizeof(server)) < 0)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"PhoneServer","Error: Cannot bind server socket!\n");
		return -3;
	}
	Logger::getInstance()->Log(Logger::LOGLEVEL_INFO,"PhoneServer","Server socket initialized on port %d\n",port);
	return 0;
}

void PhoneServer::ListenServerSocket()
{
	listen(serversock,5);
	Logger::getInstance()->Log(Logger::LOGLEVEL_INFO,"PhoneServer","Server socket now listening.\n");
}

void PhoneServer::DisconnectServer()
{
	shutdown(serversock, SD_SEND);
	CloseSocket(serversock);
	serversock = -1;
}

// Call after Init so that port is already set...
// Connects
int PhoneServer::RegisterNode(const char *registryHost, const char *registrationURL)
{
	struct sockaddr_in server;
    struct hostent *host_info;
    unsigned long addr;

	if (InitSocket())
	{
		return -1;
	}

    SOCKET regSocket = socket( AF_INET, SOCK_STREAM, 0 );

    if (regSocket < 0)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"PhoneServer","Error: Cannot register the node, cannot create socket!\n");
		CloseSocket(regSocket);
		return -2;
	}

    memset( &server, 0, sizeof (server));
    if ((addr = inet_addr(registryHost)) != INADDR_NONE) {
        memcpy( (char *)&server.sin_addr, &addr, sizeof(addr));
    }
    else {
        host_info = gethostbyname(registryHost);
        if (NULL == host_info)
		{
			Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"PhoneServer","Error: Cannot register the node, unknown server!\n");
			CloseSocket(regSocket);
			return -3;
		}
        memcpy( (char *)&server.sin_addr,
                host_info->h_addr, host_info->h_length );
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    if(connect(regSocket,(struct sockaddr*)&server,sizeof(server)) <0)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"PhoneServer","Error: Cannot register the node, connect failed!\n");		
		CloseSocket(regSocket);
		return -4;
	}

	// Send registration HTTP GET
	char buffer[256];
	memset(buffer,0,256);

	sprintf(buffer,"GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: HTMLGET 1.0\r\n\r\n",registrationURL,registryHost);
	int len=strlen(buffer);

	unsigned int sent = 0;
	while(sent < strlen(buffer))
	{ 
		int tmpres = send(regSocket, buffer+sent, len-sent, 0);
		if(tmpres == -1){
			Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"PhoneServer","Error: Cannot register the node, send failed!\n");		
			CloseSocket(regSocket);
			return -5;
		}
		sent += tmpres;
	}

	// Close connection
	shutdown(regSocket, SD_SEND);
	CloseSocket(regSocket);
	return 0;
}

int PhoneServer::InitSocket()
{
#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD (1, 1);
    if (WSAStartup (wVersionRequested, &wsaData) != 0)
	{
		Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"PhoneServer","Error: Cannot initialize winsock!\n");
		return -1;
	}
#else
#error TODO: Socket init not implemented for this platform!
#endif
	return 0;
}

void PhoneServer::CloseSocket(SOCKET socketToClose)
{
#ifdef WIN32
	closesocket(socketToClose);
	WSACleanup();
#else
#error TODO: Socket disconnect is not implemented for this platform.
#endif
}
