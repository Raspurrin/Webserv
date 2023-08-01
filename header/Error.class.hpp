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
	int	_errorCode;
	std::string _description;

public:

	ErrorResponse();
	ErrorResponse(ErrorType);
	ErrorResponse(const int, std::string);
	ErrorResponse(ErrorType, std::string);
	virtual ~ErrorResponse() throw() {};

	std::pair<const int, std::string>& getError() const throw();
	virtual const char *what() const throw();
};
