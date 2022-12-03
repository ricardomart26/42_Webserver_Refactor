#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <netdb.h>

#include "Socket.hpp"
#include "Config.hpp"
#include "Client.hpp"
#include "Poll.hpp"
#include "Directives.hpp"

class Server
{
	public:

		Server();
		Server(size_t ports, std::vector<ServerBlock *> sb);
		~Server();

		void	setupSockets();
		void	run();
		size_t	nbr_of_servers() const;

	private:
		
		void							remove_client(std::vector<Client *>::iterator client, Poll *socket);
		std::vector<Client *>::iterator	getClient(int event_fd);

		std::vector<Client *>		_clients;
		std::vector<ServerBlock *>	_sb;
		Socket						_socket;

};

#endif
