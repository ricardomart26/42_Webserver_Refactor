#include "Socket.hpp"

unsigned int Request::_READ_SIZE = 0;
unsigned int Response::_WRITE_SIZE = 0;

Socket::Socket(int family, int type, int protocol, size_t port)
{
	_address_family = family;

	if ((_server_socket = socket(_address_family, type, protocol)) == -1)
		error_and_exit("Socket error");

	_poll = new Poll(_server_socket, MAX_CONN);
	if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) == -1)
		error_and_exit("fcntl error");

	int opt = 0;
	setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	setsockopt(_server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	socket_bind(port);
	socket_listen(MAX_CONN);
	
}

/**
 * @brief Destroy the Socket:: Socket object
 */
Socket::~Socket() 
{
	// std::cout << "--Socket destructor--\n"; 
	close(_server_socket);
	delete	_poll;
}

Poll	*Socket::getPoll() 
{
	return (_poll);
}

int		Socket::getSocket()
{
	return (_server_socket);
}

/**
 * @brief Gets max read and max write size from sockets.
 */
void	Socket::getReadAndWriteSize()
{
	int read_size = 0;
	// https://stackoverflow.com/questions/4257410/what-are-so-sndbuf-and-so-rcvbuf
	unsigned int sizeof_buffer_size = sizeof(read_size);
	getsockopt(_server_socket, SOL_SOCKET, SO_RCVBUF,
					(void *)&read_size, &sizeof_buffer_size);

	Request::_READ_SIZE = read_size;

	int write_size = 0;
	sizeof_buffer_size = sizeof(write_size);
	getsockopt(_server_socket, SOL_SOCKET, SO_SNDBUF,
					(void *)&write_size, &sizeof_buffer_size);

	Response::_WRITE_SIZE = write_size;
}

/**
 * @brief Bind is used to attach and address and port to the server socket,
 * so the client knows which addr and port do connect to.
 */
void	Socket::socket_bind(size_t port)
{
	// std::cout << "port: " << port << std::endl;
	// host = gethostbyname(serverName.c_str());
	// if (!host)
	// 	error_and_exit("GetHostByName error");
	// bcopy((char *)host->h_addr_list[0], (char *)&_sock_addr.sin_addr.s_addr, host->h_length);

	_port = port;
	_sock_addr.sin_family = _address_family; // ipv4 address
	_sock_addr.sin_addr.s_addr = INADDR_ANY;
	_sock_addr.sin_port = htons(_port); // port

	// https://man7.org/linux/man-pages/man2/bind.2.html
	if (bind(_server_socket, (struct sockaddr *)&_sock_addr, (socklen_t)sizeof(_sock_addr)) == -1)
		error_and_exit("Bind error");
}

void	Socket::socket_listen(int con)
{
	if (listen(_server_socket, con) == -1)
		error_and_exit("Bind error");
}

int	Socket::socketAccept()
{
	unsigned int sock_len;
	struct sockaddr_in client_addr;

	int client_fd = accept(_server_socket, (struct sockaddr *)&client_addr, &sock_len);
	if (client_fd == -1)
		error_and_exit("Accept Error");
	
	int opt;
	setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	setsockopt(client_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	// std::cout << "New client: " << client_fd << std::endl;

	fcntl(client_fd, F_SETFL, O_NONBLOCK);	
	_poll->add_event(client_fd);
	return (client_fd);
}
