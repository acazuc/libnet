#include "Connection.h"

namespace libnet
{

	Connection::Connection(Socket &socket)
	: currentPacket(NULL)
	, socket(socket)
	, rBuffer(1024)
	, wBuffer(1024)
	, crypted(false)
	{
		rBuffer.flip();
	}

	Connection::~Connection()
	{
		delete (currentPacket);
		while (this->packets.size())
		{
			delete (this->packets.front());
			this->packets.pop();
		}
	}

	void Connection::startPacket(uint16_t packet)
	{
		this->currentPacket = new Packet(packet);
	}

	void Connection::endPacket()
	{
		if (!this->currentPacket)
			return;
		if (this->wBuffer.getRemaining() < this->currentPacket->getSize() + 6)
		{
			this->packets.push(this->currentPacket);
			this->currentPacket = NULL;
			return;
		}
		this->wBuffer.writeUInt32(this->currentPacket->getSize() + 2);
		this->wBuffer.writeUInt16(this->currentPacket->getId());
		this->wBuffer.writeBytes(this->currentPacket->getData(), this->currentPacket->getSize());
		delete (this->currentPacket);
		this->currentPacket = NULL;
	}

	bool Connection::initCrypt(const void *key, size_t keylen)
	{
		if (!this->rBuffer.initCrypt(key, keylen))
			return (false);
		if (!this->wBuffer.initCrypt(key, keylen))
			return (false);
		this->crypted = true;
		return (true);
	}

	void Connection::disableCrypt()
	{
		this->crypted = false;
		this->rBuffer.disableCrypt();
		this->wBuffer.disableCrypt();
	}

	int32_t Connection::read()
	{
		return (this->socket.read(this->rBuffer));
	}

	int32_t Connection::send()
	{
		if (this->packets.size())
		{
			Packet *packet = this->packets.front();
			this->packets.pop();
			if (this->wBuffer.getRemaining() >= packet->getSize() + 6)
			{
				this->wBuffer.writeUInt32(packet->getSize() + 2);
				this->wBuffer.writeUInt16(packet->getId());
				this->wBuffer.writeBytes(packet->getData(), packet->getSize());
			}
		}
		return (this->socket.send(this->wBuffer));
	}

	void Connection::writeBytes(const void *data, size_t len)
	{
		this->currentPacket->writeBytes(data, len);
	}

	void Connection::readBytes(void *data, size_t len)
	{
		this->rBuffer.readBytes(data, len);
	}

	void Connection::writeBool(bool value)
	{
		this->currentPacket->writeBool(value);
	}

	bool Connection::readBool()
	{
		return (this->rBuffer.readBool());
	}

	void Connection::writeInt8(int8_t value)
	{
		this->currentPacket->writeInt8(value);
	}

	int8_t Connection::readInt8()
	{
		return (this->rBuffer.readInt8());
	}

	void Connection::writeUInt8(uint8_t value)
	{
		this->currentPacket->writeUInt8(value);
	}

	uint8_t Connection::readUInt8()
	{
		return (this->rBuffer.readUInt8());
	}

	void Connection::writeInt16(int16_t value)
	{
		this->currentPacket->writeInt16(value);
	}

	int16_t Connection::readInt16()
	{
		return (this->rBuffer.readInt16());
	}

	void Connection::writeUInt16(uint16_t value)
	{
		this->currentPacket->writeUInt16(value);
	}

	uint16_t Connection::readUInt16()
	{
		return (this->rBuffer.readUInt16());
	}

	void Connection::writeInt32(int32_t value)
	{
		this->currentPacket->writeInt32(value);
	}

	int32_t Connection::readInt32()
	{
		return (this->rBuffer.readInt32());
	}

	void Connection::writeUInt32(uint32_t value)
	{
		this->currentPacket->writeUInt32(value);
	}

	uint32_t Connection::readUInt32()
	{
		return (this->rBuffer.readUInt32());
	}

	void Connection::writeInt64(int64_t value)
	{
		this->currentPacket->writeInt64(value);
	}

	int64_t Connection::readInt64()
	{
		return (this->rBuffer.readInt64());
	}

	void Connection::writeUInt64(uint64_t value)
	{
		this->currentPacket->writeUInt64(value);
	}

	uint64_t Connection::readUInt64()
	{
		return (this->rBuffer.readUInt64());
	}

	void Connection::writeFloat(float value)
	{
		this->currentPacket->writeFloat(value);
	}

	float Connection::readFloat()
	{
		return (this->rBuffer.readFloat());
	}

	void Connection::writeDouble(double value)
	{
		this->currentPacket->writeDouble(value);
	}

	double Connection::readDouble()
	{
		return (this->rBuffer.readDouble());
	}

	void Connection::writeString(std::string &value)
	{
		this->currentPacket->writeString(value);
	}

	std::string Connection::readString()
	{
		 return (this->rBuffer.readString());
	}

}
