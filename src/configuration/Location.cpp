#include "Location.hpp"

size_t	until_alpha(const std::string &str, size_t i)
{
	while (str[i] && !isalpha(str[i]))
		i++;
	return (i);
}

Location::Location(const std::string &block, std::map<std::string, Directives*> m) : _m(m)
{
	std::string end_delimiter;
	_prefix = trim(block.substr(0, block.find('{')), SPACES);
	_content = block.substr(block.find('{') + 1);

	std::cout << "\n\n\n@LOCATION BLOCK:\n\n";
	std::cout << "Prefix is: " << _prefix << std::endl;
	// std::cout << "content is: " << _content << std::endl;

	for (size_t i = 0; _content[i]; i = until_alpha(_content, i))
	{
		while (_content[i] && !isalpha(_content[i]) && _content[i] != '}')
			i++;
		std::string sliced = slice_str(_content, SPACES, i);
	
		if (sliced.empty())
			break ;
		end_delimiter = std::string(SPACES + std::string(";"));
		if (sliced == "limit_except")
			end_delimiter = "}";
		std::cout << "SEE THIS SLICED: " << sliced << std::endl;
		_m[sliced]->action(slice_str(_content, end_delimiter, ++i), LOCATION);
	}
}

Location::~Location() {}

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

const std::vector<std::string>	&Location::getIndex() const
{
	return (_index);
}

std::string Location::getRoot()
{
	Root *r = dynamic_cast<Root *>(_m[std::string("root")]);
	if (r == NULL)
		return ("");
	return (r->getValue());
}


