#include "../header/Response.class.hpp"
#include <sys/stat.h>


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
	return ;
}

void Response::status200()
{
	std::cout << "in status200" << std::endl;
	_response["Status code"] = "200 OK";
	_response["Path"] = _headerFields["Path"];
	readHTML();
	return ;
}

int Response::status404()
{
	if (access(_headerFields["Path"].c_str() + 1, F_OK) == -1)
	{
		_response["Status code"] = "404 Not Found";
		_response["Path"] = "/error_pages/404.html";
		readHTML();
		return (1);
	}
	else
		return (0);
}

int Response::status403()
{
	if (access(_headerFields["Path"].c_str() + 1, R_OK) == -1)
	{
		_response["Status code"] = "403 Forbidden";
		_response["Path"] = "/error_pages/403.html";
		readHTML();
		return (1);
	}
	else
		return (0);
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
	{
		_response["Status code"] = "500 Internal Server Error";
		_response["Path"] = "/error_pages/500.html";
		readHTML();
		return (1);
	}
}

void Response::buildResponse()
{
	_response["Version"] = "HTTP/1.1";
	methodID();
	_responseMessage += _response["Version"] + " " + _response["Status code"] + "\n" + "Content-Type: " + _response["Content-Type:"] + "\n" + "Content-Length: " + _response["Content-Length:"] + "\n\n" + _response["Body"];
	std::cout << "response MESSAGE" << _responseMessage << std::endl;
	return ;
}

void Response::GETMethod()
{
	// "/" will always be a directory, so maybe we should solve this with a route later on?
	if (_headerFields["Path"] == "/")
		_headerFields["Path"] = "/index.html";
	if (checkStat() == 1)
		return ;
	if (status404() == 1)
		return ;
	if (status403() == 1)
		return ;
	status200();
	return ;
}

void Response::methodID()
{
	if (_headerFields["Method"] == "GET")
		GETMethod();
	return ;
}

std::string	Response::getResponse()
{
	return (_responseMessage);
}

Response::Response(StringStringMap _headerFields) : 
	_headerFields(_headerFields)
{
	std::cout << "in Response constructor" << std::endl;
	buildResponse();
	return ;
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
