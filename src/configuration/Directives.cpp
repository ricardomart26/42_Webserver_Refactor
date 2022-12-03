#include "Directives.hpp"

ServerName::ServerName(t_context context) : _value()
{
	_context = context;
}

void	ServerName::action(const std::string &value, t_context context)
{
	if (_context != GLOBAL)
	{
		if (_context != context)
			throw badContext();
	}
	std::vector<std::string> temp;
	temp = split(value);
	_value.insert(_value.end(), temp.begin(), temp.end());
}

void	ServerName::printContent() const
{
	std::cout << "\t@SERVER NAME:";
	for (size_t j = 0; j < _value.size(); j++)
		std::cout << " " << _value[j];
	std::cout << "\n";
}

const std::vector<std::string>	&ServerName::getValue() const
{
	return (_value);
}


std::string	ServerName::getValue(size_t i) const
{
	if (i > _value.size())
		return ("");
	return (_value[i]);
}

AutoIndex::AutoIndex(t_context context) : _value()
{
	_context = context;
	_value = NOT_DEFINED;
}

void	AutoIndex::action(const std::string &value, t_context context)
{
	if (_context != GLOBAL)
	{
		if (_context != context)
			throw badContext();
	}
	if (value == "on")
		_value = TRUE;
	else if (value == "off")
		_value = FALSE;
	else
		throw ValueNotAllowed();
}

void	AutoIndex::printContent() const
{
	std::string res = "NOT_DEFINED";
	if (_value == FALSE)
		res = "FALSE";
	else if (_value == TRUE)
		res = "TRUE";
	std::cout << "\t@AUTOINDEX: " << res << std::endl;
}

t_autoindex	AutoIndex::getValue() const
{
	return (_value);
}


Root::Root(t_context context) : _value()
{
	_context = context;
}

void	Root::action(const std::string &value, t_context context)
{
	if (_context != GLOBAL)
	{
		if (_context != context)
			throw badContext();
	}
	std::vector<std::string> temp;
	temp = split(value);
	if (temp.size() > 1)
		throw TooManyRoot();
	_value = temp[0];
}

void	Root::printContent() const
{
	std::cout << "\t@ROOT: " << _value << std::endl;
}

const std::string	&Root::getValue() const
{
	return (_value);
}

ClientMaxBodySize::ClientMaxBodySize(t_context context) : _value(1024) // 1mb?
{
	_context = context;
}

void	ClientMaxBodySize::action(const std::string &value, t_context context)
{
	if (_context != GLOBAL)
	{
		if (_context != context)
			throw badContext();
	}

	size_t pos = 0;

	while (pos < value.length())
	{
		if (isdigit(value[pos]))
			break ;
		pos++;
	}

	int len = pos;
	while (isdigit(value[len]))
		len++;

	_value = atoi(value.c_str());
}

void	ClientMaxBodySize::printContent() const
{
	std::cout << "\t@MAX_BODY_SIZE: " << _value << std::endl;
}

ErrorPage::ErrorPage(t_context context) : _value()
{
	_context = context;
}

/**
 * 
 * ERROR_PAGE SHOULD BE: 
 * 	 [pair<[500, 300, 200], "url">],
 *   [pair<[400, 300, 200], "url2">]]
 */
void	ErrorPage::action(const std::string &value, t_context context)
{
	if (_context != GLOBAL)
	{
		if (_context != context)
			throw badContext();
	}

	std::pair<std::vector<int>, std::string>	ret;
	std::stringstream							ss;
	size_t	i = 0;
	int		error;

	std::vector<std::string> splited = split(value, SPACES);

	if (splited.size() == 1)
		throw ErrorPageOnlyOneArg();

	for (; i < splited.size() - 1; i++)
	{
		for (size_t j = 0; j < splited[i].size(); j++)
		{
			if (!isdigit(splited[i][j]))
				throw ErrorPageCodeNotInt();
		}

		ss << splited[i];
		ss >> error;
		ss.clear();

		if (error < 100 || error > 599)
			throw ErrorPageCodeNotKnowed();

		ret.first.push_back(error);
	}
	ret.second = splited[i];
	_value.push_back(ret);
}

std::vector<std::pair<std::vector<int>, std::string> >	ErrorPage::getValue() const
{
	return (_value);
}

std::string ErrorPage::getErrorPath(int errorCode) const
{
	std::vector<std::pair<std::vector<int>, std::string> >::const_iterator it = _value.begin();
	for (; it != _value.end(); it++)
	{
		std::vector<int>::const_iterator errorIt = it->first.begin();
		for (; errorIt != it->first.end(); errorIt++)
		{
			if (*errorIt == errorCode)
				return (it->second);
		}
	}
	return ("");
}

void	ErrorPage::printContent() const
{
	std::cout << "\t@ERROR_PAGE: \n";
	for (size_t j = 0; j < _value.size(); j++)
	{
		std::cout << "\t\t@error number: ";
		for (size_t x = 0; x < _value[j].first.size(); x++)
			std::cout << _value[j].first[x] << " ";
		std::cout << "\n\t\t@error url - " << _value[j].second << std::endl;
	}
}

/**
 * 
 * Listen Examples:
 * 		
 * 		listen 127.0.0.1:8000;
 * 		listen 127.0.0.1;
 * 		listen 8000;
 * 		listen *:8000;
 * 		listen localhost:8000;
 */ 

Listen::Listen(t_context context) // Not done
{
	_context = context;
}

/**
 * Existem 3 possibilidades:
 * 
 * 1. 80
 * 2. :80
 * 
 * 3. 127.3.3.9:80
 * 
 * 1 e 2 é suposto ser a mesma coisa, mas não posso apagar o ';'  porque pode dar merda, neste caso 1127.28.9.2:80
 * 
 */
void	Listen::action(const std::string &value, t_context context)
{
	if (_context != GLOBAL)
	{
		if (_context != context)
			throw badContext();
	}
	if (split(value).size() > 1)
		throw std::exception();
	_indvListen.push_back(new ListenIndv(value));
}

void	Listen::check_dup_listen_directives()
{
	for (size_t i = 0; i < _indvListen.size(); i++)
	{
		const std::pair<std::string, int>	value = _indvListen[i]->getValue();
		for (size_t x = i + 1; x < _indvListen.size(); x++)
		{
			const std::pair<std::string, int>	value2 = _indvListen[x]->getValue();
			if (value.first == value2.first && value.second == value2.second) // QUESTION: Apagar ou mandar erro?
				this->_indvListen.erase(_indvListen.begin() + x);
				// throw ListenBlockEqual();
		}
	}
}

ListenIndv *Listen::getListenIndv(size_t i) const
{
	if (i >= _indvListen.size())
		return (NULL);
	return (_indvListen[i]);
}

void	Listen::printContent() const 
{
	for (size_t i = 0; i < _indvListen.size(); i++)
	{
		const std::pair<std::string, int> port_and_address = _indvListen[i]->getValue();
		std::cout << "\t@ADDRESS: " << port_and_address.first << ":" << port_and_address.second << "\n";
	}
}

ListenIndv::ListenIndv(const std::string &valueListen)
{
	std::string value = valueListen;
	size_t f_ret = value.find(':');

	_syntax = ADDRESS_PORT;
	// No caso 80 e :80
	if (f_ret == std::string::npos || f_ret == 0)
	{
		if (value.find('.') != std::string::npos)
			_syntax = ADDRESS;
		else
			_syntax = PORT;
	}

	if (_syntax == ADDRESS_PORT)
	{
		handleAddress(trim(value.substr(0, f_ret), SPACES));
		handlePort(value.substr(f_ret + 1));
	}
	else if (_syntax == ADDRESS)
	{
		_port = 80;
		handleAddress(trim(value, SPACES));
	}
	else
	{
		_address = "localhost";
		if (value[0] == ':')
			value.erase(value.begin());
		handlePort(value.c_str());
	}
	// std::cout << "Port is: " << _port << std::endl;
	// std::cout << "Address is: " << _address << std::endl;
	_value = std::make_pair(_address, _port);

}


void	ListenIndv::handleAddress(std::string addr)
{

	if (addr == "localhost" || addr == "127.0.0.1")
	{
		_address = "localhost";
		return ;
	}
	
	if (addr.find_last_not_of("0123456789.") != std::string::npos)
		throw ListenAddressNotValid();
	else if (count_char(addr, '.') != 3)
		throw ListenAddressNotValid();

	_address = addr;

	while (1)
	{
		int num = convertTo<int, std::string>(addr);
		addr.erase(0, convertTo<std::string>(num).size());

		if (num < 0 || num > 255)
			throw ListenAddressNotValid();
		
		if (addr.length() == 0)
			break ;

		addr.erase(0, 1);

		if (!isdigit(addr[0]))
			throw ListenAddressNotValid();
	}
}

void	ListenIndv::handlePort(const std::string &port)
{
	// std::cout << "port is: " << port << std::endl;
	if (port.find_last_not_of("0123456789") != std::string::npos)
		throw ListenPortNotValid();

	_port = convertTo<int>(port);
	if (_port < 0 || _port > 65535)
		throw ListenPortNotValid();
}

const std::pair<std::string, int>	&ListenIndv::getValue() const
{
	return (_value);
}

LimitExcept::LimitExcept(t_context context)
{
	_context = context;
	for (int i = 0; i < 3; i++)
		_value[i] = 0;
}

void	LimitExcept::action(const std::string &value, t_context context)
{
	// std::cout << "LimitExcept context is: " << _context << std::endl;
	if (_context != GLOBAL)
	{
		if (_context != context)
			throw badContext();
	}

	if (((int)value.find("GET") != -1))
		_value[0] = 2;
	if (((int)value.find("POST") != -1))
		_value[1] = 2;	
	if (((int)value.find("DELETE") != -1))
		_value[2] = 2;

	if (((int)value.find("deny") != -1))
	{
		for (int i = 0; i < 3; i++) 
		{ 
			if (_value[i] == 0)
				_value[i] = 1;
			
		}
	}
}

void	LimitExcept::printContent() const
{
	std::cout << "\t@LIMIT EXCEPT:  \n\t\tGET - " << _value[0] 
	<< "\n\t\tPOST - " << _value[1]
	<< "\n\t\tDELETE - " << _value[2] << std::endl;
}


Index::Index(t_context context) : _value()
{
	_context = context;
}

void	Index::action(const std::string &value, t_context context)
{
	if (_context != GLOBAL)
	{
		if (_context != context)
			throw badContext();
	}

	std::vector<std::string> temp;
	temp = split(value);
	_value.insert(_value.end(), temp.begin(), temp.end());
}

void	Index::printContent() const
{
	std::cout << "\t@Index:";
	for (size_t i = 0; i < _value.size(); i++)
		std::cout << " " << _value[i];
	std::cout << "\n";
}

const std::vector<std::string>	&Index::getValue() const
{
	return (_value);
}
