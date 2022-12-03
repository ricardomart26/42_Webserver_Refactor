#ifndef UTILS_HPP
#define UTILS_HPP

#include <fcntl.h>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>

#define SPACES "\n\r\t\v\f "


std::vector<std::string>	split(std::string str, std::string set = SPACES);
std::string					trim(const std::string &str, const char *set = SPACES);
std::string					get_context_block(const std::string cont, size_t &i);
std::string					slice_str(const std::string &cont, const std::string &separator, size_t &i);
size_t						count_char(const std::string &str, char c);
void						erase_while_is_equal(std::string &cont, char c);
bool						isEqual(char c, const std::string &str);
bool						isEqual(const std::string &dest, const std::vector<std::string> src);

template <typename T, typename U>
void	convertTo(T &src, U &dest)
{
	std::stringstream ss;

	ss << src;
	ss >> dest;
	ss.clear();
}

template <typename T, typename U>
T	convertTo(U &src)
{
	std::stringstream ss;
	T ret;

	ss << src;
	ss >> ret;
	return (ret);
}

char			**convert_vector(const std::vector<char *> &vec);
bool			fileExists(const std::string &path);
char			*substr(const char *src, int s, int e);
int				find_char(const char *msg, char c);
void			error_and_exit(const char *msg);
std::ifstream	openFile(const char *path);
char			*ft_itoa(int n);
std::string		getTime();
std::string		getRelative_path(std::string path);
std::string		remove_header(const std::string &content); 
int				ft_atoi(const char *str);

#endif
