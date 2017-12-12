#ifndef LIBNET_CONNECTION_H
# define LIBNET_CONNECTION_H

# include "Buffer.h"
# include "Socket.h"
# include "Packet.h"
# include <cstdint>
# include <string>
# include <queue>

namespace libnet
{

	class Connection
	{

		private:
			std::queue<Packet*> packets;
			Packet *currentPacket;
			Socket &socket;
			Buffer rBuffer;
			Buffer wBuffer;
			uint32_t packetStart;
			bool crypted;

		public:
			Connection(Socket &socket);
			~Connection();
			void startPacket(uint16_t packet);
			void endPacket();
			bool initCrypt(const void *key, size_t keylen);
			void disableCrypt();
			int32_t read();
			int32_t send();
			void writeBytes(const void *data, size_t len);
			void readBytes(void *data, size_t len);
			void writeBool(bool value);
			bool readBool();
			void writeInt8(int8_t value);
			int8_t readInt8();
			void writeUInt8(uint8_t value);
			uint8_t readUInt8();
			void writeInt16(int16_t value);
			int16_t readInt16();
			void writeUInt16(uint16_t value);
			uint16_t readUInt16();
			void writeInt32(int32_t value);
			int32_t readInt32();
			void writeUInt32(uint32_t value);
			uint32_t readUInt32();
			void writeInt64(int64_t value);
			int64_t readInt64();
			void writeUInt64(uint64_t value);
			uint64_t readUInt64();
			void writeFloat(float value);
			float readFloat();
			void writeDouble(double value);
			double readDouble();
			void writeString(std::string &value);
			std::string readString();
			inline std::queue<Packet*> &getPackets() {return (this->packets);};
			inline bool setBlocking(bool blocking) {return (this->socket.setBlocking(blocking));};
			inline bool setNagle(bool nagle) {return (this->socket.setNagle(nagle));};
			inline Socket &getSocket() {return (this->socket);};
			inline Buffer &getRBuffer() {return (this->rBuffer);};
			inline Buffer &getWBuffer() {return (this->wBuffer);};
			inline bool isCrypted() {return (this->crypted);};

	};

}

#endif
