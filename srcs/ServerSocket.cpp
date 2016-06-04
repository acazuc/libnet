#include "ServerSocket.h"
#include <sys/socket.h>
#include <strings.h>
#include <fcntl.h>

namespace net
{

	ServerSocket::ServerSocket()
	{
		this->bound = false;
		this->opened = false;
	}

	bool ServerSocket::open()
	{
		if (this->opened)
			return (false);
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			return (false);
		this->opened = true;
		return (true);
	}

	bool ServerSocket::bind(uint16_t port)
	{
		if (!this->opened)
			return (false);
		bzero((char *)&server_addr, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(port);
		if (::bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
			return (false);
		listen(sockfd, 100);
		this->bound = true;
		return (true);
	}

	bool ServerSocket::setBlocking(bool blocking)
	{
		if (!this->opened)
			return (false);
		int flags = fcntl(sockfd, F_GETFL, 0);
		if (flags < 0)
		{
			return (false);
		}
		flags = blocking ? (flags&~SOCK_NONBLOCK) : (flags | SOCK_NONBLOCK);
		return ((fcntl(sockfd, F_SETFL, flags) == 0) ? true : false);
	}

	Socket *ServerSocket::accept()
	{
		struct sockaddr cli_addr;
		socklen_t cli_len;
		int newsockfd;

		if (!this->bound)
			return (NULL);
		cli_len = sizeof(cli_addr);
		if ((newsockfd = ::accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len)) == -1)
			return (NULL);
		return (new Socket(newsockfd));
	}

}
