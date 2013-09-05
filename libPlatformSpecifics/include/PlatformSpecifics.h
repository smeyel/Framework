#ifndef __PLATFORMSPECIFICS_H
#define __PLATFORMSPECIFICS_H

/** Platform specific functions collected in a single singleton class.
	Use the static getInstance() to get the instance pointer
	corresponding to the current platform.
*/
#ifdef __gnu_linux__
#define SD_SEND SHUT_WR

/** On Windows INVALID_SOCKET is used for error checking. There is no
    equivalent of this on GNU/Linux systems. For details see
    http://msdn.microsoft.com/en-us/library/windows/desktop/ms740516%28v=vs.85%29.aspx
 */
#define INVALID_SOCKET -1

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#ifdef WIN32
#include <WinSock2.h>
#endif

class PlatformSpecifics
{
	static PlatformSpecifics *instance;

protected:
	PlatformSpecifics()
	{
	}

public:
	/** Returns singleton instance corresponding to current platform */
	static PlatformSpecifics *getInstance();

	/** Sleeps for given milliseconds */
	virtual void SleepMs(long ms)=0;

	/** Initializes socket system (if necessary).
		This includes application-wide operations necessary
		before creating the first socket.
	*/
	virtual bool InitSocketSystem()=0;

	/** Connects with a socket and returns it. */
	virtual int Connect(const char *ip, int port)=0;


	/** Closes a given socket. */
	virtual void CloseSocket(int socket)=0;

	/** Shuts down the application-wide socket system
		(if necessary).
		The inverse of InitSocketSystem.
	*/
	virtual void ShutdownSocketSystem()=0;

	/** Converts string to long long */
	virtual long long atoll(const char *str)=0;

	/** Socket communication functions
	///@{ */
	/**  */
	virtual int accept(int serversocket)=0;
	/** Returns pointer to string represenation of
		the IP address of the last client connected
		via accept().
	*/
	virtual const char *GetLastRemoteIp()=0;
	/** Create server socket and bind */
	virtual int CreateServerSocket(int port)=0;
	/** Listen server socket */
	virtual void ListenServerSocket(int serversocket)=0;
	/** Send */
	virtual int send(int socket, const char *buff, int size, int flags)=0;
	/** Receive */
	virtual int recv(int socket, char *buff, int size, int flags)=0;
    /** Get last error*/
    virtual int GetLastError() = 0;
	/** ///@{ */
};

#endif
