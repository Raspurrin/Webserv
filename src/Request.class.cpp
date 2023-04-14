#include "../header/Request.class.hpp"
#include <cstdio>
#include <sstream>

void Request::printMap()
{
	std::cout << RED << "\nPrinting map of header fields...\n" << DEF << std::endl;
	std::map<std::string, std::string>::iterator it = headerFields.begin();
	while (it != headerFields.end())
	{
		std::cout << YELLOW << it->first << " " << DEF << it->second << std::endl;
		++it;
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
	printMap();
	return ;
}

void Request::parseStartLine(std::string startLine)
{
	std::cout << CYAN << "Start line is:\n" << DEF << startLine << std::endl;
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
	return ;
}

void Request::parseHeaderFields(std::string headerSection)
{
	std::cout << CYAN << "Header section is:\n" << DEF << headerSection << std::endl;

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

