#pragma once
#include <exception>

enum Error {
	Bad_Request,
	Forbidden,
	Not_Found,
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
