#ifndef LIBNET_SOCKET_H
# define LIBNET_SOCKET_H

# include "SocketPlatform.h"
# include "Buffer.h"
# include <string>

namespace libnet
{

	class Socket
	{

		private:
			bool waitingConnection;
			bool connected;
			bool opened;
			bool crypt;
			SOCKET sockfd;

		public:
			Socket(SOCKET sock);
			Socket();
			~Socket();
			bool open();
			bool close();
			bool connect(std::string host, uint16_t port);
			int getConnectionStatus();
			int32_t send(Buffer &buffer);
			int32_t read(Buffer &buffer);
			bool setNagle(bool active);
			bool setBlocking(bool blocking);
			SOCKET getSockfd() {return (this->sockfd);};
			inline void setCrypt(bool crypt) {this->crypt = crypt;};

	};

}

#endif
