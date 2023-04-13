#include "../header/Request.class.hpp"
#include <cstdio>
#include <sstream>

void Request::parseHeaderSection()
{
	std::istringstream	iss(requestMessage);
	std::string			line;

	getline(iss, line);
	std::cout << line << std::endl;
	parseStartLine(line);
	parseHeaderFields(iss);
	return ;
}

void Request::parseStartLine(std::string startLine)
{
	std::cout << "in start line" << std::endl;
	std::cout << startLine << std::endl;	
	return ;
}

void Request::parseHeaderFields(std::istringstream iss)
{
	getline(iss, line);
	std::cout << "in header fields" << std::endl;
	std::cout << headerFields << std::endl;	
	return ;
}

Request::Request(void)
{
	return ;
}

Request::Request(const std::string requestMessage) : requestMessage(requestMessage)
{
	iss(requestMessage);
	parseHeaderSection();
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

