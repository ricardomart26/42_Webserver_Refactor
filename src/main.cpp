#include <iostream>
#include <Config.hpp>
#include <Server.hpp>

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "ERROR: Bad Arguments" << std::endl;
		return (1);
	}

	std::vector <Server *> cluster;
	try
	{
		Config conf(av[1]);
		for (size_t i = 0; conf.getServerBlock(i) != NULL; i++)
		{
			cluster.push_back(new Server(conf.getServerBlock(i)));
		}
		std::cout << "\nServer Starting...\n";
		while (1)
		{
			for (size_t i = 0; i < cluster.size(); i++)
				cluster[i]->run();
		}
		for (size_t i = 0; i < cluster.size(); i++)
			delete cluster[i];
	} 
	catch (const std::exception &e) 
	{
		std::cout << e.what() << std::endl;
		for (size_t i = 0; i < cluster.size(); i++)
			delete cluster[i];
	}

	return (0);
}
