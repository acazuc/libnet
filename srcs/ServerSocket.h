#ifndef NET_SERVER_SOCKET_H
# define NET_SERVER_SOCKET_H

# include <netinet/in.h>
# include "Socket.h"
# include <cstdint>

namespace net
{

	class ServerSocket
	{

		private:
			struct sockaddr_in server_addr;
			bool opened;
			bool bound;
			int sockfd;

		public:
			ServerSocket();
			bool open();
			bool bind(uint16_t port);
			bool setBlocking(bool blocking);
			Socket *accept();

	};

}

#endif
