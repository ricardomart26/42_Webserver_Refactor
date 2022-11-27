#ifndef POST_HPP
#define POST_HPP

#include "Response.hpp"

class Post : public Response 
{
	public:

		Post(Request *_request, int fd, ServerBlock *sb, size_t err = 200);
		~Post();

		bool	responseIsEmpty();
		void	contructResponse(const Request &request);
		void	answer();
};

#endif
