#include "Socket.h"
#include <cstring>
#include <iostream>

namespace libnet
{

	Socket::Socket(SOCKET sockfd, SOCKADDR_IN cli_addr)
	: cli_addr(cli_addr)
	, sockfd(sockfd)
	{
		this->waitingConnection = false;
		this->connected = true;
		this->opened = true;
	}

	Socket::Socket()
	{
		this->waitingConnection = false;
		this->connected = false;
		this->opened = false;
	}

	Socket::~Socket()
	{
		close();
	}

	bool Socket::open()
	{
		if (this->opened)
			return (false);
		if ((this->sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
			return (false);
		this->opened = true;
		return (true);
	}

	bool Socket::close()
	{
		if (this->opened)
		{
			closesocket(sockfd);
			this->opened = false;
			this->connected = false;
			this->waitingConnection = false;
			return (true);
		}
		return (false);
	}

	bool Socket::connect(std::string host, uint16_t port)
	{
		SOCKADDR_IN serv_addr;
		struct hostent *server;

		if (!this->opened)
			return (false);
		if (!(server = gethostbyname(host.c_str())))
			return (false);
		std::memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
		serv_addr.sin_port = htons(port);
		if (::connect(this->sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		{
			#ifdef LIBNET_PLATFORM_WINDOWS
				if (WSAGetLastError() != WSAEWOULDBLOCK)
					return (false);
			#elif defined LIBNET_PLATFORM_LINUX
				if (errno != EINPROGRESS)
					return (false);
			#else
				#error Platform not supported
			#endif
			this->waitingConnection = true;
		}
		else
			this->connected = true;
		return (true);
	}

	int Socket::getConnectionStatus()
	{
		if (this->connected)
			return (1);
		if (!this->waitingConnection)
			return (-1);
		struct timeval tv;
		fd_set fdset;
		int ret;

		tv.tv_sec = 0;
		tv.tv_usec = 1000;
		FD_ZERO(&fdset);
		FD_SET(this->sockfd, &fdset);
		if ((ret = select(this->sockfd + 1, NULL, &fdset, &fdset, &tv)))
		{
			this->waitingConnection = false;
			if (ret == SOCKET_ERROR)
				return (-1);
			int err;
			socklen_t len = sizeof(err);
			if (getsockopt(this->sockfd, SOL_SOCKET, SO_ERROR, (char*)&err, &len) == SOCKET_ERROR)
				return (-1);
			#ifdef LIBNET_PLATFORM_WINDOWS
				if (err && err != WSAEINPROGRESS)
					return (-1);
			#elif defined LIBNET_PLATFORM_LINUX
				if (err && err != EINPROGRESS)
					return (-1);
			#else
				#error Unsupported platform
			#endif
			this->connected = true;
			return (1);
		}
		return (0);
	}

	int32_t Socket::send(Buffer &buffer)
	{
		int32_t written;

		if (!this->connected)
			return (-1);
		if (buffer.getPosition() == 0)
			return (-2);
		buffer.flip();
		if (buffer.getRemaining() > 0)
		{
			if ((written = ::send(this->sockfd, buffer.getDatas() + buffer.getPosition(), buffer.getRemaining(), 0)) == SOCKET_ERROR)
			{
				#ifdef LIBNET_PLATFORM_WINDOWS
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						written = -1;
						goto clear;
					}
				#elif defined LIBNET_PLATFORM_LINUX
					if (errno != EWOULDBLOCK && errno != EAGAIN)
					{
						written = -1;
						goto clear;
					}
				#else
					#error Platform not supported
				#endif
				return (-2);
			}
			buffer.setPosition(buffer.getPosition() + written);
		}
		else
			written = -2;
		clear:
		if (buffer.getPosition() < buffer.getLimit())
		{
			uint32_t remaining = buffer.getRemaining();
			char tmp[remaining];
			memcpy(tmp, buffer.getDatas() + buffer.getPosition(), remaining);
			buffer.clear();
			buffer.writeBytes(tmp, remaining);
		}
		else
			buffer.clear();
		return (written);
	}

	int32_t Socket::read(Buffer &buffer)
	{
		int32_t readed;

		if (!this->connected)
			return (-1);
		if (buffer.getPosition() < buffer.getLimit())
		{
			uint32_t remaining = buffer.getRemaining();
			char *tmp = new char[remaining];
			memcpy(tmp, buffer.getDatas() + buffer.getPosition(), remaining);
			buffer.clear();
			buffer.writeBytes(tmp, remaining);
			delete[] (tmp);
		}
		else
			buffer.clear();
		if (buffer.getRemaining() > 0)
		{
			if ((readed = ::recv(this->sockfd, buffer.getDatas() + buffer.getPosition(), buffer.getRemaining(), 0)) == SOCKET_ERROR)
			{
				buffer.flip();
				#ifdef LIBNET_PLATFORM_WINDOWS
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						readed = -1;
						goto clear;
					}
				#elif defined LIBNET_PLATFORM_LINUX
					if (errno != EWOULDBLOCK && errno != EAGAIN)
					{
						readed = -1;
						goto clear;
					}
				#else
					#error Platform not supported
				#endif
				return (-2);
			}
			buffer.setPosition(buffer.getPosition() + readed);
		}
		else
			readed = -2;
		clear:
		buffer.flip();
		return (readed);
	}

	bool Socket::setNagle(bool active)
	{
		int flag = active ? 1 : 0;
		return (setsockopt(this->sockfd, IPPROTO_TCP, TCP_NODELAY, (void*)&flag, sizeof(flag)) == 0);
	}

	bool Socket::setBlocking(bool blocking)
	{
		#ifdef LIBNET_PLATFORM_WINDOWS
			u_long mode = blocking ? 0 : 1;
			return (ioctlsocket(this->sockfd, FIONBIO, &mode) == 0);
		#elif defined LIBNET_PLATFORM_LINUX
			if (!this->opened)
				return (false);
			int flags = fcntl(this->sockfd, F_GETFL, 0);
			if (flags < 0)
				return (false);
			flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
			return (fcntl(this->sockfd, F_SETFL, flags) == 0);
		#else
			#error Platform not supported
		#endif
	}

}
