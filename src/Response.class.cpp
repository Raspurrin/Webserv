#include "../header/Response.class.hpp"

void Response::listDir()
{
	char cwd[256];
	DIR *dir;

	if (getcwd(cwd, 256) != NULL)
		dir = opendir((cwd + request["Path"]).c_str());
	else
		return;

	if (dir != NULL)
	{
		struct dirent *ent;
		std::set<std::string> files;
		std::cout << RED << "Listing dir\n" << DEF;
		while ((ent = readdir(dir)) != NULL)
			files.insert(std::string(ent->d_name));

		closedir(dir);

		for (std::set<std::string>::iterator it = files.begin(); it != files.end(); it++)
			std::cout << *it << std::endl;
	}
	else
		status404();
}

std::string Response::lenToStr(std::string body)
{
	size_t	len = body.length();
	std::ostringstream str1;

	str1 << len;
	std::string	lenStr = str1.str();

	return (lenStr);
}

void Response::status200()
{
	std::ifstream	fin(request["Path"].c_str() + 1);

	if (fin.is_open())
	{
		std::string	line, body;

		response["Status code"] = "200 OK";
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

void Response::status404()
{
	std::string	body ="404 Not Found";

	response["Status code"] = body;
	response["Content-Type:"] = "text/plain";
	response["Body"] = body;
	response["Content-Length:"] = lenToStr(body);

	return ;
}

void Response::status403()
{
	std::string	body ="403 Forbidden";

	response["Status code"] = body;
	response["Content-Type:"] = "text/plain";
	response["Body"] = body;
	response["Content-Length:"] = lenToStr(body);

	return ;
}

void Response::buildResponse()
{
	response["Version"] = "HTTP/1.1";
	methodID();
	responseMessage += response["Version"] + " " + response["Status code"] + "\n" + "Content-Type: " + response["Content-Type:"] + "\n" + "Content-Length: " + response["Content-Length:"] + "\n\n" + response["Body"]; 
	return ;
}

void Response::GETMethod()
{
	if(access(request["Path"].c_str() + 1, F_OK) == -1)
		status404();
	else if(access(request["Path"].c_str() + 1, R_OK) == -1)
		status403();
	else
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

