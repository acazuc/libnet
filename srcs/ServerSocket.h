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
			bool bind(uint16_t port);
			bool setBlocking(bool blocking);
			Socket *accept();

	};

}

#endif
