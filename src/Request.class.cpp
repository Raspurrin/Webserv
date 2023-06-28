#include "../header/Request.class.hpp"
#include <cstdio>
#include <sstream>
#include <string>
#include <unistd.h>

void Request::printMap()
{
	std::cout << RED << "\nPrinting map of header fields...\n" << DEF << std::endl;
	std::map<std::string, std::string>::iterator it = _headerFields.begin();
	while (it != _headerFields.end())
	{
		std::cout << YELLOW << it->first << " " << DEF << it->second << std::endl;
		++it;
	}
}

void Request::parseHeaderSection()
{
	int	position, lpos;

	position = _headerBuffer.find('\n');
	parseStartLine(_headerBuffer.substr(0, position));
	position++;
	lpos = position;
	position = _headerBuffer.find("\n\n", lpos);
	parseHeaderFields(_headerBuffer.substr(lpos, position - lpos));
}

void Request::parseStartLine(std::string startLine)
{
	int	position, lpos;

	position = startLine.find(' ');
	_headerFields["Method"] = startLine.substr(0, position);
	position++;
	lpos = position;
	position = startLine.find(' ', lpos);
	_headerFields["Path"] = startLine.substr(lpos, position - lpos);
	position++;
	lpos = position;
	position = startLine.find(' ', lpos);
	_headerFields["Version"] = startLine.substr(lpos, position - lpos);
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
		_headerFields[key] = value;
	}
}

void Request::readIntoString(int &socket)
{
	char	readBuffer[BUFLEN] = {0};

	if (recv(socket, readBuffer, BUFLEN - 1, 0) == -1)
		error_handle("Read failed");
	std::cout << "received message: " << readBuffer;
	_headerBuffer = readBuffer;
}

// void	Request::readingBody(int &socket)
// {
// 	std::string readString = readIntoString(socket);

// 	_bodyBuffer += readString;
// 	std::cout << "in reading Body _bodyBuffer: " << _bodyBuffer << std::endl;
// 	_readCount += BUFLEN;
// 	if (_readCount < atoi(_headerFields["Content-Length"].c_str()))
// 		_bufferFlags = _bufferFlags & REACHED_BODY_END;
// }

std::string	Request::readingHeader(int &socket)
{
/*	int		position;

	std::cout << "in readingHeader" << std::endl;
	if (!readString.find("\n\n"))
	{
		_headerBuffer += readString;
		std::cout << "_headerBuffer: " << _headerBuffer << std::endl;
		return ("");
	}
	_bufferFlags = _bufferFlags | REACHED_HEADER_END;
	std::cout << "_bufferFlags: in readingHeader: " << _bufferFlags << std::endl;
	position = readString.find("\n\n");
	_headerBuffer += readString.substr(0, position);
//	_bodyBuffer = readString.substr(position + 2, BUFLEN - position);
	*/
	readIntoString(socket);
	parseHeaderSection();
	return (buildResponse());
}

std::string	Request::getRequest(int	&socket)
{
	std::cout << "getting request" << std::endl;
	return(readingHeader(socket));
}

std::string	Request::buildResponse()
{
	printMap();
	Response response(_headerFields);
	_isRead = true;
	return (response.getResponse());
}

StringStringMap	Request::getHeaderFields()
{
	return (_headerFields);
}

void	Request::printBody(void)
{
	std::cout << _bodyBuffer << std::endl;
}

bool	Request::isFlagOn()
{
	return (_isRead);
}

Request::Request(void) :
	_bufferFlags(0),
	_readCount(0)
{
}

Request::Request(Request const &src)
{
	*this = src;
}

Request 		&Request::operator=(const Request &assign)
{
	(void) assign;
	return (*this);
}

std::string 	Request::operator[](std::string const &key) 
{
	return _headerFields[key];
}

Request::~Request(void)
{
}
