
#ifndef FILEWRAPPER_HPP
#define FILEWRAPPER_HPP

#include <cstddef>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <exception>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>


class FileWrapper {

	public:
		
		FileWrapper(size_t size);
		FileWrapper(const std::string &filename, size_t size);
		FileWrapper(int fd, size_t size); 
		~FileWrapper();

		void	read();
		void	readWithQueue();
		void	closeFile();
		void	open(const std::string &path);
		static std::vector<std::string> getDir (std::string dir);
		bool	empty() const;

		const std::string	&getContent() const;
		const std::string	&get_filename() const;
		std::string			&get_file_ext();
		int					get_fd() const;
		size_t				get_max_size() const;
		size_t				size() const;

		class FileNotFound : public std::exception
		{
				const char *what() const throw()
				{
					return ("File not Found");
				}
		};

		class FileReadError : public std::exception
		{
				const char *what() const throw()
				{
					return ("File read error");
				}
		};

		void				set_fd(int fd);
		void				set_file_ext(const std::string ext);
		void				set_content(const std::string &content);

	private:

		int			_fd;
		std::string	_filename;
		size_t		_size;
		std::string	_content;
		std::string	_file_ext;
};

#endif
