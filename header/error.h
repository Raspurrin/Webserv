#pragma once
#include <exception>
#include "webserv.hpp"

enum Error {
	Bad_Request,
	Not_Implemented,
	Forbidden,
	Not_Found,
	Conflict,
	Internal_Error,
	Unsupported_Media_Type,
	HTTP_Version_Not_Supported
};

class ErrC: public std::exception {
private:
	Error _err;
	std::string _desc;

public:
	ErrC(Error);
	ErrC(Error, std::string);
	virtual ~ErrC() throw() {};

	Error getError() const;
	virtual const char *what() const throw();
};
