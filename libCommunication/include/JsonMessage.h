#ifndef __JSONMESSAGE_H
#define __JSONMESSAGE_H

#include <iostream>
#include "PlatformSpecifics.h"	// Handles socket-related includes as well

#define MAXTYPENAMELENGTH 100

#define DEBUG_JSON_IF_UNKNOWN

typedef enum _messagetype
{
	Default,
	Ping,
	TakePicture,
	Sendlog,
	Jpeg,
	MeasurementLog,
	SendPosition,
	Position,
	MatImage,
	Text,
	Property
} JsonMessageTypeEnum;

/** Generic base class JSON based message class used for communication between phones.
	Used among others by PhoneProxy and PhoneServer.
	Applications usually use the derived classes, not this base class.

	If a new derived class is introduced, the following steps are required:
	- Extend JsonMessageTypeEnum with the new type.
	- Extend the JsonMessage.parse() method to recognize the new type.
	- Extend PhoneServer.HandleMessage to call the callback for the message
	- The derived class should not forget to set its typecode
	
	Warning: current implementation requires to close the JSON data with a hashmark (#), like
		{"type": "example", "somedata": "data1"}#
*/
class JsonMessage
{
protected:
	/** Stores the type of the current message.
	*/
	JsonMessageTypeEnum typecode;

	static void receiveIntoStream(std::ostream *targetStream, int sock, long bytenum);

public:
	/** Constructor
	*/
	JsonMessage();

	//const static int typePingMessage = 0;

	/** Parses a JSON message from a character array.
		@param	json	Pointer to a character buffer storing the JSON content.
		@returns			Returns the parsed JsonMessage object according to the type of the JSON content.
	*/
	static JsonMessage *parse(char *json);

	/** Returns the type of the message.
		Does not read the possible aux binary data. To read that, call readAuxIfNeeded().
		@returns		Type code of the message.
	*/
	JsonMessageTypeEnum getMessageType();

	/** Writes the message in a JSON format into a character buffer.
		@param	buffer	Pointer to a character buffer where the JSON should be written to.
	*/
	virtual void writeJson(char *buffer) { }

	/** Intended for debug purposes, shown the structure of the given JSON data.
	*/
	static void DebugJson(char *json);

	/** Helper method, finds a field with given name in the JSON data and copies its
		content into a buffer.

		@param	json			Pointer to the JSON data
		@param	fieldname		The name of the field to look for.
		@param	resultBuffer	Location where the content should be copied.
	*/
	static void readFieldInto(char *json, const char *fieldname, char *resultBuffer);

	/** Writes the content of this message into the log provided by the LogConfigTime Log mechanism.
	*/
	virtual void log();

	/** Writes the aux binary data into a stream.
		@param targetStream	The target stream the binary data is written to.
	*/
	virtual void writeAuxStream(std::ostream *targetStream) { }

	/** Writes the aux binary data into a socket.
		@param targetStream	The target stream the binary data is written to.
	*/
	virtual void writeAux(int socket) { }

	/** Writes the aux binary data into a file.
		Internally, calles the writeAuxStream() method, so derived classes do not need to override it.
		@param	filename	Name of the target file.
	*/
	virtual void writeAuxFile(char *filename) { }	// No need to override, uses writeAuxStream.

	/** Reads the auxiliary data of the message from a socket, if needed.
		Messages may contain auxiliary binary data after the JSON header, like a transferred JPEG image.
		If the message has such binary attachment, this method reads it. Call it after parse().
		
		@warning The size property has to be already set, so that this function knowns the size of the data to read.
		@param	socket	The socket from which the data can be read.
	*/
	virtual void readAuxIfNeeded(int socket) { }

};

#endif
