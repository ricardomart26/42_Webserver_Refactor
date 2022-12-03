#include "Poll.hpp"
#include <poll.h>

Poll::Poll(int server_socket, size_t size)  : _size(size), _nbrOfEvents(0)
{
	_events_poll = new pollfd[_size];
	for (size_t i = 0; i < _size; i++)
	{
		_events_poll[i].fd = -1;
		_events_poll[i].events = 0;
	}
	_events_poll[0].fd = server_socket;
	_events_poll[0].events |= POLLIN;

}

Poll::~Poll() { std::cout << "Poll destructed\n"; }

void	Poll::waitForConnection() 
{
	// static const timespec tm = {0, 2};

	if (_nbrOfEvents != 0)
		delete[] _events_returned;
	
	
	// int timeout = -1;
	// int timeout = 100000;
	
	_nbrOfEvents = poll(_events_poll, _size, 10);

	if (_nbrOfEvents == 0)
		return ;
				
	_events_returned = new struct pollfd*[_nbrOfEvents];

	size_t x = 0;
	for (size_t i = 0; x < _nbrOfEvents && i < _size; i++)
	{
		if (_events_poll[i].revents & (POLLIN | POLLOUT))
			_events_returned[x++] = &_events_poll[i];
	}
}

void	Poll::add_event(int clientFd)
{
	for (size_t counter = 0; counter != this->_size; counter++)
	{
		if (_events_poll[counter].fd == -1)
			return set_event(counter, clientFd, (POLLIN | POLLOUT));
	}
	error_and_exit("Client not found in add event");
}

void	Poll::removeEvent(int clientFd)
{
	for (size_t counter = 0; counter != _size; counter++)
	{
		// std::cout << "fd: " << _events_poll[counter].fd << " client fd: " << clientFd << std::endl;
		if (_events_poll[counter].fd == clientFd)
			return set_event(counter, -1, 0);
	}
	error_and_exit("Client not found in remove event");
}

// struct pollfd	&Poll::get_event(size_t pos) { return (_events_poll[pos]); }
struct pollfd	*Poll::getReturnedEvents(size_t pos) { return (&(*_events_returned[pos])); }
size_t			Poll::nbrOfEvents() { return (_nbrOfEvents); }

void	Poll::set_event(size_t pos, int fd, short event)
{
	_events_poll[pos].fd = fd;
	_events_poll[pos].events = event;
	_events_poll[pos].revents = 0;
}

void	Poll::reset_connection(size_t pos)
{
	for (size_t x = 0; x < _size; x++)
	{
		if (_events_returned[pos]->fd == _events_poll[x].fd)
		{
			set_event(x, -1, 0);
			for ( ;x + 1 < _size; x++)
				_events_poll[x] = _events_poll[x + 1];
		}
	}
}
