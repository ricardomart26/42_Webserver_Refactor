#include "Server.hpp"

Server::Server(size_t port, std::vector<ServerBlock *> sbVector) : _socket(AF_INET, SOCK_STREAM, 0, port)
{
	_socket.getReadAndWriteSize();

	for (size_t i = 0; i < sbVector.size(); i++)
	{
		if (sbVector[i]->hasPort(port))
			_sb.push_back(sbVector[i]);
	}
}

Server::~Server() {}

/**
 * @brief returns true if is server socket that has something 
 * to be read
 */
bool	isNewConnection(int fd, int server_socket)
{
	return (fd == server_socket);
}


/**
 * @brief returns true if client socket that has something 
 * to be read
 */
bool	isReadyToRead(int revents)
{
	return (revents & POLLIN);
}


/**
 * @brief returns true if client socket that has something 
 * to write
 */
bool	isReadyToWrite(int revents)
{
	return (revents & POLLOUT);
}

std::vector<Client *>::iterator	Server::getClient(int event_fd)
{
	std::vector<Client *>::iterator	it = _clients.begin();
	for (; it != _clients.end(); it++)
	{
		if ((*it)->getClientSocket() == event_fd)
			break ;
	}
	return (it);
}

void	Server::remove_client(std::vector<Client *>::iterator client, Poll *poll)
{
	/**
	 * Entra nesta condição quando o request é vazio (acho que é para "fechar" o cliente).
	 * Mas tenho que ter cuidado se o tamanho do request é maior que o que vai ser possivel
	 * para ler (_READ_SIZE), assim vai eliminar o cliente, em vez de ler o resto do pedido!
	 */
	std::cout << "Client Removed\n";
	poll->removeEvent((*client)->getClientSocket());
	close((*client)->getClientSocket());
	_clients.erase(client);
}

void	Server::run()
{
	Poll *poll = _socket.getPoll();
	poll->waitForConnection();
	for (size_t i = 0; i < poll->nbrOfEvents(); i++)
	{
		pollfd *event = poll->getReturnedEvents(i);
		if (isNewConnection(event->fd, _socket.getSocket()))
			_clients.push_back(new Client(_socket.socketAccept(), _sb));
		else
		{
			try {
				std::vector<Client *>::iterator	client = getClient(event->fd);
				if (isReadyToRead(event->revents))
				{
					if (!(*client)->createRequest())
						remove_client(client, poll);
				}
				else if (isReadyToWrite(event->revents) && (*client)->getRequest() != NULL)
				{

					if (!(*client)->createResponse())
						remove_client(client, poll);
				}
			} catch (const std::exception &e) {
				std::cout << e.what() << std::endl;
			}
		}
	}
}
