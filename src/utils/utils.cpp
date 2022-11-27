#include <utils.hpp>
#include <fstream>
#include <limits.h>


static int	find_closest(const std::string &str, std::string set, const int pos)
{
	size_t closest = -1;
	while (set.length())
	{
		closest = str.find(set[0], pos);
		if (closest != std::string::npos)
			break;
		set.erase(0, 1);
	}

	if (set.length() == 0)
		return str.length();
	
	int x = 1;
	while (set[x])
	{
		size_t find_ret = str.find(set[x], pos);
		if (find_ret == std::string::npos)
			set.erase(x, 1);
		else
		{
			if (find_ret < closest)
				closest = find_ret;
			x++;
		}
	}
	return (closest);
}

bool	isEqual(char c, const std::string &str)
{
	return (str.find_first_of(c) != std::string::npos ? true : false);
}

bool	isEqual(const std::string &dest, const std::vector<std::string> src)
{
	for (size_t i = 0; i < src.size(); i++)
	{
		if (dest == src[i])
			return (true);
	}
	return (false);
}


std::vector<std::string>	split(std::string str, std::string set)
{
	for (size_t i = 0; str[i]; i++)
	{
		size_t len = 0;
		while (isEqual(str[len + i], set)) 
			len++;
		if (len > 1)
			str.erase(i, len - 1);
	}
	
	size_t char_amount = 0;
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++)
	{
		if (isEqual(*it, set))
			char_amount++;
		while (isEqual(*it, set))
			*it++;
	}

	std::vector<std::string> ret;
	for (size_t i = 0; ret.size() < char_amount + 1; i = find_closest(str, set, i) + 1)
		ret.push_back(str.substr(i, find_closest(str, set, i) - i));

	return (ret);
}

// void		trim(std::string &str, const char *set)
// {
// 	size_t i = 0;
// 	while (str[i] && isEqual(str[i], set)) 
// 		i++;
// 	str.erase(0, i);

// 	i = str.length() - 1;
// 	while (str[i] && isEqual(str[i], set))
// 		i--;
// 	str.erase(i + 1, (str.length() - 1) - i);
// }

std::string		trim(const std::string &str, const char *set)
{
	std::string ret = str;
	size_t i = 0;
	while (ret[i] && isEqual(ret[i], set)) 
		i++;
	ret.erase(0, i);

	i = ret.length() - 1;
	while (ret[i] && isEqual(ret[i], set))
		i--;
	ret.erase(i + 1, (ret.length() - 1) - i);
	return (ret);
}

void	erase_while_is_equal(std::string &cont, char c)
{
	int len = 0;

	while (cont[len] == c)
		len++;
	cont.erase(0, len);
}

/**
 *	@brief Returns a string while it isn't one of the delimiters
 *	and increments i
 *	@example 1 
 * 
 * 		@param cont - Listen: address:port
 * 		@param del - "!:{"
 * 		@param &i - 0
 * 
 * 		@return - Listen && i = 6
 * 	
 * 	@example 2
 * 
 * 		@param cont - Random String
 * 		@param del - "m n"
 * 		@param &i - 0
 * 
 * 		@return - Ra && i = 2
 */
std::string	slice_str(const std::string &cont, const std::string &del, size_t &i)
{
	size_t save_i = i;
	//std::cout << "Check this one: " << cont << std::endl;
	while (i < cont.length() && !isEqual(cont[i], del))
	{
		// std::cout << cont[i] << std::endl;
		i++;
	}
	//std::cout << "i " << i << " vs save_i " << save_i <<"\n";
	
	if (i == save_i)
		return (std::string());
	// std::cout <<  i - save_i << std::endl;
	// std::cout <<  i << std::endl;
	// std::cout << "Answer is: " << cont.substr(save_i, i - save_i) << std::endl;
	return (trim(cont.substr(save_i, i - save_i), SPACES));
}

std::string	get_context_block(const std::string cont, size_t &i)
{
	size_t open = 0;
	size_t close = 0;

	size_t save_i = i;
	while (i < cont.length())
	{
		if (cont[i] == '{')
			open++;
		else if (cont[i] == '}')
			close++;
		if (open == close && open != 0)
			break;
		i++;
	}

	std::string ret = cont.substr(save_i, i - save_i + 1);
	return (trim(ret, SPACES));
}

size_t count_char(const std::string &str, char c)
{
	size_t ret = 0;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == c)
			ret++;
	}
	return (ret);
}

char	**convert_vector(const std::vector<char *> &vec)
{
	char	**ret;
	size_t i = 0;

	ret = new char*[vec.size() + 1];

	for (; i < vec.size(); i++)
	{
		ret[i] = vec[i];
		std::cerr << "Environment var: " << vec[i] << std::endl;
	}

	ret[i] = NULL;
	return (ret);
}

bool	fileExists(const std::string &path)
{
	struct stat	buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

static int	length(int n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n *= -1;
		len++;
	}
	while (n > 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*s;
	int		size;

	if (n == -2147483648)
		return (strdup("-2147483648"));
	size = length(n);
	s = new char[size + 1];
	if (!s)
		return (0);
	s[size] = '\0';
	if (n == 0)
		s[0] = '0';
	else if (n < 0)
	{
		s[0] = '-';
		n *= -1;
	}
	while (n > 0)
	{
		s[size - 1] = (n % 10) + '0';
		n /= 10;
		size--;
	}
	return (s);
}

char* substr(const char *src, int s, int e)
{
    int len = e - s;
    char *dest = new char[len + 1];
    for (int i = s; i < e && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
}

int find_char(const char *msg, char c)
{
    for (int ret = 0; msg[ret] != '\0'; ret++)
    {
        if (msg[ret] == c)
            return (ret);
    }
    return (-1);
}

void    error_and_exit(const char *msg)
{
    perror(msg);
    exit(0);
}

std::string getTime()
{
	time_t now = time(0);
	char temp[40];
	struct tm *now_conv = gmtime(&now);
	now_conv->tm_hour += 1;
	size_t size = strftime(temp, 40, "%a, %d %b %Y %H:%M:%S", now_conv);
	temp[size] = '\0';
	return ("Date: " + std::string(temp) + " GMT\r\n");
}

std::string getRelative_path(std::string path)
{
	char buffer[1024];
	memset(buffer, '\0', 1024);
	getcwd(buffer, 1024);

	size_t i = 0;
	for (; path[i] == buffer[i]; i++)
		i++;
	return (path.substr(i));
}

std::string	remove_header(const std::string &content) 
{
	size_t	i = 0;
	bool	helper = false;

	while (content[i])
	{
		if (content[i] == '\r' && content[i + 1] == '\n')
		{
			if (helper)
				return (content.substr(i + 2));
			i++;
			helper = true;
		}
		i++;
	}
	return (content);
}


int	ft_atoi(const char *str)
{
	int		sign = 1;
	long	res = 0;

	while (*str == '\t' || *str == '\n' || *str == '\v'
		|| *str == '\f' || *str == '\r' || *str == ' ')
		str++;
	if (*str == '-')
	{
		sign *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while ('0' <= *str && *str <= '9')
	{
		res *= 10;
		res += (sign * (*(str++) - '0'));
		if (res > INT_MAX || res < INT_MIN)
			error_and_exit("Atoi: number bigger than int max or smaller than int min");
	}
	return (res);
}
