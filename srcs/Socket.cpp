#include "Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>

namespace net
{

	Socket::Socket(int sock)
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
			close(sockfd);
	}

	bool Socket::open()
	{
		if (this->opened)
			return (false);
		if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
			return (false);
		this->opened = true;
		return (true);
	}

	bool Socket::connect(std::string &address, int port)
	{
		struct sockaddr_in serv_addr;
		struct hostent *server;

		if (!this->opened)
			return (false);
		if (!(server = gethostbyname(const_cast<char*>(address.c_str()))))
			return (false);
		bzero(&serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		memcpy(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(port);
		if (::connect(this->sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
			return (false);
		this->connected = true;
		return (true);
	}

	ssize_t Socket::send(Buffer &buffer)
	{
		ssize_t written;
		int retry;

		if (!this->connected)
			return (-1);
		buffer.flip();
		retry = 0;
		while (retry < 5 && buffer.getRemaining() > 0)
		{
			if ((written = ::write(sockfd, buffer.getDatas() + buffer.getPosition(), buffer.getRemaining())) == -1 && errno != EWOULDBLOCK && errno != EAGAIN)
				return (-1);
			buffer.setPosition(buffer.getPosition() + written);
			retry++;
		}
		if (buffer.getRemaining() > 0)
		{
			char tmp[buffer.getRemaining()];
			memcpy(tmp, buffer.getDatas() + buffer.getPosition(), sizeof(tmp));
			buffer.clear();
			buffer.writeBytes(tmp, sizeof(tmp));
		}
		else
		{
			buffer.clear();
		}
		return (written);
	}

	ssize_t Socket::read(Buffer &buffer)
	{
		ssize_t readed;

		if (!this->connected)
			return (-1);
		if (buffer.getRemaining() > 0)
		{
			char tmp[buffer.getRemaining()];
			memcpy(tmp, buffer.getDatas() + buffer.getPosition(), sizeof(tmp));
			buffer.clear();
			buffer.writeBytes(tmp, sizeof(tmp));
		}
		else
		{
			buffer.clear();
		}
		if ((readed = ::read(sockfd, buffer.getDatas() + buffer.getPosition(), buffer.getRemaining())) == -1 && errno != EWOULDBLOCK && errno != EAGAIN)
			return (-1);
		if (readed == -1)
			readed = 0;
		buffer.setPosition(readed);
		buffer.flip();
		return (readed);
	}

	bool Socket::setBlocking(bool blocking)
	{
		if (!this->opened)
			return (false);
		int flags = fcntl(sockfd, F_GETFL, 0);
		if (flags < 0)
			return (false);
		flags = blocking ? (flags&~SOCK_NONBLOCK) : (flags | SOCK_NONBLOCK);
		return ((fcntl(sockfd, F_SETFL, flags) == 0) ? true : false);
	}

}
