#include "Connection.h"

namespace net
{

	Connection::Connection(Socket &socket)
	{
		this->socket = &socket;
		this->rBuffer = new Buffer();
		this->wBuffer = new Buffer();
	}

	Connection::~Connection(void)
	{
		delete (socket);
		delete (rBuffer);
		delete (wBuffer);
	}

	int16_t Connection::read(void)
	{
		return (this->socket->read(*rBuffer));
	}

	int16_t Connection::send(void)
	{
		return (this->socket->send(*wBuffer));
	}

}
