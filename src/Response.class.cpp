#include "../header/Response.class.hpp"
#include <sys/stat.h>

/**
 * Writes a html page containing a List of files in the current directory
 * to the body of the Response object.
 *
 * Returns true if successfull, otherwise false.
*/
bool Response::listDir()
{
	char cwd[256];
	DIR *dir;

	if (getcwd(cwd, 256) != NULL)
		dir = opendir((cwd + request["Path"]).c_str());
	else
		return false;

	if (dir != NULL)
	{
		struct dirent *ent;

		// TODO: using a set makes it easier to sort the entries but has longer blocking time than an unsortet list. Need to investigate if it is too long.
		std::set<std::string> files;

		while ((ent = readdir(dir)) != NULL)
			files.insert(std::string(ent->d_name));

		closedir(dir);

		std::string body = "<h1>Content of " + request["Path"] + "</h1>";

		const char *insert = request["Path"] == "/" ? "" : "/";
		for (std::set<std::string>::iterator it = files.begin(); it != files.end(); it++)
			body += "<a href=\"" + request["Path"] + insert + *it + "\">" + *it + "</a><br>";

		response["Status code"] = "200 OK";
		response["Content-Type:"] = "text/html";
		response["Body"] = body;
		response["Content-Length:"] = lenToStr(body);
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
	std::ifstream	fin(response["Path"].c_str() + 1);

	if (fin.is_open())
	{
		std::string	line, body;

		response["Content-Type:"] = "text/html";
		while (fin.good())
		{
			getline(fin, line);
			body.append(line);
		}
		response["Body"] = body;
		response["Content-Length:"] = lenToStr(body);
	}

	return ;
}

void Response::status200()
{
	response["Status code"] = "200 OK";
	response["Path"] = request["Path"];
	readHTML();
	return ;
}

int Response::status404()
{
	if (access(request["Path"].c_str() + 1, F_OK) == -1)
	{
		response["Status code"] = "404 Not Found";
		response["Path"] = "/error_pages/404.html";
		readHTML();
		return (1);
	}
	else
		return (0);
}

int Response::status403()
{
	if (access(request["Path"].c_str() + 1, R_OK) == -1)
	{
		response["Status code"] = "403 Forbidden";
		response["Path"] = "/error_pages/403.html";
		readHTML();
		return (1);
	}
	else
		return (0);
}

int Response::checkStat()
{
	struct	stat s;

	if (stat(request["Path"].c_str() + 1, &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			response["Status code"] = "200 OK";
			response["Path"] = "/directory.html";
			readHTML();
			return (1);
		}
		else
			return (0);
	}
	else
	{
		response["Status code"] = "500 Internal Server Error";
		response["Path"] = "/error_pages/500.html";
		readHTML();
		return (1);
	}
}

void Response::buildResponse()
{
	response["Version"] = "HTTP/1.1";
	methodID();
	responseMessage += response["Version"] + " " + response["Status code"] + "\n" + "Content-Type: " + response["Content-Type:"] + "\n" + "Content-Length: " + response["Content-Length:"] + "\n\n" + response["Body"];
	responseMessage += response["Version"] + " " + response["Status code"] + "\n" + "Content-Type: " + response["Content-Type:"] + "\n" + "Content-Length: " + response["Content-Length:"] + "\n\n" + response["Body"];
	std::cout << "RESPONSE MESSAGE" << responseMessage << std::endl;
	return ;
}

void Response::GETMethod()
{
	//FIXME: only list directory when enabled. Requires working config.
	if (listDir())
		return;

	if (request["Path"] == "/")
		request["Path"] = "/index.html";
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
	if (request["Method"] == "GET")
		GETMethod();
	return ;
}

std::string Response::getResponse(void) const
{
	return (this->responseMessage);
}

Response::Response(std::map<std::string, std::string> request) : request(request)
{
	buildResponse();
	return ;
}

Response &	Response::operator=(const Response &assign)
{
	this->responseMessage = assign.getResponse();
	return (*this);
}

Response::~Response(void)
{
	return ;
}

