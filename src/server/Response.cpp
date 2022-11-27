#include "Response.hpp"

#define DOCUMENT_ROOT "DOCUMENT_ROOT=/home/ricardo/Desktop/42/42_projects/42_WebServer/web_files/"
#define REDIRECT_STATUS "REDIRECT_STATUS=200" // Ou CGI??

Response::Response(Request *request, int client_socket, size_t status_code, ServerBlock *sb) 
	: _clientSocket(client_socket), _queue(_WRITE_SIZE), _file(_WRITE_SIZE), _status_code(status_code), _sb(sb), _path(request->getPath())
{
	
	try {
		_locationHandler = new LocationHandler(_sb, _path);
		_locationHandler->findLocationBlock();
		_path = _locationHandler->findRoot();
		
		// No caso do url acabar com um '/'
		_locationHandler->checkIfDir();

		// _path = _locationHandler->getPath();
		std::cout << "Final path: " << _path << "\n";
		_file.open(_path);
	} catch (int err) {
		std::cout << "error is: " << err << std::endl;
		_status_code = (size_t)err;
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
		_status_code = 404;
	}

	// std::cout << "Status code is: " << _status_code << std::endl;
	if (_status_code == 405 || _status_code == 404)
	{
		std::string errorPath = _sb->dir<ErrorPage>("error_page")->getErrorPath(status_code);
		if (errorPath.empty())
		{
			std::string content;
			content = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\t<meta charset=\"UTF-8\">\n\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\t<title>Document</title>\n</head>";
			content += "\n<body>\n\t<h1>" + std::string(ft_itoa(_status_code)) + "</h1>\n</body>\n</html>";
			_file.set_content(content);
			_file.set_file_ext("html");
		}
		else
			_file.open(errorPath);
	}

	if (_file.getContent().empty())
	{
		_file.read();
		_file.closeFile();
	}

	if (_file.get_file_ext() == "php")
	{
		_file.set_fd(handlePhp(request));
		ConvertHttpRequestToMap(_file.getContent());

		std::vector<std::string> _request_line = split(_map["METHOD"], " ");

		if (!_request_line[1].empty())
			_status_code = ft_atoi(_request_line[1].c_str());
		if (!_request_line[2].empty())
			_reason_phrase = _request_line[2];

		std::string redirection = _map["Location"];

		if (!redirection.empty())
			_rh.add_response_header("Location: " + redirection + "\r\n");
		_file.set_content(remove_header(_file.getContent()));
	}
	std::string ext = "html";
	_rh.set_entity_header(ext, _file.getContent().size(), request->getSecFetchDest());
	_rh.set_general_header();
	_rh.set_response_header();
}


void	Response::ConvertHttpRequestToMap(const std::string &msg)
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
	if (msg[j + 1] == '\0')
		return ;
	for (; msg[j + 2] != '\0'; j++)
		_body.push_back(msg[j + 2]);
}

std::pair<std::string, std::string>	Response::splitHeaderAttribute(const std::string &s, char sep)
{
	size_t char_pos = s.find_first_of(sep);
	return (std::make_pair(s.substr(0, char_pos), 
		s.substr(char_pos + 2, s.size() - (char_pos + 2))));
}

bool	Response::finished()
{ 
	return (_queue.is_empty());
}

int	Response::handlePhp(Request *request)
{
	int pfd1[2];
	int pfd2[2];
	
	if (pipe(pfd1) == -1 || pipe(pfd2) == -1)
		error_and_exit("Pipe error in Response::handlePHP");

	pid_t pid = fork();
	if (pid == -1)
		error_and_exit("Fork error in Response::handlePHP");

	if (pid == 0)
	{
		char **arg = new char*[4];

		arg[0] = strdup("php-cgi");
		arg[1] = strdup("-q");
		arg[2] = strdup(request->getPath().c_str());
		arg[3] = NULL;

		char **e = new char*[10];

		e[0] = strdup(string("SCRIPT_FILENAME=" + getRelative_path(request->getPath())).c_str());
		e[1] = strdup("REDIRECT_STATUS=200");
		e[2] = strdup(string("CONTENT_LENGTH=" + string(ft_itoa((int)request->getBody().size()))).c_str());
		e[3] = strdup(string("SERVER_PROTOCOL=" + request->getHttpVersion()).c_str());
		e[4] = strdup(string("QUERY_STRING=" + request->getQueryString()).c_str());
		e[5] = strdup(string("PATH_INFO=" + string(request->getPath())).c_str());
		e[6] = strdup(string("REQUEST_METHOD=" + request->getMethod()).c_str());
		e[7] = strdup(string("CONTENT_TYPE=" + request->getContentType()).c_str());
		e[8] = strdup("GATEWAY_INTERFACE=CGI/1.1");
		e[9] = NULL;

		close(pfd1[1]);
		dup2(pfd1[0], STDIN_FILENO);
		close(pfd1[0]);

		close(pfd2[0]);
		dup2(pfd2[1], STDOUT_FILENO);
		close(pfd2[1]);

		execve("/usr/bin/php-cgi", arg, e);	
		error_and_exit("Execve error ");
	}
	else
	{
		close(pfd2[1]);
		int write_ret = write(pfd1[1], request->getBody().c_str(), request->getBody().size());
		if (write_ret == -1)
		{
			kill(pid, SIGTERM);
			return (-1);
		}
		close(pfd1[1]);
		close(pfd1[0]);
		waitpid(pid, NULL, 0);
	}
	
	return (pfd2[0]);
}
