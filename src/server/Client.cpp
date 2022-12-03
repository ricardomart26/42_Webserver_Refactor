#include "Client.hpp"

Client::Client(int fd, std::vector<ServerBlock *> sb)
	: _clientSocket(fd), _keep_alive(false), _request(NULL), _response(NULL), _finished(false), _sbVector(sb), _sb(NULL) 
{
	std::cout << "@CLIENT: \n";
}

Client::~Client() {}

Response	*Client::initResponse(int clientFd)
{
	if (_response != NULL)
		return (_response);
	std::string method = _request->getMethod();
	if (!method.compare("POST"))
		return (new Post(_request, clientFd, _sb));
	else if (!method.compare("GET")) 
		return (new Get(_request, clientFd, _sb));
	else if (!method.compare("DELETE")) 
		return (new Delete(_request, clientFd, _sb));
	else
		throw 405;
}

bool	Client::createRequest()
{
	try
	{
		// Se for um novo pedido
		if (_request != NULL)
			delete	_request;
		_request = new Request(_clientSocket, _sbVector);
		_sb = _request->findServerBlock(); 
	}
	catch (const std::exception &e) 
	{
		std::cout << e.what() << std::endl;
		return (false);
	} 
	return (true);
}

bool	Client::isFinished()
{
	if (_response->finished())
	{
		if (_response != NULL)
			delete _response;
		if (_request != NULL)
			delete _request;
		_response = NULL;
		_request = NULL;
		return (true);
	}
	return (false); 
}

bool	Client::createResponse()
{
	try 
	{
		_response = initResponse(_clientSocket);
		if (_response->responseIsEmpty())
			_response->contructResponse(*_request);
		_response->answer();
	}
	catch (size_t err) 
	{
		_response = new Get(_request, _clientSocket, _sb, err);
	}
	catch (const std::exception &e)
	{
		return (false);
	}
	isFinished();
	return (true);
}

int	Client::getClientSocket() const
{
	return (_clientSocket);
}

Request	*Client::getRequest() const
{
	return (_request);
}
