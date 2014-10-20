#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "PositionMessage.h"
#include "Logger.h"


PositionMessage::PositionMessage() : JsonMessage(Position)
{
}

PositionMessage::PositionMessage(Json::Value root) : JsonMessage(root, Position)
{
}

//bool PositionMessage::parse(char *json)
//{
//	for(int i = 0; i < size; i++) {
//
//		char numstr[21]; // enough to hold all numbers up to 64-bits
//		sprintf(numstr, "%d", i);
//
//
//		std::string markeridFieldName = "markerid";
//		std::string markeridFieldNameNumbered = markeridFieldName + numstr;
//
//		char markeridString[128];
//		JsonMessage::readFieldInto(json,markeridFieldNameNumbered.c_str(),markeridString);
//		int markerid = atoi(markeridString);
//		std::cout << markerid << " ";
//
//
//		std::string posxFieldName = "posx";
//		std::string posxFieldNameNumbered = posxFieldName + numstr;
//
//		char posxString[128];
//		JsonMessage::readFieldInto(json,posxFieldNameNumbered.c_str(),posxString);
//		double posx = atof(posxString);
//		std::cout << posx << " ";
//
//
//		std::string posyFieldName = "posy";
//		std::string posyFieldNameNumbered = posyFieldName + numstr;
//
//		char posyString[128];
//		JsonMessage::readFieldInto(json,posyFieldNameNumbered.c_str(),posyString);
//		double posy = atof(posyString);
//		std::cout << posy << " ";
//
//
//		std::string sizexFieldName = "sizex";
//		std::string sizexFieldNameNumbered = sizexFieldName + numstr;
//
//		char sizexString[128];
//		JsonMessage::readFieldInto(json,sizexFieldNameNumbered.c_str(),sizexString);
//		double sizex = atof(sizexString);
//		std::cout << sizex << " ";
//
//
//		std::string sizeyFieldName = "sizey";
//		std::string sizeyFieldNameNumbered = sizeyFieldName + numstr;
//
//		char sizeyString[128];
//		JsonMessage::readFieldInto(json,sizeyFieldNameNumbered.c_str(),sizeyString);
//		double sizey = atof(sizeyString);
//		std::cout << sizey << " ";
//
//
//		std::string validFieldName = "valid";
//		std::string validFieldNameNumbered = validFieldName + numstr;
//
//		char validString[128];
//		JsonMessage::readFieldInto(json,validFieldNameNumbered.c_str(),validString);
//		int validInt = atof(validString);
//		bool valid = (validInt == 1);
//		std::cout << valid << " ";
//
//
//		std::string centervalidFieldName = "centervalid";
//		std::string centervalidFieldNameNumbered = centervalidFieldName + numstr;
//
//		char centervalidString[128];
//		JsonMessage::readFieldInto(json,centervalidFieldNameNumbered.c_str(),centervalidString);
//		int centervalidInt = atof(centervalidString);
//		bool centervalid = (centervalidInt == 1);
//		std::cout << centervalid << " ";
//	}
//
//	/*char markerDatas[1024];
//	JsonMessage::readFieldInto(json,"trackerdatas",markerDatas);
//
//	std::cout << markerDatas;*/
//
//	return true;
//}


void PositionMessage::log()
{
	LogConfigTime::Logger::getInstance()->Log(
			LogConfigTime::Logger::LOGLEVEL_INFO,
			"Message",
			"PositionMessage( timestamp=%lld )",
			getTimestamp()
	);
}
