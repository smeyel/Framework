#ifndef __JSONWITHAUXMESSAGE_H
#define __JSONWITHAUXMESSAGE_H

#include <iostream>

#include "JsonMessage.h"

class JsonWithAuxMessage : public JsonMessage
{
public:
	int size;	// TODO: redundant, use data->size() instead if data!=NULL!
	std::vector<unsigned char> data;

	JsonWithAuxMessage();

	virtual void writeAuxStream(std::ostream *targetStream);
	virtual void writeAux(int socket);
	virtual void readAuxIfNeeded(int socket);
};

#endif



