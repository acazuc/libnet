#ifndef NET_BUFFER_H
# define NET_BUFFER_H

# define BUFF_SIZE 16384

# include <cstdint>
# include <string>

namespace net
{

	class Buffer
	{

		private:
			unsigned int position;
			unsigned int capacity;
			unsigned int limit;
			char *datas;

		public:
			Buffer();
			~Buffer();
			void writeBytes(void *src, size_t len);
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
			inline void clear() {this->limit = this->capacity;this->position = 0;};
			inline void flip() {this->limit = this->position;this->position = 0;};
			inline size_t getRemaining() {return (this->limit - this->position);};
			inline void setPosition(size_t position) {this->position = position;};
			inline size_t getPosition() {return (this->position);};
			inline size_t getCapacity() {return (this->capacity);};
			inline size_t getLimit() {return (this->limit);};
			inline char *getDatas() {return (this->datas);};

	};
}

#endif
