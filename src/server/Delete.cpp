#include "Delete.hpp"

Delete::Delete(Request *request, int clientFd,  ServerBlock *_sb, size_t err)
	:	Response(request, clientFd, err, _sb) {}

Delete::~Delete() {}

bool	Delete::responseIsEmpty() 
{
	return (_queue.is_empty());
}


void	Delete::contructResponse(const Request &request)
{
	std::string body = "{\"success\":\"true\"";
	if (_queue.is_empty())
	{
		if (_status_code == 200)
			_reason_phrase = "OK";
		if (_status_code == 404 || _status_code == 405)
			_reason_phrase = "KO";

		_statusLine = request.getHttpVersion() + " " +
		ft_itoa(_status_code) + " " + 
		_reason_phrase + "\r\n";

		_queue.add_data(_statusLine);
		if (!_rh.is_empty())
			_queue.add_data(_rh.get_header() + "\r\n");
		
		_queue.add_data(body);
		// _queue.add_data(_file.getContent() + "\r\n");
	}

	string response = _queue.get_data();
	int send_ret = send(_clientSocket, response.c_str(), response.size(), 0);
	(void)send_ret;
	// if (send_ret == -1)
	// 	throw 
}
