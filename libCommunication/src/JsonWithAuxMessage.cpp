#include <sstream>
#include <vector>

#include "JsonWithAuxMessage.h"
#include "Logger.h"

using namespace std;

JsonWithAuxMessage::JsonWithAuxMessage()
{
	size = 0;
}

void JsonWithAuxMessage::writeAuxStream(std::ostream *targetStream)
{
	targetStream->write((const char*)(data.data()),data.size());
}

void JsonWithAuxMessage::writeAux(int socket)
{
	int n = send(socket, (const char *)(data.data()), data.size(), 0);
	if (n != size)
		LogConfigTime::Logger::getInstance()->Log(LogConfigTime::Logger::LOGLEVEL_ERROR,"libCommunication","JsonWithAuxMessage::writeAux: Error on writing answer to socket.\n");
	return;
}

// Needs "size" to be set
void JsonWithAuxMessage::readAuxIfNeeded(int socket)
{
	if (size<=0) return;

	stringstream ss;
	JsonMessage::receiveIntoStream(&ss,socket,size);

	ss.seekp(0, ios::end);
	stringstream::pos_type jpegSize = ss.tellp();
	ss.seekg(0, ios::beg);
	//cout << "JPEG size:" << jpegSize << endl;

	for(int i=0; i<jpegSize; i++)
	{
		char ch;
		ss.read(&ch,1);
		data.push_back(ch);
	}
}
