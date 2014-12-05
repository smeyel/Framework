/*
 * TimesyncRequestMessage.h
 *
 *  Created on: Dec 4, 2014
 *      Author: akos
 */

#ifndef TIMESYNCREQUESTMESSAGE_H_
#define TIMESYNCREQUESTMESSAGE_H_

#include "JsonMessage.h"

class TimesyncRequestMessage: public JsonMessage {
public:
	TimesyncRequestMessage();
	TimesyncRequestMessage(Json::Value root);
	virtual ~TimesyncRequestMessage();
};

#endif /* TIMESYNCREQUESTMESSAGE_H_ */
