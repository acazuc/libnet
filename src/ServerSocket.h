#ifndef LIBNET_SERVER_SOCKET_H
# define LIBNET_SERVER_SOCKET_H

# include "SocketPlatform.h"
# include "Socket.h"
# include <cstdint>

namespace libnet
{

	class ServerSocket
	{

		private:
			SOCKADDR_IN server_addr;
			bool opened;
			bool bound;
			SOCKET sockfd;

		public:
			ServerSocket();
			~ServerSocket();
			bool open();
			bool close();
			bool shutdown();
			bool bind(uint16_t port);
			Socket *accept();
			bool setNagle(bool active);
			bool setBlocking(bool blocking);
			inline SOCKET getSockfd() {return this->sockfd;};

	};

}

#endif
