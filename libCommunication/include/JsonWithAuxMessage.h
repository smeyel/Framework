#ifndef __JSONWITHAUXMESSAGE_H
#define __JSONWITHAUXMESSAGE_H

#include <iostream>

#include "JsonMessage.h"

/** Generic base class for JSON messages having auxiliary binary data after them.
	Provides an unsigned char vector to store this data and functions to
	read and write it into file, stream or socket.

	Warning: aux binary data is not written by writeJson()!
*/
class JsonWithAuxMessage : public JsonMessage
{
public:
	/** Size of the aux binary data.
	*/
	int size;	// TODO: redundant, use data->size() instead if data!=NULL!

	/** Vector containing the aux binary data
	*/
	std::vector<unsigned char> data;

	/** Constructor
	*/
	JsonWithAuxMessage();

	/** Writes the aux binary data into a stream.
		@param targetStream	The target stream the binary data is written to.
	*/
	virtual void writeAuxStream(std::ostream *targetStream);

	/** Writes the aux binary data into a socket.
		@param socket	The target socket the binary data is written to.
	*/
	virtual void writeAux(int socket);

	/** Writes the aux binary data into a file.
		Internally, calles the writeAuxStream() method, so derived classes do not need to override it.
		@param	filename	Name of the target file.
	*/
	virtual void writeAuxFile(char *filename);	// No need to override, uses writeAuxStream.

	/** Reads the auxiliary data of the message from a socket, if needed.
		Messages may contain auxiliary binary data after the JSON header, like a transferred JPEG image.
		If the message has such binary attachment, this method reads it. Call it after parse().
		
		@warning The size property has to be already set, so that this function knowns the size of the data to read.
		@param	socket	The socket from which the data can be read.
	*/
	virtual void readAuxIfNeeded(int socket);
};
#endif
