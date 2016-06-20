#include "Connection.h"
#include <iostream>

namespace net
{

	Connection::Connection(Socket &sock)
	: socket(sock)
	{
		rBuffer.flip();
	}

	Connection::~Connection()
	{
		//Empty
	}

	void Connection::startPacket()
	{
		this->packetStart = this->wBuffer.getPosition();
		this->wBuffer.writeUInt32(0);
	}

	void Connection::endPacket()
	{
		uint32_t currentPos = this->wBuffer.getPosition();
		this->wBuffer.setPosition(this->packetStart);
		this->wBuffer.writeUInt32(currentPos - this->packetStart - 4);
		this->wBuffer.setPosition(currentPos);
	}

}
