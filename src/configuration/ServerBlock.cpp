#include "ServerBlock.hpp"


template <typename T>
T	getLastElementVector(const std::vector<T> vec)
{
	return (vec[vec.size() - 1]);
} 

/** @brief Creates new server block. This is hardcoded,
 * we should get this from the configuration file */
ServerBlock::ServerBlock(const std::string &cont)
	: _content(cont), _separator(' '), _end(";")
{
	_m["server_name"] = new ServerName(SERVER);
	_m["index"] = new Index(GLOBAL); // Ainda nao encontrei a doc do index
	_m["error_page"] = new ErrorPage(GLOBAL);
	_m["root"] = new Root(GLOBAL);
	_m["listen"] = new Listen(SERVER); // Do listen function
	_m["client_max_body_size"] = new ClientMaxBodySize(GLOBAL); //
	_m["limit_except"] = new LimitExcept(LOCATION);
	_m["autoindex"] = new AutoIndex(GLOBAL);
}

ServerBlock::~ServerBlock()
{
	std::map<std::string, Directives*>::iterator it = _m.begin();

	for (; it != _m.end(); it++)
		delete it->second;
	for (size_t i = 0; i < _locations.size(); i++)
		delete _locations[i];
}

ServerBlock::ServerBlock(const ServerBlock &cpy)
{
	_m = cpy._m;
	_locations = cpy._locations;
	_content = cpy._content;
	_end = cpy._end;
	_separator = cpy._separator;
}

void	ServerBlock::parseFile()
{
	InfoConfig confInfo;

	for (size_t i = 0; i < _content.length(); i++)
	{
		while (_content[i] && !isalpha(_content[i]))
			i++;

		std::string value = slice_str(_content, SPACES, i);
		if (value.empty())
			break ;

		if (!confInfo.is_context(value) && !confInfo.is_directive(value))
			throw ValueNotFound();
		if (value == "location")
		{
			_locations.push_back(new Location(get_context_block(_content, ++i)));
			getLastElementVector(_locations)->parseLocation();
		}
		else if (value == "server")
			throw std::exception();
		else
			_m[value]->action(slice_str(_content, _end, ++i), SERVER);
	}
	setPorts();
}

bool	ServerBlock::hasPort(size_t port)
{
	for (size_t i = 0; i < _ports.size(); i++)
	{
		if (_ports[i] == port)
			return (true);
	}
	return (false);
}

const std::vector<Location*>	&ServerBlock::getLocation() const
{
	return (_locations);
}

t_autoindex	ServerBlock::getAutoIndex()
{
	return (dynamic_cast<AutoIndex *>(_m["autoindex"])->getValue());
}

void	ServerBlock::setPorts()
{
	Listen *listen = dynamic_cast<Listen *>(_m["listen"]);
	listen->check_dup_listen_directives();
	ListenIndv *listenIndv = NULL;
	for (size_t i = 0; (listenIndv = listen->getListenIndv(i)) != NULL; i++)
		_ports.push_back(listenIndv->getValue().second);
	if (_ports.empty())
		throw std::exception();
}

std::vector<size_t>	ServerBlock::getPorts() const 
{
	return (_ports);
}

