#ifndef LIBNET_BUFFER_H
# define LIBNET_BUFFER_H

# include <cstdint>
# include <string>
# include <vector>

namespace libnet
{

	class Buffer
	{

		friend class Socket;

		private:
			std::vector<uint8_t> cryptBox;
			std::vector<uint8_t> data;
			uint32_t position;
			uint32_t limit;
			uint8_t cryptPos1;
			uint8_t cryptPos2;
			bool crypted;
			uint16_t b_ntohs(const uint16_t value);
			uint32_t b_ntohl(const uint32_t value);
			uint64_t b_ntohll(const uint64_t value);
			void crypt(uint32_t position, uint32_t length);
			void crypt2(uint32_t position, uint32_t length);

		public:
			Buffer(uint64_t capacity);
			bool initCrypt(const void *key, size_t keylen);
			void disableCrypt();
			void writeBytes(const void *src, size_t len);
			void writeBool(const bool value);
			void writeInt8(const int8_t value);
			void writeUInt8(const uint8_t value);
			void writeInt16(const int16_t value);
			void writeUInt16(const uint16_t value);
			void writeInt32(const int32_t value);
			void writeUInt32(const uint32_t value);
			void writeInt64(const int64_t value);
			void writeUInt64(const uint64_t value);
			void writeFloat(const float value);
			void writeDouble(const double value);
			void writeString(const std::string &value);
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
			std::string readString();
			void resize(uint64_t len);
			void clear();
			void flip();
			inline std::vector<uint8_t> &getData() {return this->data;};
			inline uint32_t getRemaining() {return this->limit - this->position;};
			void setPosition(uint32_t position);
			inline uint32_t getPosition() {return this->position;};
			void setLimit(uint32_t limit);
			inline uint32_t getLimit() {return this->limit;};
			inline bool isCrypted() {return this->crypted;};

	};
}

#endif
