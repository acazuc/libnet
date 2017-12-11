#ifndef LIBNET_BUFFER_H
# define LIBNET_BUFFER_H

# include <cstdint>
# include <string>

namespace libnet
{

	class Buffer
	{

		friend class Socket;

		private:
			uint32_t position;
			uint32_t capacity;
			uint32_t limit;
			uint8_t *cryptBox;
			uint8_t *datas;
			uint8_t cryptPos1;
			uint8_t cryptPos2;
			bool crypted;
			uint16_t b_htons(uint16_t value);
			uint32_t b_htonl(uint32_t value);
			uint64_t b_htonll(uint64_t value);
			uint16_t b_ntohs(uint16_t value);
			uint32_t b_ntohl(uint32_t value);
			uint64_t b_ntohll(uint64_t value);
			void crypt(uint32_t position, uint32_t length);

		public:
			Buffer(uint64_t capacity);
			~Buffer();
			bool initCrypt(const void *key, size_t keylen);
			void disableCrypt();
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
			std::string readString();
			void resize(uint64_t len);
			void clear();
			void flip();
			inline uint32_t getRemaining() {return (this->limit - this->position);};
			void setPosition(uint32_t position);
			inline uint32_t getPosition() {return (this->position);};
			inline uint32_t getCapacity() {return (this->capacity);};
			void setLimit(uint32_t limit);
			inline uint32_t getLimit() {return (this->limit);};
			inline uint8_t *getDatas() {return (this->datas);};

	};
}

#endif
