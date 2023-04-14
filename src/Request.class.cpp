#include "../header/Request.class.hpp"
#include <cstdio>
#include <sstream>

void Request::parseHeaderSection()
{
	std::istringstream	iss(requestMessage);
	std::string			line;

	getline(iss, line);
	parseStartLine(line);
	parseHeaderFields();
	return ;
}

void Request::parseStartLine(std::string startLine)
{
	std::cout << "in start line" << std::endl;
	std::cout << startLine << std::endl;	
	int	position, lpos;

	//adding method to map
	position = startLine.find(' ');
	headerFields["Method"] = startLine.substr(0, position);
	std::cout << "Key: Method, Value: " << headerFields["Method"] << std::endl;
	//skipping the space
	position++;
	lpos = position;
	position = startLine.find(' ', lpos);
	headerFields["Path"] = startLine.substr(lpos, position);
	std::cout << "Key: Path, Value: " << headerFields["Path"] << std::endl;
	return ;
}

void Request::parseHeaderFields()
{
	std::cout << "in header fields" << std::endl;
	std::cout << requestMessage << std::endl;	
	return ;
}

Request::Request(void)
{
	return ;
}

Request::Request(const std::string requestMessage) : requestMessage(requestMessage)
{
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

