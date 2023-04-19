#include "../header/Request.class.hpp"
#include <cstdio>
#include <sstream>
#include <string>
#include <unistd.h>

std::string Request::getResponse()
{
	return (responseMessage);
}

void Request::buildResponse()
{
	response["Version"] = "HTTP/1.1";
	methodID();
	responseMessage += response["Version"] + " " + response["Status code"] + "\n" + "Content-Type: " + response["Content-Type:"] + "\n" + "Content-Length: " + response["Content-Length:"] + "\n\n" + response["Body"]; 
	return ;
}

void Request::GETMethod()
{
	//check if file requested exists
	//if resquested resource not found 404 Not Found
	if(access(headerFields["Path"].c_str() + 1, F_OK) == -1)
		response["Status code"] = "404 Not Found";

	else if(access(headerFields["Path"].c_str() + 1, R_OK) == -1)
		response["Status code"] = "403 Forbidden";
	else
	{
		std::ifstream	fin(headerFields["Path"].c_str() + 1);
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

void Request::methodID()
{
	if (headerFields["Method"] == "GET")
		GETMethod();
	return ;
}

void Request::printMap()
{
	std::cout << RED << "\nPrinting map of header fields...\n" << DEF << std::endl;
	std::map<std::string, std::string>::iterator it = headerFields.begin();
	while (it != headerFields.end())
	{
		std::cout << YELLOW << it->first << " " << DEF << it->second << std::endl;
		++it;
	}
	std::cout << RED << "\nPrinting map of response fields...\n" << DEF << std::endl;
	std::map<std::string, std::string>::iterator itt = response.begin();
	while (itt != response.end())
	{
		std::cout << YELLOW << itt->first << " " << DEF << itt->second << std::endl;
		++itt;
	}
	return ;
}

void Request::parseHeaderSection()
{
	int	position, lpos;

	position = requestMessage.find('\n');
	parseStartLine(requestMessage.substr(0, position));
	position++;
	lpos = position;
	position = requestMessage.find("\n\n", lpos);
	parseHeaderFields(requestMessage.substr(lpos, position - lpos));

	return ;
}

void Request::parseStartLine(std::string startLine)
{
	int	position, lpos;

	position = startLine.find(' ');
	headerFields["Method"] = startLine.substr(0, position);
	position++;
	lpos = position;
	position = startLine.find(' ', lpos);
	headerFields["Path"] = startLine.substr(lpos, position - lpos);
	position++;
	lpos = position;
	position = startLine.find(' ', lpos);
	headerFields["Version"] = startLine.substr(lpos, position - lpos);

	return ;
}

void Request::parseHeaderFields(std::string headerSection)
{
	std::istringstream	iss(headerSection);
	std::string key, value, line;
	int	position;

	while (getline(iss, line))
	{
		if (line.size() == 1)
			break ;
		position = line.find(':');
		key = line.substr(0, position);
		position++;
		if (line[position] == ' ')
			position++;
		value = line.substr(position);
		headerFields[key] = value;
	}

	return ;
}

Request::Request(void)
{
	return ;
}

Request::Request(std::string requestMessage) : 
	requestMessage(requestMessage)
{
	parseHeaderSection();
	Response response(headerFields);
	buildResponse();
	return ;
}

Request::Request(Request const &src)
{
	*this = src;
	return ;
}

Request &	Request::operator=(const Request &assign)
{
	(void) assign;
	return (*this);
}

Request::~Request(void)
{
	return ;
}

