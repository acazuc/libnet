#include "Buffer.h"
#include "SocketPlatform.h"
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <utf8.h>

#define BUFF_SIZE (16384)

static uint8_t key[] = {165, 61, 151, 185, 74, 69, 202, 40, 128, 22, 150, 71, 213, 14, 136, 139
				, 138, 239, 138, 3, 233, 229, 247, 23, 82, 97, 84, 146, 242, 39, 71, 152
				, 101, 223, 82, 176, 37, 30, 216, 165, 53, 112, 237, 11, 127, 119, 151, 10
				, 103, 34, 13, 81, 9, 6, 104, 91, 103, 189, 237, 90, 228, 54, 243, 74
				, 22, 70, 250, 59, 101, 212, 225, 154, 69, 208, 165, 196, 72, 61, 207, 175
				, 96, 220, 2, 105, 226, 106, 196, 74, 40, 179, 165, 14, 233, 153, 88, 0
				, 224, 84, 59, 70, 41, 30, 224, 110, 238, 135, 52, 55, 196, 4, 231, 38
				, 225, 233, 143, 196, 85, 85, 16, 125, 9, 182, 139, 242, 80, 228, 242, 50
				, 57, 47, 121, 99, 77, 90, 209, 60, 226, 7, 116, 168, 12, 92, 206, 237
				, 71, 94, 178, 156, 180, 195, 27, 189, 122, 167, 176, 203, 140, 164, 253, 198
				, 212, 119, 42, 35, 210, 252, 96, 181, 4, 212, 94, 16, 50, 46, 254, 121
				, 141, 177, 23, 66, 117, 50, 0, 240, 217, 177, 188, 103, 87, 186, 46, 44
				, 51, 89, 79, 6, 86, 175, 188, 91, 133, 28, 108, 184, 74, 107, 51, 215
				, 30, 74, 26, 147, 124, 27, 133, 87, 205, 66, 190, 37, 252, 237, 82, 49
				, 71, 162, 55, 158, 83, 244, 249, 216, 17, 103, 146, 92, 210, 197, 53, 240
				, 16, 80, 133, 141, 107, 11, 229, 57, 77, 164, 95, 75, 147, 177, 124};
static uint8_t keylen = 255;

namespace libnet
{

	Buffer::Buffer()
	{
		this->datas = new uint8_t[BUFF_SIZE];
		this->limit = BUFF_SIZE;
		this->capacity = BUFF_SIZE;
		this->position = 0;
		this->crypt = false;
		this->crypt_box = NULL;
	}

	Buffer::~Buffer()
	{
		delete[] (this->datas);
	}

	uint16_t Buffer::b_htons(uint16_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint8_t high_part = static_cast<uint8_t>(value >> 8);
			const uint8_t low_part = static_cast<uint8_t>(value & 0xFF);
			return ((static_cast<uint16_t>(low_part) << 8) | high_part);
		}
		return (value);
	}

	uint32_t Buffer::b_htonl(uint32_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint16_t high_part = b_htons(static_cast<uint16_t>(value >> 16));
			const uint16_t low_part = b_htons(static_cast<uint16_t>(value & 0xFFFF));
			return ((static_cast<uint32_t>(low_part) << 16) | high_part);
		}
		return (value);
	}

	uint64_t Buffer::b_htonll(uint64_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint32_t high_part = b_htonl(static_cast<uint32_t>(value >> 32));
			const uint32_t low_part = b_htonl(static_cast<uint32_t>(value & 0xFFFFFFFF));
			return ((static_cast<uint64_t>(low_part) << 32) | high_part);
		}
		return (value);
	}

	uint16_t Buffer::b_ntohs(uint16_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint8_t high_part = static_cast<uint8_t>(value >> 8);
			const uint8_t low_part = static_cast<uint8_t>(value & 0xFF);
			return ((static_cast<uint16_t>(low_part) << 8) | high_part);
		}
		return (value);
	}

	uint32_t Buffer::b_ntohl(uint32_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint16_t high_part = b_ntohs(static_cast<uint16_t>(value >> 16));
			const uint16_t low_part = b_ntohs(static_cast<uint16_t>(value & 0xFFFF));
			return ((static_cast<uint32_t>(low_part) << 16) | high_part);
		}
		return (value);
	}

	uint64_t Buffer::b_ntohll(uint64_t value)
	{
		int num = 42;
		if (*reinterpret_cast<const char*>(&num) == num)
		{
			const uint32_t high_part = b_ntohl(static_cast<uint32_t>(value >> 32));
			const uint32_t low_part = b_ntohl(static_cast<uint32_t>(value & 0xFFFFFFFF));
			return ((static_cast<uint64_t>(low_part) << 32) | high_part);
		}
		return (value);
	}

	/*void Buffer::crypt(uint32_t position, uint32_t length)
	{
		uint8_t keyBox[257];
		uint8_t tmp;
		uint32_t i;
		uint32_t j;
		uint32_t k;

		for (i = 0; i < 256; ++i)
			keyBox[i] = (uint8_t)i;
		j = 0;
		for (i = 0; i < 256; ++i)
		{
			j = (j + static_cast<uint64_t>(keyBox[i]) + key[i % keylen]) % 256;
			tmp = keyBox[i];
			keyBox[i] = keyBox[j];
			keyBox[j] = tmp;
		}
		i = 0;
		j = 0;
		for (uint32_t x = 0; x < length; ++x)
		{
			i = (i + 1U) % 256;
			j = (j + static_cast<uint64_t>(keyBox[i])) % 256;
			tmp = keyBox[i];
			keyBox[i] = keyBox[j];
			keyBox[j] = tmp;
			k = (static_cast<uint64_t>(keyBox[i]) + static_cast<uint64_t>(keyBox[j])) % 256;
			this->datas[position + x] = this->datas[position + x] ^ keyBox[k];
		}
	}*/

	bool Buffer::initCrypt(const void *key, size_t keylen)
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
	}

	void Buffer::writeBytes(void *src, size_t len)
	{
		if (this->position + len > this->limit)
			throw std::out_of_range("Buffer overflow (position = " + std::to_string(this->position) + ", limit = " + std::to_string(this->limit) + ")");
		for (size_t i = 0; i < len; i++)
		{
			this->datas[this->position] = ((uint8_t*)src)[i];
			if (this->crypt)
			{
				this->crypt_pos1 = (this->crypt_pos1 + 1U) % 256;
				this->crypt_pos2 = (this->crypt_pos2 + static_cast<uint64_t>(this->crypt_box[this->crypt_pos1])) % 256;
				uint8_t tmp = this->crypt_box[this->crypt_pos1];
				this->crypt_box[this->crypt_pos1] = this->crypt_box[this->crypt_pos2];
				this->crypt_box[this->crypt_pos2] = tmp;
				uint8_t k = (static_cast<uint64_t>(this->crypt_box[this->crypt_pos1]) + static_cast<uint64_t>(this->crypt_box[this->crypt_pos2])) % 256;
				this->datas[this->position] = this->datas[this->position] ^ this->crypt_box[k];
			}
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
		uint16_t val = b_htons(static_cast<uint16_t>(value));
		writeBytes(&val, 2);

	}
	void Buffer::writeUInt16(uint16_t value)
	{
		uint16_t val = b_htons(value);
		writeBytes(&val, 2);
	}

	void Buffer::writeInt32(int32_t value)
	{
		uint32_t val = b_htonl(static_cast<uint32_t>(value));
		writeBytes(&val, 4);
	}

	void Buffer::writeUInt32(uint32_t value)
	{
		uint32_t val = b_htonl(value);
		writeBytes(&val, 4);
	}

	void Buffer::writeInt64(int64_t value)
	{
		uint64_t val = b_htonll(static_cast<uint64_t>(value));
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
		for (uint16_t i = 0; i < value.length(); ++i)
		{
			writeChar(value[i]);
		}
	}

	void Buffer::readBytes(void *dst, size_t len)
	{
		if (this->position + len > this->limit)
			throw std::out_of_range("Buffer underflow (position = " + std::to_string(this->position) + ", limit = " + std::to_string(this->limit) + ")");
		for (size_t i = 0; i < len; ++i)
		{
			if (this->crypt)
			{
				this->crypt_pos1 = (this->crypt_pos1 + 1U) % 256;
				this->crypt_pos2 = (this->crypt_pos2 + static_cast<uint64_t>(this->crypt_box[this->crypt_pos1])) % 256;
				uint8_t tmp = this->crypt_box[this->crypt_pos1];
				this->crypt_box[this->crypt_pos1] = this->crypt_box[this->crypt_pos2];
				this->crypt_box[this->crypt_pos2] = tmp;
				uint8_t k = (static_cast<uint64_t>(this->crypt_box[this->crypt_pos1]) + static_cast<uint64_t>(this->crypt_box[this->crypt_pos2])) % 256;
				this->datas[this->position] = this->datas[this->position] ^ this->crypt_box[k];
			}
			((uint8_t*)dst)[i] = this->datas[this->position];
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
		return (static_cast<int16_t>(b_ntohs(static_cast<uint16_t>(value))));
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
		return (static_cast<int32_t>(b_ntohl(static_cast<uint32_t>(value))));
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
		return (static_cast<int64_t>(b_ntohll(static_cast<uint64_t>(value))));
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
		while (--length >= 0)
			str += readChar();
		if (!utf8::is_valid(str.begin(), str.end()))
		{
			std::string tmp;
			utf8::replace_invalid(str.begin(), str.end(), back_inserter(tmp));
			str = tmp;
		}
		return (str);
	}

}
