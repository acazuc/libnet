#ifndef NET_SERVER_SOCKET_H
# define NET_SERVER_SOCKET_H

# include "Platform.h"

# ifdef PLATFORM_LINUX
#  include <netinet/in.h>
# elif defined PLATFORM_WINDOWS
#  include <winsock2.h>
#endif
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
