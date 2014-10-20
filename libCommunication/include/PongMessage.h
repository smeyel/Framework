/*
 * PongMessage.h
 *
 *  Created on: Oct 19, 2014
 *      Author: akos
 */

#ifndef PONGMESSAGE_H_
#define PONGMESSAGE_H_

#include "JsonMessage.h"

class PongMessage : public JsonMessage {
public:
	PongMessage();
	PongMessage(Json::Value root);

	virtual ~PongMessage() {}

	virtual void log();
};


#endif /* PONGMESSAGE_H_ */
