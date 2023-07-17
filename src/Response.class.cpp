#include "../header/Response.class.hpp"
#include <cstdio>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

Response::Response(StringStringMap _headerFields) : 
	_headerFields(_headerFields)
{
	if (DEBUG)
		std::cout << "in Response constructor" << std::endl;
	try
	{
		_response["Version"] = _headerFields["Version"];
		checkRequestErrors();
		methodID();
		readHTML();
	}
	catch (const std::exception &e)
	{
		const ErrC *_err = dynamic_cast<const ErrC *>(&e);
		if (_err != NULL)
		{
			buildError(_err->getError());
		}
		else
		{
			std::cout << "Catched exception " << e.what() << std::endl;
			buildError(Internal_Error);
		}
	}
	buildResponse();
	return ;
}

void Response::checkRequestErrors()
{
	if (_headerFields.count("Error") == 0)
		return ;

	if (_headerFields["Error"] == "415")
		throw ErrC(Unsupported_Media_Type);
	if (_headerFields["Error"] == "505")
		throw ErrC(HTTP_Version_Not_Supported);
}

void Response::methodID()
{
	// "/" will always be a directory, so maybe we should solve this with a route later on?
	if (_headerFields["Path"] == "/")
		_headerFields["Path"] = "/index.html";

	if (_headerFields["Method"] == "GET")
		GETMethod();
	else if (_headerFields["Method"] == "POST")
		POSTMethod();
	else if (_headerFields["Method"] == "DELETE")
		DELETEMethod();
	return ;
}

void Response::GETMethod()
{
	if (access(_headerFields["Path"].c_str() + 1, F_OK) == -1)
		throw ErrC(Not_Found);
	if (access(_headerFields["Path"].c_str() + 1, R_OK) == -1)
		throw ErrC(Forbidden);
	checkStat();
	status200();
	return ;
}

void Response::POSTMethod()
{
	if (_headerFields["Path"] != "/files/")
		throw ErrC(Forbidden);
	chdir("./files");
	std::ofstream outfile(_headerFields["Filename"].c_str());
	if (!outfile || outfile.bad() || outfile.fail())
		throw ErrC(Internal_Error, "Internal Error when creating file");
	if (outfile.good())
	{
		outfile << _headerFields["Body-Text"] << std::endl;
		outfile.close();
		chdir("..");
		status201();
	}
}

void Response::DELETEMethod()
{
	if (_headerFields["Path"].rfind("/files/", 0) == std::string::npos)
		throw ErrC(Forbidden);

	int start = _headerFields["Path"].find_last_of('/');
	_headerFields["Filename"] = _headerFields["Path"].substr(start + 1);

	chdir("./files");
	if (access(_headerFields["Filename"].c_str() + 1, F_OK) == -1)
		throw ErrC(Not_Found);
	int rem = std::remove(_headerFields["Filename"].c_str());
	chdir("..");
	if (rem != 0)
		throw ErrC(Internal_Error);
	else
	{
		_response["Status code"] = "200 OK";
		_response["Path"] = "/error_pages/deleted.html";
	}
}

void Response::buildError(const Error _err) {
	switch (_err)
	{
	case Bad_Request:
		status400();
		break;

	case Unsupported_Media_Type:
		status415();
		break;

	case Not_Implemented:
		status501();
		break;

	case HTTP_Version_Not_Supported:
		status505();
		break;

	case Forbidden:
		status403();
		break;

	case Not_Found:
		status404();
		break;

	case Conflict:
		status409();
		break;

	case Internal_Error:
		status500();
		break;

	default:
		break;

	}
}

void Response::buildResponse()
{
	_responseMessage += _response["Version"] + " "
		+ _response["Status code"] + "\n";

	for(StringStringMap::iterator it = _response.begin(); it != _response.end(); it++)
	{
		if ((*it).first.find(':') != std::string::npos)
		{
			_responseMessage += (*it).first + " " + (*it).second + "\n";
		}
	}
	_responseMessage += "Connection: close\r\n\r\n"
		+ _response["Body"];

	if (DEBUG)
		std::cout << "RESPONSE MESSAGE: \n" << _responseMessage << std::endl;
}

int Response::checkStat()
{
	struct	stat s;
	if (stat(_headerFields["Path"].c_str() + 1, &s) == 0)
	{
		//FIXME: only list directory when enabled. Requires working config.
		if (s.st_mode & S_IFDIR && listDir())
		{
			return (1);
		}
		else
			return (0);
	}
	else
		throw ErrC(Internal_Error, "Internal Error in checkStat");
	return (1);
}

/**
 * Writes a html page containing a List of files in the current directory
 * to the body of the _response object.
 *
 * Returns true if successfull, otherwise false.
*/
bool Response::listDir()
{
	char cwd[256];
	DIR *dir;

	if (getcwd(cwd, 256) != NULL)
		dir = opendir((cwd + _headerFields["Path"]).c_str());
	else
		return false;

	if (dir != NULL)
	{
		struct dirent *ent;

		// TODO: using a set makes it easier to sort the entries but has longer blocking time than an unsortet list. Need to investigate if it is too long.
		std::set<std::string> files;

		// Return value of readdir is statically allocated and must not be freed!
		while ((ent = readdir(dir)) != NULL)
			files.insert(std::string(ent->d_name));

		closedir(dir);

		std::string body = "<h1>Content of " + _headerFields["Path"] + "</h1>";

		const char *insert = _headerFields["Path"][_headerFields["Path"].size() - 1] == '/' ? "" : "/";
		for (std::set<std::string>::iterator it = files.begin(); it != files.end(); it++)
			body += "<a href=\"" + _headerFields["Path"] + insert + *it + "\">" + *it + "</a><br>";

		_response["Status code"] = "200 OK";
		_response["Content-Type:"] = "text/html";
		_response["Body"] = body;
		_response["Content-Length:"] = lenToStr(body);
		return true;
	}
	else
		return false;
}

std::string Response::lenToStr(std::string body)
{
	size_t	len = body.length();
	std::ostringstream str1;

	str1 << len;
	std::string	lenStr = str1.str();

	return (lenStr);
}

void Response::readHTML()
{
	std::ifstream	fin(_response["Path"].c_str() + 1);

	if (fin.is_open())
	{
		std::string	line, body;

		_response["Content-Type:"] = "text/html";
		while (fin.good())
		{
			getline(fin, line);
			body.append(line);
		}
		_response["Body"] = body;
		_response["Content-Length:"] = lenToStr(body);
	}
	else
		throw ErrC(Internal_Error, "Internal Error in readHtml");
	return ;
}

void Response::status200()
{
	_response["Status code"] = "200 OK";
	_response["Path"] = _headerFields["Path"];
	return ;
}

void Response::status201()
{
	_response["Status code"] = "201 CREATED";
	_response["Path"] = "/success.html";
	_response["Location:"] = _headerFields["Path"].append(_headerFields["Filename"]);
	return ;
}

void Response::status400()
{
	_response["Status code"] = "400 Bad Request";
	_response["Path"] = "/error_pages/400.html";
}

void Response::status403()
{
	_response["Status code"] = "403 Forbidden";
	_response["Path"] = "/error_pages/403.html";
}

void Response::status404()
{
	_response["Status code"] = "404 Not Found";
	_response["Path"] = "/error_pages/404.html";
}

void Response::status409()
{
	_response["Status code"] = "409 Conflict";
	_response["Path"] = "/error_pages/409.html";
}

void Response::status415()
{
	_response["Status code"] = "415 Unsupported Media Type";
	_response["Path"] = "/error_pages/415.html";
	return ;
}

void Response::status500()
{
	_response["Status code"] = "500 Internal Server Error";
	_response["Path"] = "/error_pages/500.html";
}

void Response::status501()
{
	_response["Status code"] = "501 Not Implemented";
	_response["Path"] = "/error_pages/501.html";
}

void Response::status505()
{
	_response["Status code"] = "505 HTTP Version Not Supported";
	_response["Version"] = "HTTP/1.1";
	_response["Path"] = "/error_pages/505.html";
}

std::string	Response::getResponse()
{
	return (_responseMessage);
}


// Response &	Response::operator=(Response &assign)
// {
// 	this->_responseMessage = assign.getResponse(_headerFields);
// 	return (*this);
// }

Response::Response(void)
{
}

Response::~Response(void)
{
}
