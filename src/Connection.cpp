#include "Connection.h"
#include <iostream>

namespace libnet
{

	Connection::Connection(Socket &socket)
	: socket(socket)
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
		checkAutoSend();
	}

	bool Connection::initCrypt(const void *key, size_t keylen)
	{
		if (!this->rBuffer.initCrypt(key, keylen))
			return (false);
		if (!this->wBuffer.initCrypt(key, keylen))
			return (false);
		return (true);
	}

}
