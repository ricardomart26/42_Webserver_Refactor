#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <cstddef>
#include <iostream>
#include <unistd.h>

#include "Request.hpp"
#include "ServerBlock.hpp"
#include "utils.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"

#define ERROR_PAGE "/Users/rimartin/Desktop/42_WebServer/web_files/error.html"

class Client
{
	public:

		Client(int fd, std::vector<ServerBlock *> sb);
		Client(const Client &cp);
		Client &operator=(const Client &rhs);
		~Client();

		bool	createResponse();
		bool	createRequest();
		int		getClientSocket() const;
		Request	*getRequest() const;

	private:

		bool		sendHttpError(unsigned short code);
		bool		isFinished();
		Response	*initResponse(int clientFd);
		
		int							_clientSocket;
		static size_t				_read_size;
		bool						_keep_alive;
		Request						*_request;
		Response					*_response;
		bool						_finished;
		std::vector<ServerBlock *>	_sbVector;
		ServerBlock *				_sb;

};

#endif
