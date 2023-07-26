#include "../header/webserv.hpp"
#include "../header/Error.class.hpp"

void	error_handle(std::string type)
{
	perror(type.c_str());
	exit(0);
}

ErrorResponse::ErrorResponse(): _errorType(None), _description("No description provided") {}

ErrorResponse::ErrorResponse(ErrorType _errorType): _errorType(_errorType), _description("No description provided") {}

ErrorResponse::ErrorResponse(ErrorType _errorType, std::string _description): _errorType(_errorType), _description(_description) {}

ErrorType ErrorResponse::getError() const throw() {
	return _errorType;
}

const char *ErrorResponse::what() const throw() {
	return _description.c_str();
}
