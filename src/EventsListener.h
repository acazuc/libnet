#ifndef LIBNET_EVENTS_LISTENER_H
# define LIBNET_EVENTS_LISTENER_H

# include "Platform.h"

# ifdef LIBNET_PLATFORM_LINUX

# include "Connection.h"
# include <sys/epoll.h>

# define EVENT_READ 1
# define EVENT_WRITE 2
# define EVENT_RDWR 3

namespace libnet
{

	class EventsListener
	{

	private:
		int fd;

	public:
		EventsListener();
		~EventsListener();
		int waitForEvents(struct epoll_event *events, int eventsCount, int timeout);
		bool addSocket(Socket *socket, uint8_t events, void *data);
		bool removeSocket(Socket *socket);

	};

}

# endif

#endif
