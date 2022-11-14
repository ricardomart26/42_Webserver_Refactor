#include <iostream>
#include <sys/stat.h>
#include "FileWrapper.hpp"


bool	fileExists2(const std::string &path)
{
	struct stat	buffer;
	return (stat(path.c_str(), &buffer) == 0);
}


int main2(void)
{

	std::string dir = FileWrapper::getDirPage("/sgoinfre/edos-san/42_Webserver_Refactor/inc/configuration");

		std::cout << dir << std::endl;
	
	/*std::string filename = "maino.cpp";
	if (fileExists2(filename))
		std::cout << "Existe\n";
	else
		std::cout << "Não existe\n";*/
	return (0);
}
