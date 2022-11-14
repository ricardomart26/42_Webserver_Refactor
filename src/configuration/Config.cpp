#include <Config.hpp>

// PARSING DONT WORK IF THERE IS NOT A NEW LINE IN THE END OF THE FILE....

/** @brief Receives a configuration file, see if file exists, 
 * and parse the file to pass information to the server */
Config::Config(const char *filename)
: _file(200)
{
	_file.open(filename);
	_file.read();
	_content = _file.getContent();

	remove_empty_new_lines();

	if (invalid_end_of_line())
		throw ConfigCheckEndLine();
	if (invalid_brackets())
		throw ConfigCheckOpenings();
	parse_server_block();
}

ServerBlock	*Config::getServerBlock(size_t i)
{
	if (i >= _server_block.size())
		return (NULL);
	return (_server_block[i]);
}

std::vector<ServerBlock*>	&Config::getServerBlockVec()
{
	return (_server_block);
}

/** 
 * @brief Removes every empty new line with or without spaces.
*/
void	Config::remove_empty_new_lines()
{
	erase_while_is_equal(_content, '\n');
	int index = _content.length();
	while (index > 0)
	{
		int exist = 0;
		while (index > 0 && _content[index] != '\n')
			index--;
		while (index > 0 && _content[--index] != '\n')
		{
			if (!isspace(_content[index]))
				exist++;
		}
		if (!exist)
			_content.erase(index, 1);
	}
}

/**
 * @brief Finds every server block in the config file, and constructs every serverBlock class for each server context found
 * and addes them to a vector.
*/
void	Config::parse_server_block()
{
	size_t i = 0;
	int pos = 0;

	while (i < _content.length() && (pos = _content.find("server", i)) != -1)
	{
		// Find returns the position of the beginning of the word, so we need to advance the "server" word
		pos += 6;
		while (!isalpha(_content[pos]))
			pos++;

		i = pos;
		for (size_t counter = 1; counter && _content[i]; i++) 
		{
			if (_content[i] == '{')
				counter++;
			else if (_content[i] == '}')
				counter--;
		}
		_server_block.push_back(new ServerBlock(_content.substr(pos, i - pos - 1)));
		_server_block[_server_block.size() - 1]->parseFile();
	}
}

/**
 * @brief checks if end of line finishes with ';' or '{' and if the directive 
 * or context exists!
*/
bool	Config::invalid_end_of_line()
{
	int index = _content.length() - 1;
	while (index > -1)
	{
		if (_content[index] == '\n')
		{
			while (index > 0 && isspace(_content[index])) 
				index--;

			if (_content[index] == ';')
			{
				if (!is_directives(index - 1))
					return (true);
			}
			else if (_content[index] == '{')
			{
				if (!is_context(index - 1))
					return (true);
				index--;
			}
			continue ;
		}
		index--;
	}
	return (false);
}


/**
 * @brief when it gets a '{':
 * 	1. Goes until ';' or begining of the file
 * 	2. Substring of word,  
 * 	3. Checks if the word is one of the words inside context (including limit_except).
*/
bool	Config::is_context(int index)
{
	while (index > 0 && _content[index] != ';')
		index--;

	while (_content[index] && !isalpha(_content[index]))
		index++;

	int save_index = index;

	while (_content[save_index] && !isspace(_content[save_index]))
		save_index++;

	std::string sub = _content.substr(index, save_index - index);

	if (_confInfo.is_context(sub))
		return (1);

	if (!sub.compare("GET") || !sub.compare("POST") || !sub.compare("DELETE"))
		return (is_context(index - 1));

	return (0);
}

/**
 * @brief when it gets a ';' goes until ';' or begining of the file or '{' or '}', after that it goes to the next word, 
 * and sees if the word is one of the words inside the directives (excluding limit_except).
*/
bool	Config::is_directives(int index)
{
	while (index - 1 > 0 && (_content[index - 1] != ';' && 
	_content[index - 1] != '{' && _content[index - 1] != '}'))
		index--;

	if (index - 1 == 0)
		return (false);

	while (_content[index] && (!isprint(_content[index]) || isspace(_content[index])))
		index++;

	int save_index = index;
	while (isalpha(_content[index]) || _content[index] == '_') 
		index++;

	if (_confInfo.is_directive(_content.substr(save_index, index - save_index)))
		return (true);

	return (false);
}


/**
 * @brief 
*/
bool	Config::invalid_brackets()
{
	int open = 0;
	int close = 0;
	int index = 0;

	while (_content[index])
	{
		if (_content[index] == '{')
			open++;
		else if (_content[index] == '}')
		{
			if (open <= close)
				return (true);
			close++;
		}
		index++;
	}
	if (open != close)
		return (true);
	return (false);
}

Config::~Config() 
{
	for (size_t i = 0; i < _server_block.size(); i++)
		delete _server_block[i];
}

