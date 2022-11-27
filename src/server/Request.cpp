#include "Request.hpp"

/**
 *  When we receive a request from the browser:
 * 
 *  1. We read from the client socket, if it's empty, we close the connection.
 * 	2. Parse the request from the browser:
 * 	
 * 		
 * 		Request from browser ex. 
 * 			GET / HTTP/1.1
 * 			Host: localhost:4050
 * 			User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:105.0) Gecko/20100101 Firefox/105.0
 * 			Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp
 * 			Accept-Language: en-US,en;q=0.5
 * 			Accept-Encoding: gzip, deflate, br
 * 			Connection: keep-alive
 * 			Upgrade-Insecure-Requests: 1
 * 			Sec-Fetch-Dest: document
 * 			Sec-Fetch-Mode: navigate
 * 			Sec-Fetch-Site: none
 * 			Sec-Fetch-User: ?1
 * 			Host: localhost:4050
 * 
 * 		2.1 The first line is splited to the _requestLine variable, first arg is the method, the second, the path, and the third is the protocol version
 * 			2.1.1 First we need to see which serverblock this connection is inside.
 * 			2.1.2 The method, must be GET, POST or DELETE
 * 
 */

Request::Request(int client_fd, ServerBlock * sb)
	: _file(client_fd, _READ_SIZE), _sb(sb)
{
	_file.read();

	if (_file.size() <= 1)
		throw EmptyRequest();

	std::cout << "@REQUEST: \n\n";
	std::cout << _file.getContent() << std::endl;
	std::cout << "\n\nEND REQUEST@\n\n";

	ConvertHttpRequestToMap(_file.getContent());
	
	_requestLine = split(getMapValue("METHOD"), " ");
	_path = _requestLine[1];

	if (!getMethod().compare("GET"))
		_path = removeQuery();
}

Request::~Request() {}

void	Request::ConvertHttpRequestToMap(const std::string &msg)
{
	size_t j = msg.find_first_of('\n');
	_map["METHOD"] = msg.substr(0, j - 1);

	for (size_t i = ++j; msg[i] != '\0'; i++)
	{
		if (msg[i] == '\n')
		{
			if (i == j + 1)
				break;
			std::pair<string, string> v = splitHeaderAttribute(msg.substr(j, i - (j + 1)), ':');
			_map[v.first] = v.second;
			j = i + 1;
		}
	}

	for (; msg[j + 2] != '\0'; j++)
		_body.push_back(msg[j + 2]);
}

std::pair<std::string, std::string>	Request::splitHeaderAttribute(c_string &s, char sep)
{
	std::string key;
	std::string value;

	size_t char_pos = s.find_first_of(sep);
	key = s.substr(0, char_pos);
	value = s.substr(char_pos + 2, s.size() - (char_pos + 2));

	return (std::make_pair(key, value));
}

std::string	&Request::getMapValue(c_string value) { return (_map[value]); }
std::map<std::string, std::string>	&Request::getMap() { return (_map); }

std::string &Request::removeQuery()
{
	size_t questionPos = _path.find_last_of('?');

	// O nome do ficheiro pode ter um '?' por isso temos de verificar 
	// se esse ficheiro existe antes de assumir que pode ser uma query string	
	if (!fileExists(_path) && questionPos != string::npos)
	{
		_queryString = _path.substr(questionPos + 1);
		_path = _path.substr(0, questionPos);
	}
	return (_path);
}

std::string			&Request::getPath()  { return (_path); }
void				Request::setPath(const std::string &path) { _path = path; }

const std::string	&Request::getHttpVersion() const { return (_requestLine[2]); }
const std::string	&Request::getMethod() const { return (_requestLine[0]); }
const std::string	&Request::getHost() { return (getMapValue("Host")); }
const std::string	&Request::getConnectionType() { return (getMapValue("Connection")); }
const std::string	&Request::getBody() const { return (_body); } 
const std::string	&Request::getQueryString() const { return (_queryString); }
const std::string	&Request::getReferer() { return (getMapValue("Referer")); }
const std::string	&Request::getSecFetchDest() { return (getMapValue("Sec-Fetch-Dest")); }
const std::string	&Request::getContentType() { return (getMapValue("Content-Type")); }
const std::string	&Request::getLocationHeader() { return (getMapValue("Location")); }

std::map<std::string, std::string> &Request::getInfo() { return (getMap()); }

