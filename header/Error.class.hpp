#pragma once
#include <exception>
#include "webserv.hpp"

class ErrorResponse: public std::exception {
private:
	t_status	_status;

public:

	ErrorResponse();
	ErrorResponse(int, const std::string&);
	virtual ~ErrorResponse() throw() {};

	const t_status& getError() const throw();
	virtual const char *what() const throw();
};
