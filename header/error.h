#pragma once
#include <exception>

enum Error {
	Bad_Request,
	Unsupported_Media_Type,
	Not_Implemented,
	HTTP_Version_Not_Supported,
	Forbidden,
	Not_Found,
	Conflict,
	Internal_Error
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
