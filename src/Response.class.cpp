#include "../header/Response.class.hpp"

void Response::buildResponse()
{
	response["Version"] = "HTTP/1.1";
	methodID();
	responseMessage += response["Version"] + " " + response["Status code"] + "\n" + "Content-Type: " + response["Content-Type:"] + "\n" + "Content-Length: " + response["Content-Length:"] + "\n\n" + response["Body"]; 
	return ;
}

void Response::GETMethod()
{
	//check if file requested exists
	//if resquested resource not found 404 Not Found
	if(access(request["Path"].c_str() + 1, F_OK) == -1)
		response["Status code"] = "404 Not Found";

	else if(access(request["Path"].c_str() + 1, R_OK) == -1)
		response["Status code"] = "403 Forbidden";
	else
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
			size_t	len = body.length();
			std::ostringstream str1;
			str1 << len;
			std::string	lenStr = str1.str();
			response["Content-Length:"] = lenStr;
		}
	}
	//if no acess rights then 403 Forbidden
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

