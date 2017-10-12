#include "Packet.h"
#include "SocketPlatform.h"
#include <stdexcept>
#include <cstring>
#include <utf8.h>

namespace libnet
{

	Packet::Packet()
	: buffer(1)
	{
		//
	}

	Packet::~Packet()
	{
		//
	}

}
