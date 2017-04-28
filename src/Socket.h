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
			SOCKADDR_IN cli_addr;
			SOCKET sockfd;
			bool waitingConnection;
			bool connected;
			bool opened;

		public:
			Socket(SOCKET sockfd, SOCKADDR_IN cli_addr);
			Socket();
			~Socket();
			bool open();
			bool close();
			bool shutdown();
			bool connect(std::string host, uint16_t port);
			int getConnectionStatus();
			int32_t send(Buffer &buffer);
			int32_t read(Buffer &buffer);
			bool setNagle(bool active);
			bool setBlocking(bool blocking);
			SOCKET getSockfd() {return (this->sockfd);};
			inline uint32_t getIp() {return (this->cli_addr.sin_addr.s_addr);};

	};

}

#endif
