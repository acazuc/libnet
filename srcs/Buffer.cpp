#include "Buffer.h"
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
		uint8_t val;

		if (value)
			val = 1;
		else
			val = 0;
		writeBytes(&val, 1);
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
		uint8_t value;

		readBytes(&value, 1);
		return (value != 0);
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
		return (value);
	}

	uint16_t Buffer::readUInt16()
	{
		uint16_t value;

		readBytes(&value, 2);
		return (value);
	}

	int32_t Buffer::readInt32()
	{
		int32_t value;

		readBytes(&value, 4);
		return (value);
	}

	uint32_t Buffer::readUInt32()
	{
		uint32_t value;

		readBytes(&value, 4);
		return (value);
	}

	int64_t Buffer::readInt64()
	{
		int64_t value;

		readBytes(&value, 8);
		return (value);
	}

	uint64_t Buffer::readUInt64()
	{
		uint64_t value;

		readBytes(&value, 8);
		return (value);
	}

	float Buffer::readFloat()
	{
		float value;

		readBytes(&value, 4);
		return (value);
	}

	double Buffer::readDouble()
	{
		double value;

		readBytes(&value, 8);
		return (value);
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
