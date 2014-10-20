#ifndef __PROPERTYMESSAGE_H
#define __PROPERTYMESSAGE_H

#include "JsonMessage.h"

#include <string>


/** Property message
	Used to set or get property values (key - value pairs)
*/
class PropertyMessage : public JsonMessage
{
	std::string key;
	std::string value;
public:

	PropertyMessage();
	PropertyMessage(Json::Value root);

	virtual void log();

	virtual void pack();
	virtual void unpack();

	void set(const std::string key, const std::string value);
	void set(const std::string key, const int value);
	void set(const std::string key, const double value);

	std::string getKey() { return key; }

	int getIntValue();
	std::string getStringValue();
	double getDoubleValue();
};

#endif
