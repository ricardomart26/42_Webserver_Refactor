#ifndef DELETE_HPP
#define DELETE_HPP

#include "Response.hpp"

class Delete : public Response 
{
	public:

		Delete(Request *_request, int fd, ServerBlock *sb, size_t err = 200);
		virtual ~Delete();

		bool	responseIsEmpty();
		void	contructResponse(const Request &request);
		void	answer();
};

#endif
