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
	size_t	position, lpos;
	std::string delim = "\r\n\r\n";

	if(_readCount <= 0)
		return ;
	position = _requestBuffer.find("\r\n");
	std::cout << "position: " << position << std::endl;
	parseStartLine(_requestBuffer.substr(0, position));
	position += 2;
	lpos = position;
	std::cout << "lpos: " << lpos << std::endl;
	position = _requestBuffer.find(delim, lpos);
	std::cout << "position: " << position << std::endl;
	parseHeaderFields(_requestBuffer.substr(lpos, position - lpos));
	position += 4;
	if ((int)position < _readCount)
	{
		_bodyBuffer = _requestBuffer.substr(position);
		std::cout << "Body buffer:\n" << _bodyBuffer << "END" << std::endl;
	}
}

void Request::parseStartLine(std::string startLine)
{
	std::cout << "start line: " << startLine << "e";
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
	std::cout << "header is: " << headerSection << "e";
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

	_readCount = recv(socket, readBuffer, BUFLEN - 1, 0);
	if (_readCount <= 0)
	{
		close(socket);
		_indexesToRemove.push_back(socket);		
	}
	std::cout << RED << "Received message:\n" << DEF << readBuffer << "END" << std::endl;
	std::cout << RED << "Read count:\n" << DEF << _readCount << "END" << std::endl;
	_requestBuffer = readBuffer;
}

std::string	Request::readingRequest(int &socket)
{
	readIntoString(socket);
	parseHeaderSection();
	return (buildResponse());
}

std::string	Request::getRequest(int	&socket)
{
	std::cout << RED << "Getting request..." << DEF << std::endl;
	return(readingRequest(socket));
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
	_isRead(false),
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
