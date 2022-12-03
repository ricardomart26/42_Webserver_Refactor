#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <utility>
#include <stdlib.h>

#include "utils.hpp"

typedef enum s_context {
	GLOBAL,
	LOCATION,
	SERVER
} t_context;

typedef enum s_option {
	OVERRIDE,
	APPEND
} t_option;

class Directives 
{
	public:

		virtual ~Directives() {}
		virtual void	action(const std::string &value, t_context context) = 0;
		virtual void	printContent() const = 0;

		class badContext : public std::exception
		{
			const char *what() const throw()
			{
				return ("A directive is in wrong context");
			}
		};

	protected:

		t_context	_context;
};

class ServerName : public Directives
{
	public:

		ServerName(t_context context);
		~ServerName() {}

		void				action(const std::string &value, t_context context);
		void				printContent() const;
		std::string			getValue(size_t i) const;
		const std::vector<std::string>	&getValue() const;

	private:

		std::vector<std::string>	_value;

};

typedef enum s_autoindex {
	NOT_DEFINED,
	FALSE,
	TRUE
} t_autoindex;

class AutoIndex : public Directives
{
	public:

		AutoIndex(t_context context);
		~AutoIndex() {}

		void		action(const std::string &value, t_context context);
		void		printContent() const;
		t_autoindex	getValue() const;

		class ValueNotAllowed : public std::exception
		{
			const char *what() const throw()
			{
				return ("AutoIndex value not allowed (should be on/off)");
			}
		};

	private:

		t_autoindex	_value;

};

class Root : public Directives
{
	public:

		Root(t_context context);
		~Root() {}

		void	action(const std::string &value, t_context context);
		void	printContent() const;
		const std::string	&getValue() const;

		class TooManyRoot : public std::exception
		{
			const char *what() const throw()
			{
				return ("More than 1 root");
			}
		};

	private:

		std::string	_value;

};

class ClientMaxBodySize : public Directives
{
	public:

		ClientMaxBodySize(t_context context);
		~ClientMaxBodySize() {}

		void	action(const std::string &value, t_context context);
		void	printContent() const;

	private:

		int	_value;

};

class ErrorPage : public Directives
{
	public:

		ErrorPage(t_context context);
		~ErrorPage() {}

		void	action(const std::string &value, t_context context);
		void	printContent() const;
		std::vector<std::pair<std::vector<int>, std::string> >	getValue() const;
		std::string	getErrorPath(int error) const;

		class ErrorPageCodeNotInt : public std::exception
		{
			const char *what() const throw() 
			{
				return ("error_page directive, has letters in int code");
			}
		};

		class ErrorPageCodeNotKnowed : public std::exception
		{
			const char *what() const throw() 
			{
				return ("error_page directive, int code not exist's should be between 100-599");
			}
		};

		class ErrorPageOnlyOneArg : public std::exception
		{
			const char *what() const throw()
			{
				return ("error_page directive, Should have more than one arg");
			}
		};


	private:

		std::vector<std::pair<std::vector<int>, std::string> > _value;

};

typedef enum s_syntax {
	NONE,
	ADDRESS,
	ADDRESS_PORT,
	PORT
} e_syntax;

class ListenIndv 
{
	public:

		ListenIndv(const std::string &value);
		~ListenIndv() {}

		void	handleAddress(std::string addr);
		void	handlePort(const std::string &port);

		class ListenAddressNotValid : public std::exception
		{
			const char *what() const throw()
			{
				return ("Listen address not valid"); 
			}
		};


		class ListenPortNotValid : public std::exception
		{
			const char *what() const throw()
			{
				return ("Listen port not valid"); 
			}
		};

		const std::pair<std::string, int>	&getValue() const;

	private:

		std::pair<std::string, int>	_value;
		std::string					_address;
		int							_port;
		e_syntax					_syntax;
		static const std::vector<std::string>	_options;
};

class Listen : public Directives
{
	public:
		Listen(t_context context);
		
		~Listen()
		{
			std::vector<ListenIndv*>::iterator it = _indvListen.begin();
			for (; it !=_indvListen.end(); it++)
				delete *it;
		}

		class ListenBlockEqual : public std::exception
		{
			const char *what() const throw()
			{
				return ("Two listens with port and address equal"); 
			}
		};

		void		action(const std::string &value, t_context context);
		void		printContent() const;
		ListenIndv	*getListenIndv(size_t i) const;
		void		check_dup_listen_directives();

	private:

		std::vector<ListenIndv*>	_indvListen;
		t_context					_context;
};


class LimitExcept : public Directives
{
	public:

		LimitExcept(t_context context);
		~LimitExcept() {}

		void	action(const std::string &value, t_context context);
		void	printContent() const;

	private:

		int	_value[3];

};

class Index : public Directives
{
	public:

		Index(t_context context);
		~Index() {}

		void	action(const std::string &value, t_context context);
		void	printContent() const;
		const std::vector<std::string>	&getValue() const;

	private:

		std::vector<std::string>	_value;

};

#endif
