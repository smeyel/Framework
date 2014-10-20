#ifndef __TEXTMESSAGE_H
#define __TEXTMESSAGE_H

#include "JsonMessage.h"

#include <string>


/** String message
	Used to transfer single-string messages which do not have own JsonMessage
*/
class TextMessage : public JsonMessage
{
	std::string content;
public:
	TextMessage();
	TextMessage(Json::Value root);

	virtual void log();

	virtual void pack();
	virtual void unpack();

	void setContent(const std::string& content);
	std::string getContent() const { return content; }

};

#endif
