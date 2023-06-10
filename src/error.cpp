#include "../header/webserv.hpp"
#include "../header/error.h"

void	error_handle(std::string type)
{
	perror(type.c_str());
	exit(0);
}

ErrC::ErrC(Error err): err(err) {}

Error ErrC::getError() const {
	return err;
}