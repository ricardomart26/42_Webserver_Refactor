#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>

#include "utils.hpp"
#include "Directives.hpp"

class Location 
{
	public:

		Location(const std::string &block, std::map<std::string, Directives*> m);
		Location(const Location &cpy);
		const Location &operator=(const Location &rhs);
		~Location();

		template <typename T>
		T *dir(const std::string key)
		{
			return (dynamic_cast<T*>(_m[key]));
		}

		const std::string				&getPrefix() const;
		std::string						getRoot();
		const std::vector<std::string>	&getIndex() const;

		void							orderLocations();

	private:


		std::string							_prefix; // Path between "location" and "{"
		std::vector<std::string>			_index; // Path to test in front of prefix
		std::map<std::string, Directives*>	_m;
		std::string							_content;
};

#endif
