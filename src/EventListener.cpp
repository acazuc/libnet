#include "EventListener.h"

#ifdef PLATFORM_LINUX

namespace libnet
{

	EventListener::EventListener()
	{
		if ((this->fd = epoll_create(1)) == -1)
			throw std::exception();
	}

	EventListener::~EventListener()
	{
		close(this->fd);
	}

	bool EventListener::waitForEvents()
	{
		int eventsCount = 50;
		struct epoll_event events[eventsCount];
		int ret;
		if ((ret = epoll_wait(this->fd, &events, eventsCount, 0)) == -1)
			return (false);
		for (int i = 0; i < ret; ++i)
		{
			if (event[i].events & EPOLLIN)
			{
				reinterpret_cast<Connection*>(event.data.ptr)->read();
			}
		}
	}

	bool EventListener::addSocket(Connection *conn, uint8_t mode)
	{
		struct epoll_event event;
		event.events = (mode & EVENT_READ ? EPOLLIN : 0) + (mode & EVENT_WRITE ? EPOLLOUT : 0);
		event.data.ptr = conn;
		return (epoll_ctl(this->fd, EPOLL_CTL_ADD, conn.getSocket()->getFd(), &event) == 0);
	}

	bool EventListener::removeSocket(Connection *conn)
	{
		return (epoll_ctl(this->fd, EPOLL_CTL_DEL, conn->getSocket()->getFd(), NULL) == 0);
	}

}

#endif
