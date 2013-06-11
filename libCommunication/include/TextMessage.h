#ifndef __TEXTMESSAGE_H
#define __TEXTMESSAGE_H

#include "JsonMessage.h"

#define TEXT_MESSAGE_BUFFER_SIZE 100

/** String message
	Used to transfer single-string messages which do not have own JsonMessage
*/
class TextMessage : public JsonMessage
{
public:
	char content[TEXT_MESSAGE_BUFFER_SIZE];
	/** @warning Be careful not to use this constructor to set the content! */
	TextMessage(char *json);
	TextMessage();
	bool parse(char *json);
	virtual void writeJson(char *buffer);
	virtual void log();

	void copyToContent(const char *srcText);
};

#endif
