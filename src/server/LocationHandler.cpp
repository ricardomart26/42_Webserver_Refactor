#include "LocationHandler.hpp"

LocationHandler::LocationHandler(ServerBlock *sb, std::string &path)
{
	_sb = sb;
	_locationVec = _sb->getLocation();
	_location = NULL;

	_path = path;
}

LocationHandler::~LocationHandler() {}

/**
 * @brief Init will compare url endpoint (e.g. https://[domain name]:[port]/[url endpoint]), and compare with 
*/
void	LocationHandler::findLocationBlock()
{
	std::cout << "\n@FIND LOCATION BLOCK\n " << std::endl;

	std::cout << "Path is: " << _path << std::endl;
	// Search in location prefix for the best match for request path
	std::cout << "locationVec size is: " << _locationVec.size() << std::endl;
	size_t i = 0;
	for (; i < _locationVec.size(); i++)
	{
		std::cout << "Searching in: " << _locationVec[i]->getPrefix() << std::endl;
		std::cout << "Root Searching in: " << _locationVec[i]->getRoot() << std::endl;

		// std::cout << "Options are: " << _locationVec[i]->dir<Root>("root")->getValue() << std::endl;
		if (!_locationVec[i]->getPrefix().compare(_path))
		{
			_location = _locationVec[i];
			// std::cout << "\nLocation found: " <<  _location->getPrefix() << "\n\n";
			return ;
		}
	}
	throw 404;
	// What happens if nginx doest find the location
	// sendHttpError(404);
}

std::string	LocationHandler::findRoot()
{
	// std::cout << _location->dir<Root>("root")->getValue() << std::endl;
	if (_location->dir<Root>("root")->getValue().empty())
	{
		if (_sbRoot.empty())
		{
			// Send error because it doesn't have a folder to look for files
			error_and_exit("LocationHandler::getLocationRoot root not found");
		} 
		return (_sbRoot + _path); 
	}
	else
	{
		std::cout << "\n\t@LOCATION ROOT: " << _location->dir<Root>("root")->getValue() << "\n\n";
		std::cout << "\n\t@FINAL PATH: " << _path << "\n\n";
		return (_location->dir<Root>("root")->getValue() + _path);
	}
}

void	LocationHandler::checkIfDir()
{
	if (_path[_path.size() - 1] != '/')
		return ;

	const std::vector<std::string> indexVec = _sb->dir<Index>("index")->getValue(); 
	bool autoIndex = _sb->getAutoIndex();
	std::string	new_path;

	new_path = searchForPathWithIndex(_location->getIndex());
	if (new_path.empty())
	{
		new_path = searchForPathWithIndex(indexVec);
		if (new_path.empty())
		{
			if (autoIndex == true)
			{
				// Do autoindex page
			}
			else
			{
				// throw 404;
				// Return 404 page not found
				// EXIT HERE
			}
		}
	}
	_path = new_path;
}

std::string	LocationHandler::searchForPathWithIndex(const std::vector<std::string> &indexVec)
{
	for (size_t i = 0; i < indexVec.size(); i++)
	{
		if (fileExists(_path + indexVec[i]))
			return (_path += indexVec[i]);
	}
	return ("");
}

bool	LocationHandler::emptyLocation() const
{
	return (!_locationVec.size());
}

std::string	&LocationHandler::getPath()
{
	return (_path);
}
