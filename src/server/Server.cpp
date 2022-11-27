#include "Server.hpp"

Server::Server(ServerBlock *sb) : _sb(sb)
{
	ListenIndv *listenIndv = NULL;
	for (size_t j = 0; (listenIndv = _sb->dir<Listen>("listen")->getListenIndv(j)) != NULL ; j++)
		_socket_set.push_back(new Socket(AF_INET, SOCK_STREAM, 0, listenIndv->getValue().second));
	_socket_set[0]->getReadAndWriteSize();
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
	for (std::vector<Socket *>::iterator socket = _socket_set.begin(); socket != _socket_set.end(); socket++)
	{
		Poll *poll = (*socket)->getPoll();
		poll->waitForConnection();
		for (size_t i = 0; i < poll->nbrOfEvents(); i++)
		{
			pollfd *event = poll->getReturnedEvents(i);
			if (isNewConnection(event->fd, (*socket)->getSocket()))
				_clients.push_back(new Client((*socket)->socketAccept(), _sb));
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
}
