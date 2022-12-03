#ifndef LOCATIONHANDLER_HPP
#define LOCATIONHANDLER_HPP

#include "Location.hpp"
#include "ServerBlock.hpp"
#include "utils.hpp"
#include "FileWrapper.hpp"

class LocationHandler {

	public:
		
		LocationHandler(ServerBlock	*sb, std::string &path);
		~LocationHandler();

		void					findLocationBlock();
		void					findRoot();
		std::string				searchForPathWithIndex(const std::vector<std::string> &index_vec);
		bool					emptyLocation() const;
		bool					checkIfDir();

		std::vector<Location*>	getLocationVec() const;
		Location				*getLocation() const;
		std::string				&getPath();
		// bool					getAutoIndex() const;

	private:

		ServerBlock				*_sb;
		Location				*_location;
		std::string				_path;
		std::string				_sbRoot;
		std::vector<Location *>	_locationVec;
		// bool					_autoIndex;

};



#endif

