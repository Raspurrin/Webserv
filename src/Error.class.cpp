#include "../header/webserv.hpp"
#include "../header/Error.class.hpp"
#include <utility>

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

ErrorResponse::ErrorResponse()
{
	_status._code = 0;
	_status._description = "none";
	_status._message = "none";
}

ErrorResponse::ErrorResponse(int _errorCode, const std::string& _description)
{
	IntStringMap errorTypes = createErrorMap();

	IntStringMap::iterator it = errorTypes.find(_errorCode);
	if (it == errorTypes.end())
		it = errorTypes.find(500);
	_status._code = it->first;
	_status._description = it->second;
	_status._message = _description;
}

const t_status& ErrorResponse::getError() const throw() {
	return (_status);
}

const char *ErrorResponse::what() const throw() {
	return _status._message.c_str();
}
