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
	else
		throw ErrC(Internal_Error, "Internal Error in readHtml");
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
	response["Status code"] = "404 Not Found";
	response["Path"] = "/error_pages/404.html";
	readHTML();
	return (1);
}

void Response::status500()
{
	response["Status code"] = "500 Internal Server Error";
	response["Path"] = "/error_pages/500.html";
	readHTML();
}

int Response::status403()
{
	response["Status code"] = "403 Forbidden";
	response["Path"] = "/error_pages/403.html";
	readHTML();
	return (1);
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

void Response::buildError(const Error err) {
	switch (err)
	{
	case Bad_Request:
		// TODO
		break;

	case Forbidden:
		status403();
		break;

	case Not_Found:
		status404();
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
	// TODO: this block can be moved to a different place, depending on djaisins changes
	try {
		response["Version"] = "HTTP/1.1";
		methodID();
	} catch (const std::exception &e) {
		const ErrC *err = dynamic_cast<const ErrC *>(&e);
		if (err != NULL) {
			buildError(err->getError());
		} else {
			std::cout << "Catched exception " << e.what() << std::endl;
			buildError(Internal_Error);
		}
	}

	responseMessage += response["Version"] + " " + response["Status code"] + "\n" + "Content-Type: " + response["Content-Type:"] + "\n" + "Content-Length: " + response["Content-Length:"] + "\n\n" + response["Body"];
	std::cout << "RESPONSE MESSAGE" << responseMessage << std::endl;
}

void Response::GETMethod()
{
	// "/" will always be a directory, so maybe we should solve this with a route later on?
	if (request["Path"] == "/")
		request["Path"] = "/index.html";
	if (access(request["Path"].c_str() + 1, F_OK) == -1)
		throw ErrC(Not_Found);
	if (access(request["Path"].c_str() + 1, R_OK) == -1)
		throw ErrC(Forbidden);
	checkStat();
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
