#ifndef NET_SOCKET_H
# define NET_SOCKET_H

# include "Platform.h"

# include "Buffer.h"
# include <string>

namespace net
{

	class Socket
	{

		private:
			bool connected;
			bool opened;
			int sockfd;

		public:
			Socket(int sock);
			Socket();
			~Socket();
			bool open();
			bool connect(std::string &, int);
			ssize_t send(Buffer &buffer);
			ssize_t read(Buffer &buffer);
			bool setBlocking(bool blocking);

	};

}

#endif
