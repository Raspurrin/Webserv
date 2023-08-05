#include "../header/Request.class.hpp"

void Request::printMap()
{
	std::cout << CYAN << "\nPrinting map of header fields...\n" << DEF << std::endl;
	StringStringMap::iterator it = _headerFields.begin();
	while (it != _headerFields.end())
	{
		std::cout << YELLOW << it->first << " " << DEF << it->second << std::endl;
		++it;
	}
}

void Request::doesKeyExist(int error, const std::string& key, const std::string& message)
{
	if (_headerFields.count(key) == 0)
		throw ErrorResponse(error, message);
}

void Request::getBoundary()
{
	doesKeyExist(400, "Content-Type", "Couldn't find content type");
	_headerFields["Boundary"] = _headerFields["Content-Type"].substr(_headerFields["Content-Type"].find('=') + 1);

}

void Request::parseBody(std::string body)
{
	std::string line;
	size_t	found, lpos;

	
	_headerFields["Boundary"] = _headerFields["Content-Type"].substr(_headerFields["Content-Type"].find('=') + 1);

	std::istringstream	ss(body);
	getline(ss, line);
	found = line.find(_headerFields["Boundary"]);
	if (found == std::string::npos)
		throw ErrorResponse(400, "No boundary found.");
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

void Request::parseStartLine(std::string startLine)
{
	int	position, lpos;

	position = startLine.find(' ');
	_headerFields["Method"] = startLine.substr(0, position);
	position++;
	lpos = position;
	position = startLine.find(' ', lpos);
	URLDecode(startLine.substr(lpos, position - lpos));
	position++;
	lpos = position;
	position = startLine.find(' ', lpos);
	_headerFields["Version"] = startLine.substr(lpos, position - lpos);
	checkStartLine();
}

void Request::URLDecode(const std::string& encoded)
{
	std::string	decoded;
	char		ch;
	int			len = encoded.length();

	for (int i = 0; i < len; ++i)
	{
		if (encoded[i] == '%')
		{
			int hexValue;
			sscanf(encoded.substr(i + 1, 2).c_str(), "%x", &hexValue);
			ch = static_cast<char>(hexValue);
			decoded += ch;
			i +=2;
		}
		else if (encoded[i] == '+')
			decoded += ' ';
		else
			decoded += encoded[i];
	}
	_headerFields["Path"] = decoded;
}

void Request::checkStartLine()
{
	doesKeyExist(400, "Method", "Missing method.");
	std::string method = _headerFields["Method"];
	if (method != "POST" && method != "GET" && method != "DELETE")
		throw ErrorResponse(501, method);

	doesKeyExist(400, "Path", "Missing path.");
	std::string path = _headerFields["Path"];
	if (path.length() > 255)
		throw ErrorResponse(414, "Too long URI");

	doesKeyExist(400, "Version", "Missing version.");
	std::string version = _headerFields["Version"];
	size_t found = version.find("HTTP/1.1");
	if (found == std::string::npos)
		throw ErrorResponse(505, version);
}

void Request::checkHeaderFields()
{
	std::string method = _headerFields["Method"];

	if (method == "POST")
	{
		doesKeyExist(411, "Content-Length", "Missing header field.");
		if (_headerFields["Content-Length"].length() > 10)
			throw ErrorResponse(413, "Try a smaller file");
		else if (_headerFields["Content-Length"] == "0")
			throw ErrorResponse(400, "Lack of required content.");
		doesKeyExist(400, "Content-Type", "Missing header field.");
	}
		
}

void Request::parseHeaderFields(std::istringstream &iss)
{
	std::string key, value, line;
	int	position;

	while (getline(iss, line))
	{
		if (line.size() == 1) {
			_header_done = true;
			checkHeaderFields();
			if (_headerFields["Transfer-Encoding"] == "chunked") {
				_isChunked = true;
			}
			break ;
		}
		position = line.find(':');
		key = line.substr(0, position);
		position++;
		if (line[position] == ' ')
			position++;
		value = line.substr(position, line.find('\r') - position);
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

	_last_activity = time(NULL);
	int bytes_read = recv(socket, readBuffer, BUFLEN - 1, 0);
	if (bytes_read <= 0)
	{
		close(socket);
		return;
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
		std::stringstream tmp;
		tmp << _headerFields["Content-Length"];
		tmp >> _content_len;
	}


	if (_header_done && _isChunked) {
		if (_readCount < _content_len) {
			memset(readBuffer, 0, BUFLEN);
			iss.read(readBuffer, _content_len - _readCount + 2);
			size_t count = iss.gcount();
			_readCount += count;
			// We need to consider that there are possible conditions where the trailing \r\n gets send later
			// TODO: talk to the team about it
			_bodyBuffer += std::string(readBuffer, count);
			if (_readCount < _content_len) {
				return;
			}
			_readCount = 0;
		}
		std::string tmp;
		while (getline(iss, tmp)) {
			std::stringstream ss;
			ss << std::hex << tmp;
			ss >> _content_len;
			std::cout << "Content len: " << _content_len << std::endl;
			if (_content_len == 0) {
				_chunkedFinished = true;
				break;
			}
			memset(readBuffer, 0, BUFLEN);
			iss.read(readBuffer, _content_len + 2);
			size_t count = iss.gcount();
			_readCount += count;
			if (_readCount == _content_len + 2) {
				_bodyBuffer += std::string(readBuffer, count - 2);
			} else {
				_bodyBuffer += std::string(readBuffer, count);
				return;
			}
			_readCount = 0;
		}
	} else if (_header_done && _readCount < _content_len) {
		std::stringstream remainder;
		remainder << iss.rdbuf();
		_bodyBuffer += remainder.str();
		_readCount += remainder.str().length();
	}

	if (_header_done && (_readCount >= _content_len || _chunkedFinished)) {
		std::stringstream ss;
		ss << _bodyBuffer.length();
		_headerFields["Content-Length"] = ss.str();
	std::cout << "contentttt" << _headerFields["Content-Length"] << std::endl;
		if (_bodyBuffer.length() > 0 || _chunkedFinished) {
			// std::cout << _bodyBuffer << std::endl;
			parseBody(_bodyBuffer);
		}
		_isRead = true;
	}

	if (DEBUG)
	{
		std::cout << CYAN << "\nReceived message:\n\n" << DEF << readBuffer << std::endl;
		std::cout << CYAN << "Read count:\n" << DEF << _readCount << std::endl;
	}
	// _requestBuffer.append(readBuffer);
}

void	Request::getRequest(int	&socket)
{
	if (DEBUG)
		std::cout << CYAN << "\nGetting request...\n" << DEF;
	try {
		readIntoString(socket);
	} catch (const ErrorResponse& error) {
		_isRead = true;
		_response._hasError = true;
		_response._requestParsingError = error;
	}
}

std::string	Request::getResponse()
{
	return (_response.getResponse());
}

StringStringMap	Request::getHeaderFields()
{
	return (_headerFields);
}

bool	Request::isFlagOn()
{
	return (_isRead);
}

void Request::setError(const ErrorResponse &error) {
	_response._hasError = true;
	_isRead = true;
	_response._requestParsingError = error;
}

time_t Request::getLastActivity() {
	return _last_activity;
}

Request::Request(void) :
	_response(_headerFields),
	_isRead(false),
	_readCount(0),
	_first_line(false),
	_header_done(false),
	_isChunked(false),
	_chunkedFinished(false),
	_content_len(0),
	_last_activity(time(NULL))
{
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
