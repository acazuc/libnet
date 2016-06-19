#include "Buffer.h"
#include "SocketPlatform.h"
#include <stdexcept>
#include <iostream>

#define BUFF_SIZE 16384

namespace net
{

	Buffer::Buffer()
	{
		this->datas = new char[BUFF_SIZE];
		this->limit = BUFF_SIZE;
		this->capacity = BUFF_SIZE;
		this->position = 0;
	}

	Buffer::~Buffer()
	{
		delete[] (this->datas);
	}

	uint16_t Buffer::htons(uint16_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint8_t high_part = static_cast<uint8_t>(value >> 8);
			const uint8_t low_part = static_cast<uint8_t>(value & 0xFF);
			return ((static_cast<uint16_t>(low_part) << 8) | high_part);
		}
		else
		{
			return (value);
		}
	}

	uint32_t Buffer::htonl(uint32_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint16_t high_part = htons(static_cast<uint16_t>(value >> 16));
			const uint16_t low_part = htons(static_cast<uint16_t>(value & 0xFFFF));
			return ((static_cast<uint32_t>(low_part) << 16) | high_part);
		}
		else
		{
			return (value);
		}
	}

	uint64_t Buffer::htonll(uint64_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
			const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFF));
			return ((static_cast<uint64_t>(low_part) << 32) | high_part);
		}
		else
		{
			return (value);
		}
	}

	uint16_t Buffer::ntohs(uint16_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint8_t high_part = static_cast<uint8_t>(value >> 8);
			const uint8_t low_part = static_cast<uint8_t>(value & 0xFF);
			return ((static_cast<uint16_t>(low_part) << 8) | high_part);
		}
		else
		{
			return (value);
		}
	}

	uint32_t Buffer::ntohl(uint32_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint16_t high_part = ntohs(static_cast<uint16_t>(value >> 16));
			const uint16_t low_part = ntohs(static_cast<uint16_t>(value & 0xFFFF));
			return ((static_cast<uint32_t>(low_part) << 16) | high_part);
		}
		else
		{
			return (value);
		}
	}

	uint64_t Buffer::ntohll(uint64_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint32_t high_part = ntohl(static_cast<uint32_t>(value >> 32));
			const uint32_t low_part = ntohl(static_cast<uint32_t>(value & 0xFFFFFFFF));
			return ((static_cast<uint64_t>(low_part) << 32) | high_part);
		}
		else
		{
			return (value);
		}
	}

	void Buffer::writeBytes(void *src, size_t len)
	{
		if (this->position + len > this->limit)
			throw std::out_of_range("Buffer overflow");
		for (size_t i = 0;i < len;i++)
		{
			this->datas[this->position] = ((char*)src)[i];
			this->position++;
		}
	}

	void Buffer::writeBool(bool value)
	{
		writeUInt8(value ? 1 : 0);
	}

	void Buffer::writeInt8(int8_t value)
	{
		writeBytes(&value, 1);
	}

	void Buffer::writeUInt8(uint8_t value)
	{
		writeBytes(&value, 1);
	}

	void Buffer::writeInt16(int16_t value)
	{
		uint16_t val = htons(static_cast<uint16_t>(value));
		writeBytes(&val, 2);

	}
	void Buffer::writeUInt16(uint16_t value)
	{
		uint16_t val = htons(value);
		writeBytes(&val, 2);
	}

	void Buffer::writeInt32(int32_t value)
	{
		uint32_t val = htonl(static_cast<uint32_t>(value));
		writeBytes(&val, 4);
	}

	void Buffer::writeUInt32(uint32_t value)
	{
		uint32_t val = htonl(value);
		writeBytes(&val, 4);
	}

	void Buffer::writeInt64(int64_t value)
	{
		uint64_t val = htonll(static_cast<uint64_t>(value));
		writeBytes(&val, 8);
	}

	void Buffer::writeUInt64(uint64_t value)
	{
		uint64_t val = htonll(value);
		writeBytes(&val, 8);
	}

	void Buffer::writeFloat(float value)
	{
		uint32_t val = htonl(static_cast<uint32_t>(value));
		writeBytes(&val, 4);
	}

	void Buffer::writeDouble(double value)
	{
		uint64_t val = htonll(static_cast<uint64_t>(value));
		writeBytes(&val, 8);
	}

	void Buffer::writeChar(char value)
	{
		writeBytes(&value, 1);
	}

	void Buffer::writeString(std::string &value)
	{
		int i;

		writeUInt16(value.length());
		i = 0;
		while (i < value.length())
		{
			writeChar(value[i]);
			i++;
		}
	}

	void Buffer::readBytes(void *dst, size_t len)
	{
		if (this->position + len > this->limit)
			throw std::out_of_range("Buffer overflow");
		for (size_t i = 0;i < len;i++)
		{
			((char*)dst)[i] = this->datas[this->position];
			this->position++;
		}
	}

	bool Buffer::readBool()
	{
		return (readUInt8() != 0);
	}

	int8_t Buffer::readInt8()
	{
		int8_t value;

		readBytes(&value, 1);
		return (value);
	}

	uint8_t Buffer::readUInt8()
	{
		uint8_t value;

		readBytes(&value, 1);
		return (value);
	}

	int16_t Buffer::readInt16()
	{
		int16_t value;

		readBytes(&value, 2);
		return (static_cast<int16_t>(ntohs(static_cast<uint16_t>(value))));
	}

	uint16_t Buffer::readUInt16()
	{
		uint16_t value;

		readBytes(&value, 2);
		return (ntohs(value));
	}

	int32_t Buffer::readInt32()
	{
		int32_t value;

		readBytes(&value, 4);
		return (static_cast<int32_t>(ntohl(static_cast<uint32_t>(value))));
	}

	uint32_t Buffer::readUInt32()
	{
		uint32_t value;

		readBytes(&value, 4);
		return (ntohl(value));
	}

	int64_t Buffer::readInt64()
	{
		int64_t value;

		readBytes(&value, 8);
		return (static_cast<int64_t>(ntohll(static_cast<uint64_t>(value))));
	}

	uint64_t Buffer::readUInt64()
	{
		uint64_t value;

		readBytes(&value, 8);
		return (ntohll(value));
	}

	float Buffer::readFloat()
	{
		float value;

		readBytes(&value, 4);
		return (static_cast<float>(ntohl(static_cast<uint32_t>(value))));
	}

	double Buffer::readDouble()
	{
		double value;

		readBytes(&value, 8);
		return (static_cast<double>(ntohl(static_cast<uint64_t>(value))));
	}

	char Buffer::readChar()
	{
		char value;

		readBytes(&value, 1);
		return (value);
	}

	std::string Buffer::readString()
	{
		std::string str;
		int16_t length;

		length = readUInt16();
		while (--length >= 0)
			str += readChar();
		return (str);
	}

}
