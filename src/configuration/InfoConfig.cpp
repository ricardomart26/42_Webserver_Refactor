#include "InfoConfig.hpp"

std::vector<std::string> InfoConfig::_directives = std::vector<std::string>();
std::vector<std::string> InfoConfig::_contexts = std::vector<std::string>();

InfoConfig::InfoConfig() 
{
	if (_directives.empty())
	{
		std::string dir_arr[] = {"listen", "server_name", "root", "index", "client_max_body_size", "error_page", "autoindex", "limit_except"};
		for (size_t i = 0; i < (sizeof(dir_arr) / sizeof(std::string)); i++)
			_directives.push_back(dir_arr[i]);
	}

	if (_contexts.empty())
	{
		std::string con_arr[] = {"http", "server", "location"};
		for (size_t i = 0; i < (sizeof(con_arr) / sizeof(std::string)); i++)
			_contexts.push_back(con_arr[i]);
	}
}

InfoConfig::~InfoConfig()
{

}

bool	InfoConfig::is_directive(const std::string &cmp) const
{
	for (size_t i = 0; i < _directives.size(); i++)
	{
		if (!_directives[i].compare(cmp))
			return (true);
	}
	return (false);
}

bool	InfoConfig::is_context(const std::string &cmp) const
{
	for (size_t i = 0; i < _contexts.size(); i++)
	{
		if (!_contexts[i].compare(cmp))
			return (true);
	}
	return (false);
}
