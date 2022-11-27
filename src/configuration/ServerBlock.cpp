#include "ServerBlock.hpp"

/** @brief Creates new server block. This is hardcoded,
 * we should get this from the configuration file */
ServerBlock::ServerBlock(const std::string &cont)
	: _content(cont), _separator(' '), _end(";"), _autoIndex(false)
{
	_m["server_name"] = new ServerName(SERVER);
	_m["index"] = new Index(GLOBAL); // Ainda nao encontrei a doc do index
	_m["error_page"] = new ErrorPage(GLOBAL);
	_m["root"] = new Root(GLOBAL);
	_m["listen"] = new Listen(SERVER); // Do listen function
	_m["client_max_body_size"] = new ClientMaxBodySize(GLOBAL); //
	_m["limit_except"] = new LimitExcept(LOCATION);
	// _m["autoindex"] = new AutoIndex(LOCATION);

}

void	ServerBlock::parseFile()
{
	InfoConfig confInfo;

	for (size_t i = 0; _content[i]; i++)
	{
		while (_content[i] && !isalpha(_content[i]))
			i++;

		std::string value = slice_str(_content, SPACES, i);
		if (value.empty())
			break ;

		if (!confInfo.is_context(value) && !confInfo.is_directive(value))
			throw ValueNotFound();

		try {
			if (value == "location")
				_locations.push_back(new Location(get_context_block(_content, ++i)));
			else if (value == "limit_except")
				_m[value]->action(slice_str(_content, "}", ++i), SERVER);
			else
				_m[value]->action(slice_str(_content, _end, ++i), SERVER);
		} catch (const std::exception &e) {
			std::cout << "New error:" << e.what() << std::endl;
		}
	}
	dynamic_cast<Listen *>(_m["listen"])->check_dup_listen_directives();
}

const std::vector<Location*>	&ServerBlock::getLocation() const
{
	return (_locations);
}

bool	ServerBlock::getAutoIndex() const
{
	return (_autoIndex);
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
	_autoIndex = cpy._autoIndex;
	_m = cpy._m;
	_locations = cpy._locations;
	_content = cpy._content;
	_end = cpy._end;
	_separator = cpy._separator;
}
