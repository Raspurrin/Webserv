#include "../header/Request.class.hpp"
#include <cstdio>
#include <sstream>
#include <string>
#include <unistd.h>

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

	position = headerBuffer.find('\n');
	parseStartLine(headerBuffer.substr(0, position));
	position++;
	lpos = position;
	position = headerBuffer.find("\n\n", lpos);
	parseHeaderFields(headerBuffer.substr(lpos, position - lpos));
	requestHeader = headerFields;
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

void	Request::readingBody(int &socket)
{

}

void	Request::readingHeader(int &socket)
{
	int		position;
	char	readBuffer[BUFLEN];

	if (!read(socket, readBuffer, BUFLEN))
		error_handle("Read failed");
	std::string readString(readBuffer);

	if (readString.find("\n\n"))
	{
		bufferFlags = bufferFlags | REACHED_HEADER_END;
		position = readString.find("\n\n");
		headerBuffer += readString.substr(0, position);
		bodyBuffer = readString.substr(position, BUFLEN - position);
	}
	else
		headerBuffer += readString;
}

void	Request::getRequest(int	&socket)
{
	if (bufferFlags & REACHED_HEADER_END == 0)
		readingHeader(socket);
	else if (bufferFlags & REACHED_BODY_END == 0)
		readingBody(socket);
	else
		parseHeaderFields(headerBuffer);
}

Request::Request(void)
{
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
