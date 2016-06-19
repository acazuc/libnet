#include "Connection.h"

namespace net
{

	Connection::Connection(Socket &sock)
	: socket(sock)
	{
	}

	Connection::~Connection()
	{
		//Empty
	}

}
