#include "Logger.h"
#include "PhoneServer.h"
#include <stdio.h>

#include "PlatformSpecifics.h"	// Handles socket-related includes as well

using namespace LogConfigTime;

int PhoneServer::InitServer(int port)
{
	serverport = port;	// Save for cases when we have to re-init the connection.

	if (InitSocket())
	{
		return -1;
	}

	serversock = PlatformSpecifics::getInstance()->CreateServerSocket(serverport);
	Logger::getInstance()->Log(Logger::LOGLEVEL_INFO,"PhoneServer","Server socket initialized on port %d\n",port);
	return 0;
}

void PhoneServer::ListenServerSocket()
{
	PlatformSpecifics::getInstance()->ListenServerSocket(serversock);
	//listen(serversock,5);
	Logger::getInstance()->Log(Logger::LOGLEVEL_INFO,"PhoneServer","Server socket now listening.\n");
}

void PhoneServer::DisconnectServer()
{
	PlatformSpecifics::getInstance()->CloseSocket(serversock);
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
	return PlatformSpecifics::getInstance()->InitSocketSystem() ? 0 : 1;
}

void PhoneServer::CloseSocket(SOCKET socketToClose)
{
	PlatformSpecifics::getInstance()->CloseSocket(socketToClose);
	PlatformSpecifics::getInstance()->ShutdownSocketSystem();
}

void PhoneServer::Run()
{
	ListenServerSocket();

	mainLoopRunning=true;
	while(mainLoopRunning)
	{
		// Wait for connection
		Logger::getInstance()->Log(Logger::LOGLEVEL_INFO,"PhoneServer","Waiting for connection\n");
		cout << "Waiting for connection." << endl;
		//struct sockaddr_in addr;
		//SOCKET clientsock = accept(serversock, (struct sockaddr *) &addr, NULL);
		SOCKET clientsock = PlatformSpecifics::getInstance()->accept(serversock);
		if (clientsock < 0 || clientsock == INVALID_SOCKET)
		{
//			WSAGetLastError
			Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"PhoneServer","Error on accept(), trying to re-initialize server.\n");
			DisconnectServer();
			InitServer(serverport);
			ListenServerSocket();
			cout << "Server rebinded, waiting for connection." << endl;
			clientsock = PlatformSpecifics::getInstance()->accept(serversock);
			if (clientsock < 0 || clientsock == INVALID_SOCKET)
			{
				Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"PhoneServer","Error on accept(), 2nd try failed, exiting.\n");
				DisconnectServer();
				exit(1);
			}
		}
		// Convert IP address to string
		Logger::getInstance()->Log(Logger::LOGLEVEL_INFO,"PhoneServer","Connection received from %s\n",
			PlatformSpecifics::getInstance()->GetLastRemoteIp());
		cout << "Connected." << endl;

		// TODO: while socket is not closed by remote size, repeat waiting for commands and execute them...
		bool connectionOpen = true;
		// Use internal PhoneProxy to  handle the data reception
		PhoneProxy::SetSock(clientsock);
		while(connectionOpen)
		{
			// Handle connection

			// Wait for command
			// Receive command
			// TODO: wait for the whole message
			timeMeasurement.start(TimeMeasurementCodeDefs::ReceiveCommand);
			JsonMessage *msg = ReceiveNew();
			timeMeasurement.finish(TimeMeasurementCodeDefs::ReceiveCommand);
			
			if (!msg)
			{
				// The connection was closed from the remote side.
				// (Or a malformed JSON was received...)
				Logger::getInstance()->Log(Logger::LOGLEVEL_ERROR,"PhoneServer","Error on reading from socket. Closing connection.\n");
				connectionOpen=false;
				break;
			}

			// Call respective handler callback
			timeMeasurement.start(TimeMeasurementCodeDefs::HandleMessageCallback);

			//handleJSON(msg, &server);
			JsonMessage *answer = HandleMessage(msg);

			timeMeasurement.finish(TimeMeasurementCodeDefs::HandleMessageCallback);

			// Send and delete answer created by callback

			if (answer)
			{
				timeMeasurement.start(TimeMeasurementCodeDefs::SendingAnswer);
				Send(answer);
				timeMeasurement.finish(TimeMeasurementCodeDefs::SendingAnswer);
				delete answer;
				answer = NULL;
			}
		}

		// Close connection
		Disconnect();
		Logger::getInstance()->Log(Logger::LOGLEVEL_INFO,"PhoneServer","Connection closed\n");
	}

	DisconnectServer();
}

JsonMessage *PhoneServer::HandleMessage(JsonMessage *message)
{
	JsonMessage *response = NULL;
	if (!message)
		return NULL;
	switch(message->getMessageType())
	{
	case Default:	// Not a command, nothing to do.
		break;
	case Ping:
		response = PingCallback((PingMessage *)message);
		break;
	case TakePicture:
		response = TakePictureCallback((TakePictureMessage *)message);
		break;
	case Sendlog:
		response = SendLogCallback((SendlogMessage *)message);
		break;
	case Jpeg:	// Not a command, nothing to do.
		break;
	case MeasurementLog:	// Not a command, nothing to do.
		break;
	case SendPosition:
		response = SendPositionCallback((SendPositionMessage *)message);
		break;
	case Text:
		response = TextCallback((TextMessage *)message);
		break;
	case MatImage:	// Not a command, nothing to do.
		break;
	}
	return response;
}
