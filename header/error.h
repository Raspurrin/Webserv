#pragma once
#include <exception>

enum Error {
	Bad_Request,
	Forbidden,
	Not_Found,
	Internal_Error
};

class ErrC: public std::exception {
private:
	Error err;
	std::string desc;

public:
	ErrC(Error);
	ErrC(Error, std::string);
	virtual ~ErrC() throw() {};

	Error getError() const;
	virtual const char *what() const throw();
};