#include <iostream>
#include "Config.hpp"
#include "Server.hpp"

void	debug_function(ServerBlock *sb)
{
	std::cout << "\n@SERVER: \n\n";
	sb->dir<Listen>("listen")->printContent();
	sb->dir<ServerName>("server_name")->printContent();
	sb->dir<Root>("root")->printContent();
	sb->dir<ErrorPage>("error_page")->printContent();
	sb->dir<ClientMaxBodySize>("client_max_body_size")->printContent();
	sb->dir<LimitExcept>("limit_except")->printContent();
	sb->dir<Index>("index")->printContent();
	sb->dir<AutoIndex>("autoindex")->printContent();
	std::cout << "\n";
	std::vector<Location *> locationVector = sb->getLocation();
	for (size_t i = 0; i < locationVector.size(); i++)
	{
		std::cout << "\n@LOCATION " << locationVector[i]->getPrefix() << ": \n\n";
		locationVector[i]->dir<Listen>("listen")->printContent();
		locationVector[i]->dir<ServerName>("server_name")->printContent();
		locationVector[i]->dir<Root>("root")->printContent();
		locationVector[i]->dir<ErrorPage>("error_page")->printContent();
		locationVector[i]->dir<ClientMaxBodySize>("client_max_body_size")->printContent();
		locationVector[i]->dir<LimitExcept>("limit_except")->printContent();
		locationVector[i]->dir<Index>("index")->printContent();
		locationVector[i]->dir<AutoIndex>("autoindex")->printContent();
		// 	Index(GLOBAL);
		// 	ErrorPage(GLOBAL);
		// 	Root(GLOBAL);
		// 	ClientMaxBodySize(GLOBAL);
		// 	LimitExcept(LOCATION);
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "ERROR: Bad Arguments" << std::endl;
		return (1);
	}

	std::vector <Server *> cluster;
	Config *conf = NULL;
	try
	{
		conf = new Config(av[1]);
		if (conf->getServerBlock(0) == NULL)
			throw std::exception();
	} catch (const std::exception &e) {
			std::cout << e.what() << std::endl;
			delete conf;
			return (1);
	}
	try {
		std::vector<size_t> ports = conf->getServerPorts();

		ServerBlock *serverBlock = NULL;
		for (size_t i = 0; (serverBlock = conf->getServerBlock(i)) != NULL; i++)
			debug_function(serverBlock);

		for (size_t i = 0; i < ports.size(); i++)
			cluster.push_back(new Server(ports[i], conf->getServerBlockVec()));

		std::cout << "\nServer Starting...\n";
		while (1)
		{
			for (size_t i = 0; i < cluster.size(); i++)
				cluster[i]->run();
		}
		for (size_t i = 0; i < cluster.size(); i++)
			delete cluster[i];

	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		for (size_t i = 0; i < cluster.size(); i++)
			delete cluster[i];
		return (1);
	}

	return (0);
}
