#ifndef NET_BUFFER_H
# define NET_BUFFER_H

# include <cstdint>
# include <string>

namespace net
{

	class Buffer
	{

		private:
			uint32_t position;
			uint32_t capacity;
			uint32_t limit;
			char *datas;

		public:
			Buffer();
			~Buffer();
			void writeBytes(void *src, size_t len);
			void writeBool(bool value);
			void writeInt8(int8_t value) {writeBytes(&value, 1);};
			void writeUInt8(uint8_t value) {writeBytes(&value, 1);};
			void writeInt16(int16_t value) {writeBytes(&value, 2);};
			void writeUInt16(uint16_t value) {writeBytes(&value, 2);};
			void writeInt32(int32_t value) {writeBytes(&value, 4);};
			void writeUInt32(uint32_t value) {writeBytes(&value, 4);};
			void writeInt64(int64_t value) {writeBytes(&value, 8);};
			void writeUInt64(uint64_t value) {writeBytes(&value, 8);};
			void writeFloat(float value) {writeBytes(&value, 4);};
			void writeDouble(double value) {writeBytes(&value, 8);};
			void writeChar(char value) {writeBytes(&value, 1);};
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
			inline uint32_t getRemaining() {return (this->limit - this->position);};
			inline void setPosition(uint32_t position) {this->position = position;};
			inline uint32_t getPosition() {return (this->position);};
			inline uint32_t getCapacity() {return (this->capacity);};
			inline uint32_t getLimit() {return (this->limit);};
			inline char *getDatas() {return (this->datas);};

	};
}

#endif
