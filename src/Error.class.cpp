#include "../header/webserv.hpp"
#include "../header/Error.class.hpp"

void	error_handle(std::string type)
{
	perror(type.c_str());
	exit(0);
}

IntStringMap	createErrorMap()
{
	IntStringMap	errorTypes;

	errorTypes[400] = "Bad Request";
	errorTypes[403] = "Forbidden";
	errorTypes[404] = "Not Found";
	errorTypes[405] = "Method Not Allowed";
	errorTypes[408] = "Request Timeout";
	errorTypes[409] = "Conflict";
	errorTypes[411] = "Length Required";
	errorTypes[413] = "Content Too Large";
	errorTypes[414] = "URI Too Long";
	errorTypes[431] = "Request Header Fields Too Large";
	errorTypes[415] = "Unsupported Media Type";
	errorTypes[500] = "Internal Server Error";
	errorTypes[501] = "Not Implemented";
	errorTypes[505] = "HTTP Version Not Supported";

	return errorTypes;
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
