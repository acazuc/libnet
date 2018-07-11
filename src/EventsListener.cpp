#include "EventsListener.h"

#ifdef LIBNET_PLATFORM_LINUX

namespace libnet
{

	EventsListener::EventsListener()
	{
		if ((this->fd = epoll_create(1)) == -1)
			throw std::exception();
	}

	EventsListener::~EventsListener()
	{
		close(this->fd);
	}

	int EventsListener::waitForEvents(struct epoll_event *events, int eventsCount, int timeout)
	{
		return (epoll_wait(this->fd, events, eventsCount, timeout));
	}

	bool EventsListener::addSocket(Socket *socket, uint8_t mode, void *data)
	{
		struct epoll_event event;
		event.events = (mode & EVENT_READ ? (int)EPOLLIN : 0) | (mode & EVENT_WRITE ? (int)EPOLLOUT : 0) | (mode & EVENT_RDHUP ? (int)EPOLLRDHUP : 0);
		event.data.ptr = data;
		return (epoll_ctl(this->fd, EPOLL_CTL_ADD, socket->getSockfd(), &event) == 0);
	}

	bool EventsListener::removeSocket(Socket *socket)
	{
		return (epoll_ctl(this->fd, EPOLL_CTL_DEL, socket->getSockfd(), NULL) == 0);
	}

}

#endif
