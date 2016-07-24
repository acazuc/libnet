#ifndef NET_CONNECTION_H
# define NET_CONNECTION_H

# include "Buffer.h"
# include "Socket.h"
# include <cstdint>
# include <string>

namespace net
{

	class Connection
	{

		private:
			Socket &socket;
			Buffer rBuffer;
			Buffer wBuffer;
			uint32_t packetStart;

		public:
			Connection(Socket &socket);
			~Connection();
			void startPacket();
			void endPacket();
			inline int32_t read() {return (this->socket.read(rBuffer));};
			inline int32_t send() {return (this->socket.send(wBuffer));};
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
			inline void setBlocking(bool blocking) {this->socket.setBlocking(blocking);};
			inline void setCrypt(bool crypt) {this->socket.setCrypt(crypt);};
			inline Socket &getSocket() {return (this->socket);};
			inline Buffer &getRBuffer() {return (this->rBuffer);};
			inline Buffer &getWBuffer() {return (this->wBuffer);};

	};

}

#endif
