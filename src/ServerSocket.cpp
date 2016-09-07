#include "ServerSocket.h"
#include <cstring>

namespace libnet
{

	ServerSocket::ServerSocket()
	{
		this->bound = false;
		this->opened = false;
	}

	ServerSocket::~ServerSocket()
	{
		if (this->opened)
			closesocket(sockfd);
	}

	bool ServerSocket::open()
	{
		if (this->opened)
			return (false);
		if ((sockfd = ::socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
			return (false);
		this->opened = true;
		return (true);
	}

	bool ServerSocket::bind(uint16_t port)
	{
		if (!this->opened)
			return (false);
		memset((char *)&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(port);
		if (::bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
			return (false);
		listen(sockfd, 255);
		this->bound = true;
		return (true);
	}

	Socket *ServerSocket::accept()
	{
		struct sockaddr cli_addr;
		SOCKLEN_T cli_len;
		SOCKET newsockfd;

		if (!this->bound)
			return (NULL);
		cli_len = sizeof(cli_addr);
		if ((newsockfd = ::accept(this->sockfd, (struct sockaddr*)&cli_addr, &cli_len)) == -1)
			return (NULL);
		return (new Socket(newsockfd));
	}

	bool ServerSocket::setNagle(bool active)
	{
		int flag = active ? 1 : 0;
		return (setsockopt(this->sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)) == SOCKET_ERROR);
	}

	bool ServerSocket::setBlocking(bool blocking)
	{
		#ifdef PLATFORM_WINDOWS
			u_long iMode = blocking ? 0 : 1;
			return (ioctlsocket(sockfd, FIONBIO, &iMode) == 0);
		#elif defined PLATFORM_LINUX
			if (!this->opened)
				return (false);
			int flags = fcntl(sockfd, F_GETFL, 0);
			if (flags < 0)
				return (false);
			flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
			return ((fcntl(sockfd, F_SETFL, flags) == 0) ? true : false);
		#else
			#error Not supported platform
		#endif
	}

}
