#ifndef GET_HPP
#define GET_HPP

#include "Response.hpp"

class Get : public Response 
{
	public:

		Get(Request *_request, int fd, ServerBlock *sb, size_t err = 200);
		~Get();

		bool	responseIsEmpty();
		void	contructResponse(const Request &request);
		void	answer();
	
};

#endif
