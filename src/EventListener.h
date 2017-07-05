#ifndef LIBNET_EVENT_LISTENER_H
# define LIBNET_EVENT_LISTENER_H

# include "Platform.h"

# ifdef PLATFORM_LINUX

# include "Socket.h"

# define EVENT_READ 1
# define EVENT_WRITE 2
# define EVENT_RDWR 3

namespace libnet
{

	class EventListener
	{

	private:
		int fd;

	public:
		EventListener();
		~EventListener();
		void waitForEvents();
		void addSocket(Socket *socket, uint8_t events);
		void removeSocket(Socket *socket, uint8_t events);

	};

}

# endif

#endif
