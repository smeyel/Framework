#ifndef __PLATFORMSPECIFICS_H
#define __PLATFORMSPECIFICS_H


#include <string>

/** Platform specific functions collected in a single singleton class.
	Use the static getInstance() to get the instance pointer
	corresponding to the current platform.
*/
class PlatformSpecifics
{
	static PlatformSpecifics *instance;


protected:
	PlatformSpecifics()	{}
	virtual ~PlatformSpecifics() {}

	std::string lastErrorMessage;

public:
	/** Returns singleton instance corresponding to current platform */
	static PlatformSpecifics *getInstance();

	virtual std::string getLastErrorMessage() { return lastErrorMessage; }

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
	/** ///@{ */
};

#endif
