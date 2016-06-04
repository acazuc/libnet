#include "Buffer.h"
#include <stdexcept>
#include <iostream>

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
		delete (this->datas);
	}

	void Buffer::writeBytes(void *src, size_t len)
	{
		for (size_t i = 0;i < len;i++)
		{
			if (this->position >= this->limit)
				throw std::out_of_range("Buffer overflow");
			this->datas[this->position] = ((char*)src)[i];
			this->position++;
		}
	}

	void Buffer::writeBool(bool value)
	{
		int8_t val;

		if (value)
			val = 1;
		else
			val = 0;
		writeBytes(&val, 1);
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
		writeBytes(&value, 2);
	}

	void Buffer::writeUInt16(uint16_t value)
	{
		writeBytes(&value, 2);
	}

	void Buffer::writeInt32(int32_t value)
	{
		writeBytes(&value, 4);
	}

	void Buffer::writeUInt32(uint32_t value)
	{
		writeBytes(&value, 4);
	}

	void Buffer::writeInt64(int64_t value)
	{
		writeBytes(&value, 8);
	}

	void Buffer::writeUInt64(uint64_t value)
	{
		writeBytes(&value, 8);
	}

	void Buffer::writeFloat(float value)
	{
		writeBytes(&value, 4);
	}

	void Buffer::writeDouble(double value)
	{
		writeBytes(&value, 8);
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
		for (size_t i = 0;i < len;i++)
		{
			if (this->position >= this->limit)
				throw std::out_of_range("Buffer overflow");
			((char*)dst)[i] = this->datas[this->position];
			this->position++;
		}
	}

	bool Buffer::readBool()
	{
		int8_t value;

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
