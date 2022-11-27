#include "Get.hpp"

Get::Get(Request *request, int clientFd, ServerBlock *_sb, size_t err) 
	:	Response(request, clientFd, err, _sb) {}

Get::~Get() {}

bool	Get::responseIsEmpty()
{
	return (_queue.is_empty());
}

void	Get::contructResponse(const Request &request)
{
	if (_queue.is_empty())
	{
		if (_status_code == 200)
			_reason_phrase = "OK";
		if (_status_code == 404 || _status_code == 405)
			_reason_phrase = "KO";

		_statusLine = request.getHttpVersion() + " " + ft_itoa(_status_code) + " " + _reason_phrase + "\r\n";

		_queue.add_data(_statusLine);
		_queue.add_data(_rh.get_header() + "\r\n");
		// std::cout << "\nheader " << _rh.get_header() << std::endl;
		_queue.add_data(_file.getContent() + "\r\n");
	}
	string response = _queue.get_data();
	// std::cout << "\n\n\nRESPONSE IS:\n\n" << response << "\n";
	send(_clientSocket, response.c_str(), response.size() - 1, 0);
}

void	Get::answer()
{
	string response = _queue.get_data();
	send(_clientSocket, response.c_str(), response.size(), 0);
}

