#pragma once
#include <exception>
#include "webserv.hpp"

enum ErrorType {
	Bad_Request,
	Not_Implemented,
	Forbidden,
	Not_Found,
	Conflict,
	Internal_Error,
	Unsupported_Media_Type,
	HTTP_Version_Not_Supported,
	Length_Required,
	Payload_Too_Large,
	Method_Not_Allowed,
	Request_Timeout,
	Request_Header_Fields_Too_Large,
	Too_Many_Requests,
	None
};

class ErrorResponse: public std::exception {
private:
	ErrorType _errorType;
	std::string _description;

public:

	ErrorResponse();
	ErrorResponse(ErrorType);
	ErrorResponse(ErrorType, std::string);
	virtual ~ErrorResponse() throw() {};

	ErrorType getError() const throw();
	virtual const char *what() const throw();
};
