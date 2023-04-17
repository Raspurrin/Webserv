#include "webserv.hpp"

void	error_handle(std::string type)
{
	perror(type.c_str());
	exit(0);
}
