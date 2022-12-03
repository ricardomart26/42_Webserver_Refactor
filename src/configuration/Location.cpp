#include "Location.hpp"

size_t	until_alpha(const std::string &str, size_t i)
{
	while (str[i] && !isalpha(str[i]))
		i++;
	return (i);
}

Location::Location(const std::string &block)
{
	_m["server_name"] = new ServerName(SERVER);
	_m["index"] = new Index(GLOBAL); // Ainda nao encontrei a doc do index
	_m["error_page"] = new ErrorPage(GLOBAL);
	_m["root"] = new Root(GLOBAL);
	_m["listen"] = new Listen(SERVER); // Do listen function
	_m["client_max_body_size"] = new ClientMaxBodySize(GLOBAL); //
	_m["limit_except"] = new LimitExcept(LOCATION);
	_m["autoindex"] = new AutoIndex(GLOBAL);

	_prefix = trim(block.substr(0, block.find('{')), SPACES);
	_content = block.substr(block.find('{'));

}

void	Location::parseLocation()
{
	for (size_t i = 0; _content[i]; i++)
	{
		while (_content[i] && (isEqual(_content[i], SPACES) || _content[i] == '}'))
			i++;
		_content = &_content[i];
		i = 0;
		while (_content[i] && !isalpha(_content[i]))
			i++;
		_content = trim(_content);
		if (_content.empty())
			break ;

		std::string directive;
		std::string value;
		char delimitador = ';';
		while (_content[i] && !isEqual(_content[i], SPACES))
			directive.push_back(_content[i++]);
		while (_content[i] && _content[i] != delimitador)
			value.push_back(_content[i++]);
		i += _content[i] == delimitador;
		if (directive[0] == '}' || value.empty())
			break;
		_m[directive]->action(trim(value), LOCATION);
	}
}

Location::~Location() 
{
	for (std::map<std::string, Directives *>::iterator it = _m.begin(); it != _m.end(); it++)
		delete (it->second);
}

Location::Location(const Location &cpy)
{
	_prefix = cpy._prefix;
	_m = cpy._m;
	_index = cpy._index;
	_content = cpy._content;
}

const Location &Location::operator=(const Location &rhs)
{
	if (this == &rhs)
	{
		this->_prefix = rhs._prefix;

		std::map<std::string, Directives*>::const_iterator it = rhs._m.begin();
		for (; it != rhs._m.end(); it++)
			this->_m[it->first] = it->second;

		std::vector<std::string>::const_iterator it_loc = rhs._index.begin();
		for (; it_loc != rhs._index.end(); it_loc++)
			this->_index.push_back(*it_loc);
	}
	return (*this);
}

const std::string &Location::getPrefix() const
{
	return (_prefix);
}

const std::vector<std::string>	&Location::getIndex()
{
	return (dynamic_cast<Index *>(_m["index"])->getValue());
}

t_autoindex	Location::getAutoIndex()
{
	return (dynamic_cast<AutoIndex *>(_m["autoindex"])->getValue());
}

std::string Location::getRoot()
{
	Root *r = dynamic_cast<Root *>(_m[std::string("root")]);
	if (r == NULL)
		return ("");
	return (r->getValue());
}


