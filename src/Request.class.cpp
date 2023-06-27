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
}

void Request::readIntoString(int &socket)
{
	char	readBuffer[BUFLEN] = {0};

	if (recv(socket, readBuffer, BUFLEN - 1, 0) == -1)
		error_handle("Read failed");
	std::cout << "received message: " << readBuffer;
	headerBuffer = readBuffer;
}

// void	Request::readingBody(int &socket)
// {
// 	std::string readString = readIntoString(socket);

// 	bodyBuffer += readString;
// 	std::cout << "in reading Body bodyBuffer: " << bodyBuffer << std::endl;
// 	readCount += BUFLEN;
// 	if (readCount < atoi(headerFields["Content-Length"].c_str()))
// 		bufferFlags = bufferFlags & REACHED_BODY_END;
// }

std::string	Request::readingHeader(int &socket)
{
/*	int		position;

	std::cout << "in readingHeader" << std::endl;
	if (!readString.find("\n\n"))
	{
		headerBuffer += readString;
		std::cout << "headerBuffer: " << headerBuffer << std::endl;
		return ("");
	}
	bufferFlags = bufferFlags | REACHED_HEADER_END;
	std::cout << "bufferFlags: in readingHeader: " << bufferFlags << std::endl;
	position = readString.find("\n\n");
	headerBuffer += readString.substr(0, position);
//	bodyBuffer = readString.substr(position + 2, BUFLEN - position);
	*/
	readIntoString(socket);
	parseHeaderSection();
	return (buildResponse());
}

std::string	Request::getRequest(int	&socket)
{
	std::cout << "getting request" << std::endl;
	return(readingHeader(socket));
	// if ((bufferFlags & REACHED_HEADER_END) == false)
	// else if ((bufferFlags & REACHED_BODY_END) == false)
	// {
	// 	std::cout << "before reading body" << std::endl;
	// 	readingBody(socket);
	// 	_responseMessage = buildResponse();
	// 	return (_responseMessage);
	// }
}

std::string	Request::buildResponse()
{
	printMap();
	Response response(headerFields);
	return (response.getResponse());

}

StringStringMap	Request::getHeaderFields()
{
	return (headerFields);
}

void	Request::printBody(void)
{
	std::cout << bodyBuffer << std::endl;
}

bool	Request::isFlagOn(int flag)
{
	return ((bufferFlags & flag));
}

Request::Request(void) :
	bufferFlags(0),
	readCount(0)
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
	return headerFields[key];
}

Request::~Request(void)
{
}
