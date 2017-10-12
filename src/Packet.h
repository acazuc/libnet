#ifndef LIBNET_PACKET_H
# define LIBNET_PACKET_H

# include "Buffer.h"
# include <cstdint>
# include <cstring>
# include <string>

namespace libnet
{

	class Packet
	{

	private:
		Buffer buffer;

	public:
		Packet();
		~Packet();
		void writeBytes(const void *src, size_t len);
		void writeBool(bool value);
		void writeInt8(int8_t value);
		void writeUInt8(uint8_t value);
		void writeInt16(int16_t value);
		void writeUInt16(uint16_t value);
		void writeInt32(int32_t value);
		void writeUInt32(uint32_t value);
		void writeInt64(int64_t value);
		void writeUInt64(uint64_t value);
		void writeFloat(float value);
		void writeDouble(double value);
		void writeChar(char value);
		void writeString(std::string &value);
		void readBytes(void *dst, size_t len);
		bool readBool();
		int8_t readInt8();
		uint8_t readUInt8();
		int16_t readInt16();
		uint16_t readUInt16();
		int32_t readInt32();
		uint32_t readUInt32();
		int64_t readInt64();
		uint64_t readUInt64();
		float readFloat();
		double readDouble();
		char readChar();
		std::string readString();
		void resize(uint64_t len);

	};

}

#endif
