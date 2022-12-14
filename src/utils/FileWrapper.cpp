#include "FileWrapper.hpp"
#include <utils.hpp>

FileWrapper::FileWrapper(size_t size)
	: _fd(-1), _filename(), _size(size), _file_ext() {}

FileWrapper::FileWrapper(const std::string &filename, size_t size) 
	: _fd(-1), _filename(filename), _size(size), _file_ext(std::string(_filename, _filename.find_last_of('.') + 1)) {}

FileWrapper::FileWrapper(int fd, size_t size) 
	: _fd(fd), _filename(), _size(size), _file_ext() {} 

FileWrapper::~FileWrapper() {}

void	FileWrapper::open(const std::string &path)
{
	// std::cout << "Paths are: " << path << std::endl;
	_fd = ::open(path.c_str(), O_RDWR);
	if (_fd == -1)
		throw FileNotFound();

	size_t pos = path.find_last_of('/');
	_filename = std::string(path, pos + 1);
	if (_file_ext.empty())
		_file_ext = std::string(_filename, _filename.find_last_of('.') + 1);
}

void	FileWrapper::read()
{
	if (_fd < 0)
		throw FileNotFound();
	int valRead = 1;
	do {
		char *msg = new char[_size];
		memset(msg, 0, _size);
		valRead = ::read(_fd, msg, _size);
		if (valRead == -1)
			throw FileReadError();
		_content += std::string(msg, valRead);
		delete[] msg;
	}
	while ((unsigned int)valRead == _size);

	_content += '\0';
	if (_content.size() == 0)
		return ;
}

static int is_not_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
   int ret = S_ISDIR(path_stat.st_mode);
   return ret;
}

std::vector<std::string> FileWrapper::getDir(std::string dir)
{
    DIR                      *dp;
    std::vector<std::string> files;

    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir <<  std::endl;
        return files;
    }
    while ((dirp = readdir(dp)) != NULL)
	{
		if (is_not_regular_file(std::string(dir + dirp->d_name).c_str()))
	        files.push_back(std::string(dirp->d_name) + "/");
		else
	        files.push_back(std::string(dirp->d_name));

	}
    closedir(dp);
    return files;
}

std::string FileWrapper::getDirPage(std::string dir)
{
	std::vector<std::string> files = FileWrapper::getDir(dir);
	std::string page = "<!DOCTYPE html>\n<html>\n   <body>\n    <div>\n    <ul>\n";

	for (size_t i = 0; i < files.size(); i++)
		page += "   	<li><a href=" + files[i] +">" + files[i] +"</a></li>\n";	
	page += "    </ul>\n    </div>\n   </body>\n</html>\n\r\n";
	return (page); 
}

void				FileWrapper::closeFile() { close(_fd); _fd = -1; }
bool				FileWrapper::empty() const { return (_content.size() <= 1); }
const std::string	&FileWrapper::getContent() const { return (_content); }
const std::string	&FileWrapper::get_filename() const { return (_filename); }
std::string			&FileWrapper::get_file_ext() { return (_file_ext); }
int					FileWrapper::get_fd() const { return (_fd); }
size_t				FileWrapper::size() const { return (_content.size()); }
size_t				FileWrapper::get_max_size() const { return (_size); }

void				FileWrapper::set_fd(int fd)
{ 
	if (fd < 0) 
		throw FileNotFound();
	_fd = fd;
}

void				FileWrapper::set_content(const std::string &content) { _content = content; }
void				FileWrapper::set_file_ext(const std::string ext) { _file_ext = ext; }
