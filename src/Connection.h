#ifndef LIBNET_CONNECTION_H
# define LIBNET_CONNECTION_H

# include "Buffer.h"
# include "Socket.h"
# include <cstdint>
# include <string>

namespace libnet
{

	class Connection
	{

		private:
			Socket &socket;
			Buffer rBuffer;
			Buffer wBuffer;
			uint32_t packetStart;
			inline void checkAutoSend() {if (wBuffer.getPosition() >= wBuffer.getLimit() * 3 / 4){send();}};

		public:
			Connection(Socket &socket);
			~Connection();
			void startPacket();
			void endPacket();
			bool initCrypt(const void *key, size_t keylen);
			inline int32_t read() {return (this->socket.read(rBuffer));};
			inline int32_t send() {return (this->socket.send(wBuffer));};
			inline void writeBytes(const void *data, size_t len) {this->wBuffer.writeBytes(data, len);};
			inline void readBytes(void *data, size_t len) {this->rBuffer.readBytes(data, len);};
			inline void writeBool(bool value) {this->wBuffer.writeBool(value);};
			inline bool readBool() {return (this->rBuffer.readBool());};
			inline void writeInt8(int8_t value) {this->wBuffer.writeInt8(value);};
			inline int8_t readInt8() {return (this->rBuffer.readInt8());};
			inline void writeUInt8(uint8_t value) {this->wBuffer.writeUInt8(value);};
			inline uint8_t readUInt8() {return (this->rBuffer.readUInt8());};
			inline void writeInt16(int16_t value) {this->wBuffer.writeInt16(value);};
			inline int16_t readInt16() {return (this->rBuffer.readInt16());};
			inline void writeUInt16(uint16_t value) {this->wBuffer.writeUInt16(value);};
			inline uint16_t readUInt16() {return (this->rBuffer.readUInt16());};
			inline void writeInt32(int32_t value) {this->wBuffer.writeInt32(value);};
			inline int32_t readInt32() {return (this->rBuffer.readInt32());};
			inline void writeUInt32(uint32_t value) {this->wBuffer.writeUInt32(value);};
			inline uint32_t readUInt32() {return (this->rBuffer.readUInt32());};
			inline void writeInt64(int64_t value) {this->wBuffer.writeInt64(value);};
			inline int64_t readInt64() {return (this->rBuffer.readInt64());};
			inline void writeUInt64(uint64_t value) {this->wBuffer.writeUInt64(value);};
			inline uint64_t readUInt64() {return (this->rBuffer.readUInt64());};
			inline void writeFloat(float value) {this->wBuffer.writeFloat(value);};
			inline float readFloat() {return (this->rBuffer.readFloat());};
			inline void writeDouble(double value) {this->wBuffer.writeDouble(value);};
			inline double readDouble() {return (this->rBuffer.readDouble());};
			inline void writeString(std::string &value) {this->wBuffer.writeString(value);};
			inline std::string readString() {return (this->rBuffer.readString());};
			inline bool setBlocking(bool blocking) {return (this->socket.setBlocking(blocking));};
			inline bool setNagle(bool nagle) {return (this->socket.setNagle(nagle));};
			inline Socket &getSocket() {return (this->socket);};
			inline Buffer &getRBuffer() {return (this->rBuffer);};
			inline Buffer &getWBuffer() {return (this->wBuffer);};

	};

}

#endif
