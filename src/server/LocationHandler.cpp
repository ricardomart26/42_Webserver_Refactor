#include "LocationHandler.hpp"

LocationHandler::LocationHandler(ServerBlock *sb, std::string &path)
{
	std::cout << "\n\t@LOCATIONHANDLER: \n";
	_sb = sb;
	_locationVec = _sb->getLocation();
	_location = NULL;
	_sbRoot = _sb->dir<Root>("root")->getValue();
	_path = path;
}

LocationHandler::~LocationHandler() {}

/**
 * @brief Init will compare url endpoint (e.g. https://[domain name]:[port]/[url endpoint]), and compare with 
*/
void	LocationHandler::findLocationBlock()
{
	size_t i = 0;
	for (; i < _locationVec.size(); i++)
	{
		if (!_locationVec[i]->getPrefix().compare(_path))
		{
			_location = _locationVec[i];
			std::cout << "\t\t@LOCATION FOUND: " <<  _location->getPrefix() << "\n";
			break ;
		}
	}
}

void	LocationHandler::findRoot()
{
	std::cout << "\n@DEBUG: " << _path << std::endl;
	// std::cout << _location->dir<Root>("root")->getValue() << std::endl;
	if (_location == NULL || _location->dir<Root>("root")->getValue().empty())
	{
		std::cout << _sbRoot << std::endl;
		if (_sbRoot.empty())
		{
			std::cout << "\n@DEBUG DON'T KNOW HOW TO HANDLE THIS 0\n\n";
			// Send error because it doesn't have a folder to look for files
			throw 404;
			// error_and_exit("LocationHandler::getLocationRoot root not found");
		}
		else
			_path = _sbRoot + _path;
	}
	else
		_path = _location->dir<Root>("root")->getValue() + _path;
}

bool	LocationHandler::checkIfDir()
{
	if (_path[_path.size() - 1] != '/')
		return false;

	const std::vector<std::string> indexVector = _sb->dir<Index>("index")->getValue(); 
	
	t_autoindex serverAutoIndex = _sb->getAutoIndex();
	t_autoindex locationAutoIndex = NOT_DEFINED;
	std::string	new_path;

	if (_location != NULL)
	{
		locationAutoIndex = _location->getAutoIndex();
		new_path = searchForPathWithIndex(_location->getIndex());
	}
	std::cout << "\t\t@PATH WITH INDEX: " <<  new_path << "\n"; 
	if (new_path.empty())
	{
		new_path = searchForPathWithIndex(indexVector);
		std::cout << "\t\t@PATH WITH INDEX: " <<  new_path << "\n"; 

		if (new_path.empty())
		{
			std::cout << "\nAUTOINDEX COOL\n";
			if (locationAutoIndex == TRUE || 
				(locationAutoIndex == NOT_DEFINED && serverAutoIndex == TRUE))
				return true;
			else
			{
				std::cout << "\n@DEBUG DON'T KNOW HOW TO HANDLE THIS 1\n\n";
				return false;
			}
		}
	}
	_path = new_path;
	return false;
}

std::string	LocationHandler::searchForPathWithIndex(const std::vector<std::string> &indexVector)
{
	for (size_t i = 0; i < indexVector.size(); i++)
	{
		std::string new_path = _path + indexVector[i];
		std::cout << "\t\t\t@TRY: " << new_path << std::endl;
		if (fileExists(new_path))
			return (new_path);
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

// bool	LocationHandler::getAutoIndex() const
// {
// 	return (_autoIndex);
// }
