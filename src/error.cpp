#include "../header/webserv.hpp"
#include "../header/error.h"

void	error_handle(std::string type)
{
	perror(type.c_str());
	exit(0);
}

ErrC::ErrC(Error err): err(err), desc("No desc provided") {}
ErrC::ErrC(Error err, std::string desc): err(err), desc(desc) {}

Error ErrC::getError() const {
	return err;
}

const char *ErrC::what() const throw() {
	return desc.c_str();
}