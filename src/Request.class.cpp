#include "../header/Request.class.hpp"
#include <cstdio>
#include <sstream>

void Request::printMap()
{
	std::cout << RED << "\nPrinting map of header fields...\n" << DEF << std::endl;
	std::map<std::string, std::string>::iterator it = headerFields.begin();
	std::cout << YELLOW;
	while (it != headerFields.end())
	{
		std::cout << it->first << ": " << it->second << std::endl;
		++it;
	}
	std::cout << DEF << std::endl;
	return ;
}

void Request::parseHeaderSection()
{
	std::istringstream	iss(requestMessage);
	std::string			line;

	int position;

	position = requestMessage.find('\n');
	getline(iss, line);
	parseStartLine(requestMessage.substr(0, position));
	parseHeaderFields();
	return ;
}

void Request::parseStartLine(std::string startLine)
{
	std::cout << CYAN << "First line is: " << DEF << startLine << std::endl;
	int	position, lpos;

	//adding method to map
	position = startLine.find(' ');
	headerFields["Method"] = startLine.substr(0, position);
	//skipping the space
	position++;
	lpos = position;
	position = startLine.find(' ', lpos);
	headerFields["Path"] = startLine.substr(lpos, position - lpos);
	position++;
	lpos = position;
	position = startLine.find(' ', lpos);
	headerFields["Version"] = startLine.substr(lpos, position - lpos);
	printMap();
	return ;
}

void Request::parseHeaderFields()
{
//	std::cout << "in header fields" << std::endl;
//	std::cout << requestMessage << std::endl;	
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

