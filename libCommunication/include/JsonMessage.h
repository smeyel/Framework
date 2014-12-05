#ifndef __JSONMESSAGE_H
#define __JSONMESSAGE_H

#include <iostream>
#include <string>

#include <memory> // shared_ptr

#include "PlatformSpecifics.h"	// Handles socket-related includes as well
#include "json.h"

#define MAXTYPENAMELENGTH 100

#define DEBUG_JSON_IF_UNKNOWN


namespace Types {
	namespace Action {
		static const char* KEY = "action";

		static const char* INFO = "info";
		static const char* COMMAND = "command";
		static const char* QUERY = "query";
		static const char* CONFIRM = "confirm";
		static const char* ERROR = "error";
	}

	namespace Subject {
		static const char* KEY = "subject";

		static const char* MOVE_TO = "moveTo";
		static const char* OBSTACLE = "obstacle";
		static const char* START = "start";
		static const char* SET_SPEED = "setSpeed";
		static const char* ROBOT_MODEL = "robotModel";
		static const char* CAMERA_IMAGE = "cameraImage";
		static const char* TAKE_PICTURE = "takePicture";
		static const char* PING = "ping";
		static const char* PONG = "pong";
		static const char* LOG = "log";
		static const char* POSITION = "position";
		static const char* MAT_IMAGE = "matimage";
		static const char* TEXT = "text";
		static const char* PROPERTY = "property";
		static const char* TIMESYNC = "timesync";
	}

	namespace Type {
		static const char* KEY = "type";

		static const char* NONE = "none";
		static const char* POINT_2D = "point2d";
		static const char* CONFIG_2D = "config2d";
		static const char* RECT_2D = "rect2d";
		static const char* CONE_3D = "cone3d";
		static const char* POLYGON_2D = "polygon2d";
		static const char* JPEG = "jpeg";
		static const char* MAT = "Mat";
		static const char* TIMESTAMP = "timestamp";
	}

	namespace Misc {
		static const char* KEY_TIMESTAMP = "timestamp";
		static const char* KEY_SIZE = "binarySize";
		static const char* KEY_VALUES = "values";
		static const char* KEY_TEXT = "text";
	}
}


typedef enum _messagetype
{
	Default,
	Ping,
	Pong,
	TakePicture,
	Sendlog,
	Jpeg,
	MeasurementLog,
	SendPosition,
	Position,
	MatImage,
	Text,
	Property,
	Timesync
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

	friend std::ostream& operator<<(std::ostream& o, JsonMessage* msg);

	long long int timestamp;

protected:
	/** Stores the type of the current message.
	*/
	JsonMessageTypeEnum typecode;

	static void receiveIntoStream(std::ostream *targetStream, int sock, long bytenum);

	Json::Value root;

	Json::FastWriter fWriter;
	Json::StyledWriter sWriter;

	virtual void pack();
	virtual void unpack();

public:
	/** Constructor
	*/
	JsonMessage();

	virtual ~JsonMessage() {}

	JsonMessage(Json::Value root);
	JsonMessage(JsonMessageTypeEnum typecode);
	JsonMessage(Json::Value root, JsonMessageTypeEnum typecode);

	long long int getTimestamp() {
		return timestamp;
	}

	void setTimestamp(long long int ts) {
		timestamp = ts;
	}

	std::string toString(bool compact = false);

	//const static int typePingMessage = 0;

	/** Parses a JSON message from a character array.
		@param	json	Pointer to a character buffer storing the JSON content.
		@returns			Returns the parsed JsonMessage object according to the type of the JSON content.
	*/
	static JsonMessage *parse(const char *json);
	static JsonMessage *parse(Json::Value& root);

	/** Returns the type of the message.
		Does not read the possible aux binary data. To read that, call readAuxIfNeeded().
		@returns		Type code of the message.
	*/
	JsonMessageTypeEnum getMessageType();

	/** Writes the message in a JSON format into a character buffer.
		@param	buffer	Pointer to a character buffer where the JSON should be written to.
	*/
	virtual void writeJson(char *buffer);

	/** Intended for debug purposes, shown the structure of the given JSON data.
	*/
	static void DebugJson(const char *json);

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

std::ostream& operator<<(std::ostream& o, JsonMessage* msg);


#endif
