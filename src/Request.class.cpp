#include "../header/Request.class.hpp"
#include <cstring>
#include <sstream>

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

void	Request::getRequest(int	&socket, ServerConfig &serverConfig)
{
	_response._serverConfig = serverConfig;
	if (DEBUG)
		std::cout << CYAN << "\nGetting request...\n" << DEF;
	try {
		readIntoString(socket);
	} catch (const ErrorResponse& error) {
		if (DEBUG)
		{
			printMap();
		}
		_isRead = true;
		_response._hasError = true;
		_response._requestParsingError = error;
	}
}

void Request::readIntoString(int &socket)
{
	char	readBuffer[BUFLEN] = {0};

	_last_activity = time(NULL);
	int bytes_read = recv(socket, readBuffer, BUFLEN - 1, 0);
	if (bytes_read <= 0)
		return;
	if (strstr(readBuffer, "SERVER_SHUTDOWN") != NULL)
		std::cout << "Server shutting down..." << std::endl;
	// We need the number of bytes read here since we cant be sure that we didnt read any zero bytes,
	// which would lead to a truncation of the string.
	std::string read(readBuffer, bytes_read);

	if (!_first_line && read.find("\r\n") != std::string::npos) {
		parseStartLine(read.substr(0, read.find("\r\n")));
		read = read.substr(read.find("\r\n") + 2);
	}

	std::istringstream iss(read);
	parseHeaderFields(iss);

	if (_content_len == 0 && _headerFields.find("Content-Length") != _headerFields.end()) {
		std::stringstream tmp;
		tmp << _headerFields["Content-Length"];
		tmp >> _content_len;
	}

	if (_header_done && _isChunked) {
		if (_readCount < _content_len) {
			whenReadSmallerThanLen(iss, readBuffer);
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
		if ((_bodyBuffer.length() > 0 || _chunkedFinished) && _response._serverConfig.getRouteCGI(_headerFields["Route"]).empty()) {
			parseBody(_bodyBuffer);
		} else if ((_bodyBuffer.length() > 0 || _chunkedFinished) && !_response._serverConfig.getRouteCGI(_headerFields["Route"]).empty()) {
			_headerFields["Body"] = _bodyBuffer;
		}
		_isRead = true;
	}

	if (DEBUG)
	{
		std::cout << CYAN << "\nReceived message:\n\n" << DEF << readBuffer << std::endl;
		std::cout << CYAN << "Read count:\n" << DEF << _readCount << std::endl;
		printMap();
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
	URLDecode(startLine.substr(lpos, position - lpos));
	position++;
	lpos = position;
	position = startLine.find(' ', lpos);
	_headerFields["Version"] = startLine.substr(lpos, position - lpos);
	_first_line = true;
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
	separatingPathAndFilename();

	doesKeyExist(400, "Version", "Missing version.");
	std::string version = _headerFields["Version"];
	size_t found = version.find("HTTP/1.1");
	if (found == std::string::npos)
		throw ErrorResponse(505, version);
}

void Request::parseHeaderFields(std::istringstream &iss)
{
	if (_header_done)
		return ;
	std::string key, value, line;
	int	position;

	while (getline(iss, line))
	{
		if (line.size() == 1) {
			whenDoneParsingHeader();
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
		checkValueSize(key, value);
		_headerFields[key] = value;
	}
}

void Request::checkHeaderFields()
{
	std::string method = _headerFields["Method"];

	if (method == "POST")
	{
		if (!_isChunked) {
			doesKeyExist(411, "Content-Length", "Missing header field.");
			int content_length = atoi(_headerFields["Content-Length"].c_str());
			if (content_length > _response._serverConfig.getClientBodySize())
				throw ErrorResponse(413, "Content is bigger than set in config file.");
			else if (_headerFields["Content-Length"] == "0")
				throw ErrorResponse(400, "Lack of required content.");
		}

		doesKeyExist(400, "Content-Type", "Missing content type field.");
		std::string content_type = _headerFields["Content-Type"];
		size_t found = content_type.find("multipart/form-data");
		if (found == std::string::npos)
			throw ErrorResponse(415, content_type);
	}
}

void Request::parseBody(std::string body)
{
	std::string line;
	std::istringstream	ss(body);

	getline(ss, line);
	checkBoundary(line);
	getline(ss, _headerFields["Body-Disposition"]);
	extractingFilenameToUpload();
	getline(ss, _headerFields["Body-Type"]);
	getline(ss, line);
	std::stringstream remainder;
	remainder << ss.rdbuf();
	const std::string tmp = remainder.str();
	size_t idx = tmp.find_last_of(_headerFields["Boundary"]);
	_headerFields["Body-Text"] = tmp.substr(0, idx - _headerFields["Boundary"].length() - 4);
}

void Request::whenReadSmallerThanLen(std::istringstream& iss, char* readBuffer)
{
	memset(readBuffer, 0, BUFLEN);
	iss.read(readBuffer, _content_len - _readCount + 2);
	size_t count = iss.gcount();
	_readCount += count;
	// We need to consider that there are possible conditions where the trailing \r\n gets send later
	// TODO: talk to the team about it
	_bodyBuffer += std::string(readBuffer, count);
	if (_bodyBuffer.size() > (size_t)_response._serverConfig.getClientBodySize()) {
		throw ErrorResponse(413, "Body of chunked request is biger than max body size.");
	}
}

void Request::checkValueSize(const std::string& key, const std::string& value)
{
	if (value.length() > 9000)
		throw ErrorResponse(431, key);
}

void Request::whenDoneParsingHeader()
{
	_header_done = true;
	if (_headerFields.count("Transfer-Encoding") > 0)
	{
		if (_headerFields["Transfer-Encoding"] == "chunked") {
			_isChunked = true;
		}
	}
	checkHeaderFields();
}

void Request::doesKeyExist(int error, const std::string& key, const std::string& message)
{
	if (_headerFields.count(key) == 0)
		throw ErrorResponse(error, message);
}

void Request::checkBoundary(const std::string& line)
{
	size_t	found;

	_headerFields["Boundary"] = _headerFields["Content-Type"].substr(_headerFields["Content-Type"].find('=') + 1);
	found = line.find(_headerFields["Boundary"]);
	if (found == std::string::npos)
		throw ErrorResponse(400, "No boundary found in payload.");
}

void Request::separatingPathAndFilename()
{
	const std::string& path = _headerFields["Path"];
	const std::string& route = _headerFields["Route"];
	const std::string& method = _headerFields["Method"];
	static StringIntMap methods;

	if (path == "/") {
		_headerFields["Route"] = "/";
	} else {
		StringRouteMap &routes = (StringRouteMap &)_response._serverConfig.getRoutesMap();
		StringRouteMap::reverse_iterator rit = routes.rbegin();
		for (; rit != routes.rend(); rit++) {
			size_t pos = path.find(rit->first, 1);
			if (pos == 1)
				_headerFields["Route"] = rit->first;
		}
	}
	std::cout << "ROUTE: " << route << std::endl;

	if (!_response._serverConfig.isRouteValid(route))
		throw ErrorResponse(404, "Route not configured.");
	checkRoot(route);
	setMethods(methods);

	if (methods.find(method) == methods.end())
		throw ErrorResponse(501, method);
	if (!_response._serverConfig.isRouteMethodAllowed(route, methods[method]))
		throw ErrorResponse(405, method);
}

void Request::checkRoot(const std::string& route)
{
	std::string root = _response._serverConfig.getRouteRoot(route);

	if (root.empty())
		return ;
	if (root[0] == '/' && _headerFields["Path"].length() != 1)
		root = root.substr(1);
	size_t pos = 0;
	_headerFields["Path_Info"] = _headerFields["Path"];
	pos = _headerFields["Path"].find(route, pos);
	if (pos != std::string::npos)
		_headerFields["Path"].replace(pos, route.length(), root);
	_headerFields["Root"] = root;
}

void Request::setMethods(StringIntMap& methods)
{
	if (!methods.empty())
		return ;
	methods["GET"] = 1;
	methods["POST"] = 2;
	methods["DELETE"] = 4;
}

void Request::extractingFilenameToUpload()
{
	size_t	found, lpos;
	std::string body_disposition = _headerFields["Body-Disposition"];

	found = body_disposition.find_last_of('"');
	found -= 1;
	lpos = found;
	while (body_disposition[lpos] != '"')
		lpos--;
	_headerFields["Upload-Filename"] = body_disposition.substr(lpos + 1, found - lpos);
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

bool	Request::responseFinished() {
	return _response._getResponseFinished();
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
