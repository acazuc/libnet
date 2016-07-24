#ifndef NET_SOCKET_H
# define NET_SOCKET_H

# include "SocketPlatform.h"
# include "Buffer.h"
# include <string>

namespace net
{

	class Socket
	{

		private:
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
			int32_t send(Buffer &buffer);
			int32_t read(Buffer &buffer);
			bool setBlocking(bool blocking);
			SOCKET getSockfd() {return (this->sockfd);};
			inline void setCrypt(bool crypt) {this->crypt = crypt;};

	};

}

#endif
