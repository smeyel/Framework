#ifndef __PHONEPROXY_H
#define __PHONEPROXY_H
#ifdef WIN32
	#include <winsock2.h>
#else
	#define SOCKET int
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "JsonMessage.h"
#include "TimeMeasurement.h"

using namespace std;
using namespace LogConfigTime;

#define RCVBUFSIZE 8192//32768//16384//8192

/** Proxy class for a remote smartphone.
	Wraps all connection related functions.

	To use,
	- call Connect(ip,port)
	- instantiate JsonMessage objects and send them with Send()
	- use ReceiveNew() to wait and receive responses.
		After processing the response, do not forget to delete the response.
	- call Disconnect() to close the communication.
*/
class PhoneProxy
{
private:
	SOCKET sock;

	class TimeMeasurementCodeDefs
	{
	public:
		const static int ReceiveNew		= 1;
		const static int ReceiveNew_WaitAndReceiveJson	= 2;
		const static int ReceiveNew_ParseJson	= 3;
		const static int ReceiveNew_ReceiveAux	= 4;

		static void setnames(TimeMeasurement *measurement)
		{
			measurement->setMeasurementName("PhoneProxy internal time measurements");

			// "PhoneProxy" prefix is used to separate results from the ones of derived classes
			//	using the TimeMeasurement object derived from PhoneProxy.
			measurement->setname(ReceiveNew,"PhoneProxy-ReceiveNew");
			measurement->setname(ReceiveNew_WaitAndReceiveJson,"PhoneProxy-ReceiveNew_WaitAndReceiveJson");
			measurement->setname(ReceiveNew_ParseJson,"PhoneProxy-ReceiveNew_ParseJson");
			measurement->setname(ReceiveNew_ReceiveAux,"PhoneProxy-ReceiveNew_ReceiveAux");
		}
	};

	void receiveIntoStream(ostream *targetStream, SOCKET sock, long bytenum);	// Deprecated, handled by JsonMessage...

public:
	/** Time measurement object used for measuring the time requirements of the internal procedures.
		May be accessed to monitor the time consumption.
		Derived classes may use measurement ID-s >100
	*/
	TimeMeasurement timeMeasurement;

	long long lastReceivedTimeStamp;	// Used to query the timestamp of the last reception

	PhoneProxy()
	{
		lastReceivedTimeStamp=0;
		// Prepare time measurement
		timeMeasurement.init();
		TimeMeasurementCodeDefs::setnames(&timeMeasurement);
	}

	/** Connect the proxy to a phone via TCP/IP.
		@param	ip		IPv4 address of the phone as a string, like "127.0.0.1".
		@param	port	TCP port the phone is listening at.
	*/
	void Connect(const char *ip, int port);

	/** Disconnects the connection to the phone. */
	void Disconnect();

	// Wrappers for Send()
	/** Wraps Send() for TakePicture command
		@param desiredTimeStamp	Desired timestamp the picture should be taken.
	*/
	void RequestPhoto(long long desiredTimeStamp);
	/** Wraps Send() for SendPosition command */
	void RequestPosition();
	/** Wraps Send() for Ping command */
	void RequestPing();
	/** Wraps Send() for Sendlog command */
	void RequestLog();
	
	// Deprecated functions
	/** @deprecated Use ReceiveNew() instead! */
	void Receive(char *filename);	// Deprecated, use ReceiveNew()
	/** @deprecated Use ReceiveNew() instead! */
	void Receive(ostream *targetStream);	// Deprecated, use ReceiveNew()
	/** @deprecated Use ReceiveNew() instead! */
	void ReceiveJpeg(cv::Mat *targetMat);	// Deprecated, use ReceiveNew()
	/** @deprecated Use ReceiveNew() instead! */
	void ReceiveDebug();	// Deprecated, use ReceiveNew()

	// New interface
	/** Sends a given JsonMessage object to the phone.
		If it contains aux binary data, that is sent as well.
		@param	msg	JsonMessage to be sent.
	*/
	void Send(JsonMessage *msg);

	/** Receives a new message. Blocks until the reception is completed.
		@returns	Pointer to the received JsonMessage.

		@warning The new message is created on heap. Do not forget to delete it after using.
	*/
	JsonMessage *ReceiveNew();

protected:
	/** Used by the derived class PhoneServer to set the socket externally after the server
		socket opens a new client socket.
		@param	newsock	The socket to be used for the communication. (Not the server socket.)
	*/
	void SetSock(SOCKET newsock)
	{
		// Used on server side to define connection socket after accept()
		sock = newsock;
	}

private:
	/**	Processes an incoming JSON message.
		@deprecated These functions are handled by JsonMessage. Left for compatibility.
	*/
	void ProcessIncomingJSON(int sock,char *buffer, ostream *targetStream);	// Deprecated, handled by JsonMessage...
};

static void error_exit(char *errorMessage);

#endif /*__PHONEPROXY_H*/
