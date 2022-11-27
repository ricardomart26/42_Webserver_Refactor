#include "EntityHeader.hpp"

EntityHeader::EntityHeader(string  &file_ext, size_t len, c_string &type)
{
	// Content-Type: text/html
	string ctype = "text";
	std::cout << "File extension: " << file_ext << std::endl;

	if (!type.compare("image") || !file_ext.compare("jpeg"))
		ctype = "image";
	else if (file_ext == "js")
	{
		ctype = "application";
		file_ext = "javascript";
	}


	_content_type = "Content-Type: " + ctype + "/" + file_ext + "\r\n";
	_content_len = "Content-Length: " + string(ft_itoa(len)) + "\r\n";

	// Content-Encoding: gzip
	// https://developer.mozilla.org/pt-BR/docs/Web/HTTP/Headers/Content-Encoding
	// Last-Modified: Date
}

EntityHeader::~EntityHeader() {}

std::string	EntityHeader::get_entity_header() 
{ 
	return (_content_type + _content_len);
}
