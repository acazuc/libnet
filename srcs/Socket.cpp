#include "Socket.h"
#include <cstring>
#include <iostream>

namespace net
{

	Socket::Socket(SOCKET sock)
	{
		this->connected = true;
		this->sockfd = sock;
		this->opened = true;
	}

	Socket::Socket()
	{
		this->connected = false;
		this->opened = false;
	}

	Socket::~Socket()
	{
		if (this->opened)
			closesocket(sockfd);
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
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
		serv_addr.sin_port = htons(port);
		if (::connect(this->sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR && errno != EINPROGRESS)
			return (false);
		this->connected = true;
		return (true);
	}

	int32_t Socket::send(Buffer &buffer)
	{
		int32_t written;

		if (!this->connected)
			return (-1);
		buffer.flip();
		if ((written = ::send(sockfd, buffer.getDatas() + buffer.getPosition(), buffer.getRemaining(), 0)) == SOCKET_ERROR)
		{
			#ifdef PLATFORM_WINDOWS
				int error = WSAGetLastError();
				if (error != WSAEWOULDBLOCK)
					return (-1);
			#elif defined PLATFORM_LINUX
				if (errno != EWOULDBLOCK && errno != EAGAIN)
					return (-1);
			#else
			# error Platform not supported
			#endif
		}
		if (written == -1)
			written = 0;
		buffer.setPosition(buffer.getPosition() + written);
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
			char tmp[remaining];
			memcpy(tmp, buffer.getDatas() + buffer.getPosition(), remaining);
			buffer.clear();
			buffer.writeBytes(tmp, remaining);
		}
		else
		{
			buffer.clear();
		}
		if ((readed = ::recv(sockfd, buffer.getDatas() + buffer.getPosition(), buffer.getRemaining(), 0)) == SOCKET_ERROR)
		{
			#ifdef PLATFORM_WINDOWS
				int error = WSAGetLastError();
				if (error != WSAEWOULDBLOCK)
				{
					buffer.flip();
					return (-1);
				}
			#elif defined PLATFORM_LINUX
				if (errno != EWOULDBLOCK && errno != EAGAIN)
				{
					buffer.flip();
					return (-1);
				}
			#else
			# error Platform not supported
			#endif
		}
		if (readed == -1)
			readed = 0;
		buffer.setPosition(buffer.getPosition() + readed);
		buffer.flip();
		return (readed);
	}

	bool Socket::setBlocking(bool blocking)
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
