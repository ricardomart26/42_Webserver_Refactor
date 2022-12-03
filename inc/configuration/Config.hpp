#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "ServerBlock.hpp"
#include "FileWrapper.hpp"
#include "InfoConfig.hpp"
#include <string>
#include <cstdlib>

class ServerBlock;

class Config
{
	public:
		Config(const char *filename);
		~Config();


		ServerBlock					*getServerBlock(size_t i);
		std::vector<ServerBlock*>	&getServerBlockVec();
		std::vector<size_t>			getServerPorts();

		class ConfigCheckEndLine : public std::exception
		{
			const char *what() const throw() { return ("Check end line failed"); }
		};

		class ConfigCheckOpenings : public std::exception
		{
			const char *what() const throw() { return ("Check openings failed"); }
		};

	private:

		bool	is_context(int index);
		bool	is_directives(int index);
		void	remove_empty_new_lines();
		void	parse_server_block();
		bool	invalid_end_of_line();
		bool	invalid_brackets();

		std::string					_content;
		FileWrapper					_file;
		std::vector<ServerBlock *>	_server_block;
		InfoConfig					_confInfo;
};

#endif
