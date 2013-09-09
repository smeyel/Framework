#ifndef __PROPERTYMESSAGE_H
#define __PROPERTYMESSAGE_H

#include "JsonMessage.h"

#define PROPERTY_KEY_SIZE 100
#define PROPERTY_VALUE_SIZE 100

/** Property message
	Used to set or get property values (key - value pairs)
*/
class PropertyMessage : public JsonMessage
{
public:
	char key[PROPERTY_KEY_SIZE];
	char value[PROPERTY_VALUE_SIZE];

	/**	Constructor to parse a received JSON message.
		Calles parse() internally.
	@warning Be careful not to use this constructor to set the content! */
	PropertyMessage(char *json);
	/** Constructor to create a new message object.  */
	PropertyMessage();
	bool parse(char *json);
	virtual void writeJson(char *buffer);
	virtual void log();

	void set(const char *srcKey, const char *srcValue);
	void set(const char *srcKey, const int srcValue);
};

#endif
