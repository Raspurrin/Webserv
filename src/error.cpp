#include "../header/webserv.hpp"
#include "../header/error.h"

void	error_handle(std::string type)
{
	perror(type.c_str());
	exit(0);
}

ErrC::ErrC(Error _err): _err(_err), _desc("No _desc provided") {}
ErrC::ErrC(Error _err, std::string _desc): _err(_err), _desc(_desc) {}

Error ErrC::getError() const {
	return _err;
}

const char *ErrC::what() const throw() {
	return _desc.c_str();
}
