#include "../header/Request.class.hpp"
#include <cstdio>
#include <sstream>
#include <string>
#include <unistd.h>

void Request::printMap()
{
	std::cout << CYAN << "\nPrinting map of header fields...\n" << DEF << std::endl;
	std::map<std::string, std::string>::iterator it = _headerFields.begin();
	while (it != _headerFields.end())
	{
		std::cout << YELLOW << it->first << " " << DEF << it->second << std::endl;
		++it;
	}
}

void Request::parseBody(std::string body)
{
	std::string line;
	size_t	found, lpos;

	if (_headerFields.count("Content-Type") == 0 || _headerFields.count("Content-Length") == 0 || _headerFields["Content-Length"] == "0")
	{
		_headerFields["Error"] = "400";
		return ;
	}
  
	_headerFields["Boundary"] = _headerFields["Content-Type"].substr(_headerFields["Content-Type"].find('=') + 1);

	std::istringstream	ss(body);
	getline(ss, line);
	found = line.find(_headerFields["Boundary"]);
	if (found != std::string::npos)
	{
		getline(ss, _headerFields["Body-Disposition"]);
		found = _headerFields["Body-Disposition"].find_last_of('"');
		found -= 1;
		lpos = found;
		while (_headerFields["Body-Disposition"][lpos] != '"')
			lpos--;
		_headerFields["Filename"] = _headerFields["Body-Disposition"].substr(lpos + 1, found - lpos);
		getline(ss, _headerFields["Body-Type"]);
		getline(ss, line);
		std::stringstream remainder;
		remainder << ss.rdbuf();
		const std::string tmp = remainder.str();
		size_t idx = tmp.find_last_of(_headerFields["Boundary"]);
		_headerFields["Body-Text"] = tmp.substr(0, idx - _headerFields["Boundary"].length() - 4);
	}
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
	size_t found = _headerFields["Version"].find("HTTP/1.1");
	if (found == std::string::npos)
		_headerFields["Error"] = "505";
}

void Request::parseHeaderFields(std::istringstream &iss)
{
	std::string key, value, line;
	int	position;

	while (getline(iss, line))
	{
		if (line.size() == 1) {
			_header_done = true;
			if (_headerFields["Transfer-Encoding"] == "chunked")
				_isChunked = true;
			break ;
		}
		position = line.find(':');
		key = line.substr(0, position);
		position++;
		if (line[position] == ' ')
			position++;
		value = line.substr(position);
		key[0] = toupper(key[0]);
		size_t pos = key.find('-');
		if (pos != std::string::npos)
			key[pos + 1] = toupper(key[pos + 1]);
		_headerFields[key] = value;
	}
}

// TODO: Maybe find a way to avoid so many if statements?
void Request::readIntoString(int &socket)
{
	char	readBuffer[BUFLEN] = {0};

	int bytes_read = recv(socket, readBuffer, BUFLEN - 1, 0);
	if (bytes_read <= 0)
	{
		close(socket);
		_indexesToRemove.push_back(socket);
	}

	// We need the number of bytes read here since we cant be sure that we didnt read any zero bytes,
	// which would lead to a truncation of the string.
	std::string read(readBuffer, bytes_read);

	if (!_first_line && read.find("\r\n") != std::string::npos) {
		parseStartLine(read.substr(0, read.find("\r\n")));
		read = read.substr(read.find("\r\n") + 2);
		_first_line = true;
	}

	std::istringstream iss(read);
	if (!_header_done)
		parseHeaderFields(iss);

	if (_content_len == 0 && _headerFields.find("Content-Length") != _headerFields.end()) {
		_content_len = atoi(_headerFields["Content-Length"].c_str());
	}

	std::stringstream remainder;
	remainder << iss.rdbuf();

	if (_header_done && _isChunked) {
		const std::string &tmp = remainder.str();
		std::stringstream ss;
		ss << std::hex << tmp.substr(0, tmp.find("\r\n"));
	}

	if (_header_done && _readCount < _content_len) {
		_bodyBuffer += remainder.str();
		_readCount += remainder.str().length();
	}

	if (_header_done && _readCount >= _content_len) {
		if (_readCount > 0) {
			parseBody(_bodyBuffer);
		}
		_isRead = true;
	}

	if (DEBUG)
	{
		std::cout << CYAN << "\nReceived message:\n\n" << DEF << readBuffer << std::endl;
		std::cout << CYAN << "Read count:\n" << DEF << _readCount << std::endl;
	}
	_requestBuffer.append(readBuffer);
}

void	Request::getRequest(int	&socket)
{
	if (DEBUG)
		std::cout << CYAN << "\nGetting request...\n" << DEF;
	try {
		readIntoString(socket);
	} catch (const std::exception &e) {
//		const ErrC *_err = dynamic_cast<const ErrC *>(&e);
		std::cout << "Catched exception " << e.what() << std::endl;
	}
}

std::string	Request::getResponse()
{
	Response response(_headerFields);
	return (response.getResponse());
}

StringStringMap	Request::getHeaderFields()
{
	return (_headerFields);
}

bool	Request::isFlagOn()
{
	return (_isRead);
}

Request::Request(void) :
	_isRead(false),
	_readCount(0),
	_first_line(false),
	_header_done(false),
	_isChunked(false),
	_content_len(0)
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
