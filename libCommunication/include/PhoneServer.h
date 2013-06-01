#ifndef __PHONESERVER_H
#define __PHONESERVER_H

#ifdef WIN32
#include <winsock2.h>
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "PhoneProxy.h"
#include "JsonMessage.h"

// Used for the callbacks
#include "PingMessage.h"
#include "TakePictureMessage.h"
#include "SendPositionMessage.h"
#include "SendLogMessage.h"

using namespace std;

#define RCVBUFSIZE 8192//32768//16384//8192

/** Server functions for a phone which can be connected by PhoneProxy.
	Usage:
	- Use InitServer() and ListenServerSocket() to start the server.
	- Currently, server socket handling has to be implemented externally.
		After receiving the connection, use SetSock().
		Use GetServerSocket() to get the server socket for accept().
	- Use Send() and ReceiveNew() to communicate with the remote side.
	- Use DisconnectServer() to close the server.

	If you use a http based registry of nodes, you can use the RegisterNode()
	method to send the HTTP message that registers the node.
*/
class PhoneServer : public PhoneProxy
{
private:
	/** Helper method to init the OS socket subsystem. */
	int InitSocket();
	/** Helper method to close sockets. */
	void CloseSocket(SOCKET socket);

	SOCKET serversock;
	int serverport;

	// !!! TimeMeasurement is derived from PhoneProxy!
	// Derived classes may use measurement ID-s >20
	class TimeMeasurementCodeDefs
	{
	public:
		// This class may use ID-s 101-200.
		const static int ReceiveCommand			= 11;
		const static int HandleMessageCallback	= 12;
		const static int SendingAnswer			= 13;

		static void setnames(TimeMeasurement *measurement)
		{
			measurement->setMeasurementName("PhoneServer internal time measurements");

			measurement->setname(ReceiveCommand,"PhoneServer-ReceiveCommand");
			measurement->setname(HandleMessageCallback,"PhoneServer-HandleMessageCallback");
			measurement->setname(SendingAnswer,"PhoneServer-SendingAnswer");
		}
	};

public:
	/** Message handling callbacks to override.
		If the handler function returns a valid pointer (not NULL),
		that message is sent as response. If NULL, no answer is sent.
		Callbacks should create the new response message in heap. The communication
		loop will delete that.
	///@{ */
	/** Ping callback */
	virtual JsonMessage *PingCallback(PingMessage *pingMessage) { return NULL; }

	/** TakePicture callback */
	virtual JsonMessage *TakePictureCallback(TakePictureMessage *takePictureMessage) { return NULL; }

	/** SendPosition callback */
	virtual JsonMessage *SendPositionCallback(SendPositionMessage *sendPositionMessage) { return NULL; }

	/** SendLog callback */
	virtual JsonMessage *SendLogCallback(SendlogMessage *sendlogMessage) { return NULL; }
	/** ///@} */

	/** Constructor */
	PhoneServer() : PhoneProxy()
	{
		lastReceivedTimeStamp=0;
		serverport = -1;
		TimeMeasurementCodeDefs::setnames(&timeMeasurement);
	}

	/** Returns the server socket. */
/*	SOCKET GetServerSocket()
	{
		return serversock;
	}*/

	/** Initializes the server listening on a given port.
		The server socket is created, but listening is not started.
		Use ListenServerSocket() to start listening.

		@param port	The TCP port to listen to.
		@returns	Zero for success, <0 for error codes.
	*/
	int InitServer(int port);

	/** Registers the server on a SMEyeL node registry server.
		(Do not have to call InitServer before this. This only registers the servers presence.)
		The registrationURL is sent to the server defined by registryHost on port 80 as a HTTP GET request.

		@param	registryHost	Address (IP or URL) of the server, like "avalon.aut.bme.hu".
		@param	registrationURL	Path on the server for the registration script, like
								"~kristof/smeyel/smeyel_reg.php?IP=127.0.0.1:6000". (Without starting "/".)
		@return				0 for success, <0 for error codes.
	*/
	int RegisterNode(const char *registryHost, const char *registrationURL);

	/** Set by Run() true. If set false externally, main loop in Run() stops after handling
		the message in progress (if any). */
	bool mainLoopRunning;

	/** Main communication loop.
		Receives commands, calls callbacks and sends answers if any.
		@see mainLoopRunning on how to terminate it.
	*/
	void Run();

private:
	JsonMessage *HandleMessage(JsonMessage *message);

	/** Starts listening on the server socket port. */
	void ListenServerSocket();

	/** Closes the server socket. */
	void DisconnectServer();

	/** After receiving a connection via the server socket,
		use this method to set the client socket so that you can use
		the Send() and ReceiveNew() methods this class derives from PhoneProxy.
	*/
	void SetSock(SOCKET newsock)
	{
		PhoneProxy::SetSock(newsock);
	}



};

#endif
