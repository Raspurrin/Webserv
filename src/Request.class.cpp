#include "../header/Request.class.hpp"

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

void Request::parseHeaderSection()
{
	size_t	position, lpos;

	position = _requestBuffer.find("\r\n");
	parseStartLine(_requestBuffer.substr(0, position));
	position += 2;
	lpos = position;
	position = _requestBuffer.find("\r\n\r\n", lpos);
	parseHeaderFields(_requestBuffer.substr(lpos, position - lpos));
	position += 4;
	if (_headerFields["Method"] == "POST")
		parseBody(_requestBuffer.substr(position));
	if (DEBUG)
		printMap();
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

	std::stringstream	ss(body);
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
		found = _headerFields["Body-Type"].find("text/plain");
		if (found != std::string::npos)
		{
			getline(ss, line);
			getline(ss, _headerFields["Body-Text"], '\r');
		}
		else
			_headerFields["Error"] = "415";
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

	_readCount = recv(socket, readBuffer, BUFLEN - 1, 0);
	if (_readCount <= 0)
	{
		close(socket);
		_indexesToRemove.push_back(socket);		
	}
		_isRead = true;
	
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
		parseHeaderSection();
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
