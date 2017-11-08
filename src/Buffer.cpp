#include "Buffer.h"
#include "SocketPlatform.h"
#include <stdexcept>
#include <cstring>
#include <utf8.h>

namespace libnet
{

	Buffer::Buffer(uint64_t capacity)
	{
		this->cryptedDatas = NULL;
		this->datas = new uint8_t[capacity];
		this->limit = capacity;
		this->capacity = capacity;
		this->position = 0;
	}

	Buffer::~Buffer()
	{
		delete[] (this->datas);
	}

	uint16_t Buffer::b_htons(uint16_t value)
	{
		return (b_ntohs(value));
	}

	uint32_t Buffer::b_htonl(uint32_t value)
	{
		return (b_ntohl(value));
	}

	uint64_t Buffer::b_htonll(uint64_t value)
	{
		return (b_ntohll(value));
	}

	uint16_t Buffer::b_ntohs(uint16_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint16_t highPart = (value >> 8) & 0xFF;
			const uint16_t lowPart = value & 0xFF;
			return ((lowPart << 8) | highPart);
		}
		return (value);
	}

	uint32_t Buffer::b_ntohl(uint32_t value)
	{
		int32_t num = 42;
		if (*reinterpret_cast<char*>(&num) == num)
		{
			const uint16_t highVal = (value >> 16) & 0xFFFF;
			const uint16_t lowVal = value & 0xFFFF;
			const uint32_t highPart = b_ntohs(*reinterpret_cast<const uint16_t*>(&highVal));
			const uint32_t lowPart = b_ntohs(*reinterpret_cast<const uint16_t*>(&lowVal));
			return ((lowPart << 16) | highPart);
		}
		return (value);
	}

	uint64_t Buffer::b_ntohll(uint64_t value)
	{
		int32_t num = 42;
		if (*reinterpret_cast<char*>(&num) == num)
		{
			const uint32_t highVal = (value >> 32) & 0xFFFFFFFF;
			const uint32_t lowVal = value & 0xFFFFFFFF;
			const uint64_t highPart = b_ntohl(*reinterpret_cast<const uint32_t*>(&highVal));
			const uint64_t lowPart = b_ntohl(*reinterpret_cast<const uint32_t*>(&lowVal));
			return ((lowPart << 32) | highPart);
		}
		return (value);
	}

	/*void Buffer::crypt(uint32_t position, uint32_t length)
	{
		uint8_t tmp;
		uint32_t i;
		uint32_t j;
		uint32_t k;

		i = 0;
		j = 0;
		for (uint32_t x = 0; x < length; ++x)
		{
			i = (i + 1U) % 256;
			j = (j + this->crypt_box[i]) % 256;
			tmp = this->crypt_box[i];
			this->crypt_box[i] = this->crypt_box[j];
			this->crypt_box[j] = tmp;
			k = (this->crypt_box[i] + this->crypt_box[j]) % 256;
			this->datas[position + x] = this->datas[position + x] ^ this->crypt_box[k];
		}
	}*/

	/*bool Buffer::initCrypt(const void *key, size_t keylen)
	{
		if (keylen == 0)
			return (false);
		this->crypt = true;
		this->crypt_pos1 = 0;
		this->crypt_pos2 = 0;
		this->crypt_box = new uint8_t[256];
		for (uint16_t i = 0; i < 256; ++i)
			this->crypt_box[i] = (uint8_t)i;
		uint16_t j = 0;
		for (uint16_t i = 0; i < 256; ++i)
		{
			j = (j + static_cast<uint64_t>(this->crypt_box[i]) + ((uint8_t*)key)[i % keylen]) % 256;
			uint8_t tmp = this->crypt_box[i];
			this->crypt_box[i] = this->crypt_box[j];
			this->crypt_box[j] = tmp;
		}
		return (true);
	}*/

	void Buffer::writeBytes(const void *src, size_t len)
	{
		if (this->position + len > this->limit)
			throw std::out_of_range("Buffer overflow (position = " + std::to_string(this->position) + ", limit = " + std::to_string(this->limit) + ", len = " + std::to_string(len) + ")");
		std::memmove(&this->datas[this->position], src, len);
		setPosition(this->position + len);
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
		uint16_t val = b_htons(*reinterpret_cast<uint16_t*>(&value));
		writeBytes(&val, 2);

	}
	void Buffer::writeUInt16(uint16_t value)
	{
		uint16_t val = b_htons(value);
		writeBytes(&val, 2);
	}

	void Buffer::writeInt32(int32_t value)
	{
		uint32_t val = b_htonl(*reinterpret_cast<uint32_t*>(&value));
		writeBytes(&val, 4);
	}

	void Buffer::writeUInt32(uint32_t value)
	{
		uint32_t val = b_htonl(value);
		writeBytes(&val, 4);
	}

	void Buffer::writeInt64(int64_t value)
	{
		uint64_t val = b_htonll(*reinterpret_cast<uint64_t*>(&value));
		writeBytes(&val, 8);
	}

	void Buffer::writeUInt64(uint64_t value)
	{
		uint64_t val = b_htonll(value);
		writeBytes(&val, 8);
	}

	void Buffer::writeFloat(float value)
	{
		uint32_t val = b_htonl(*reinterpret_cast<uint32_t*>(&value));
		writeBytes(&val, 4);
	}

	void Buffer::writeDouble(double value)
	{
		uint64_t val = b_htonll(*reinterpret_cast<uint64_t*>(&value));
		writeBytes(&val, 8);
	}

	void Buffer::writeChar(char value)
	{
		writeBytes(&value, 1);
	}

	void Buffer::writeString(std::string &value)
	{
		writeUInt16(value.length());
		writeBytes(value.c_str(), value.length());
	}

	void Buffer::readBytes(void *dst, size_t len)
	{
		if (this->position + len > this->limit)
			throw std::out_of_range("Buffer underflow (position = " + std::to_string(this->position) + ", limit = " + std::to_string(this->limit) + ", len = " + std::to_string(len) + ")");
		std::memmove(dst, &this->datas[this->position], len);
		setPosition(this->position + len);
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
		uint16_t tmp = b_ntohs(*reinterpret_cast<uint16_t*>(&value));
		return (*reinterpret_cast<int16_t*>(&tmp));
	}

	uint16_t Buffer::readUInt16()
	{
		uint16_t value;
		readBytes(&value, 2);
		return (b_ntohs(value));
	}

	int32_t Buffer::readInt32()
	{
		int32_t value;
		readBytes(&value, 4);
		uint32_t tmp = b_ntohl(*reinterpret_cast<uint32_t*>(&value));
		return (*reinterpret_cast<int32_t*>(&tmp));
	}

	uint32_t Buffer::readUInt32()
	{
		uint32_t value;
		readBytes(&value, 4);
		return (b_ntohl(value));
	}

	int64_t Buffer::readInt64()
	{
		int64_t value;
		readBytes(&value, 8);
		uint64_t tmp = b_ntohll(*reinterpret_cast<uint64_t*>(&value));
		return (*reinterpret_cast<int64_t*>(&tmp));
	}

	uint64_t Buffer::readUInt64()
	{
		uint64_t value;
		readBytes(&value, 8);
		return (b_ntohll(value));
	}

	float Buffer::readFloat()
	{
		float value;
		readBytes(&value, 4);
		uint32_t tmp = b_ntohl(*reinterpret_cast<uint32_t*>(&value));
		return (*reinterpret_cast<float*>(&tmp));
	}

	double Buffer::readDouble()
	{
		double value;
		readBytes(&value, 8);
		uint64_t tmp = b_ntohl(*reinterpret_cast<uint64_t*>(&value));
		return (*reinterpret_cast<double*>(&tmp));
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
		int16_t length = readUInt16();
		str.resize(length);
		readBytes(const_cast<char*>(str.c_str()), length);
		{
			std::string tmp;
			utf8::replace_invalid(str.begin(), str.end(), back_inserter(tmp));
			str = tmp;
		}
		return (str);
	}

	void Buffer::resize(uint64_t len)
	{
		if (len == 0)
			len = 1;
		uint8_t *newDatas = new uint8_t[len];
		std::memmove(newDatas, this->datas, std::min(static_cast<uint32_t>(len), this->limit));
		delete[] (this->datas);
		if (this->limit >= len || this->limit == this->capacity)
		{
			setLimit(len);
			if (this->position > this->limit)
			{
				setPosition(this->limit);
			}
		}
		this->capacity = len;
		this->datas = newDatas;
	}

	void Buffer::setPosition(uint32_t position)
	{
		this->position = position;
	}

	void Buffer::setLimit(uint32_t limit)
	{
		this->limit = limit;
	}

	void Buffer::clear()
	{
		setLimit(this->capacity);
		setPosition(0);
	}

	void Buffer::flip()
	{
		setLimit(this->position);
		setPosition(0);
	}

}
