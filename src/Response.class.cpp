#include "../header/Response.class.hpp"
#include "../header/Cgi.class.hpp"
#include "../header/utils.hpp"
#include <cstdio>
#include <fstream>
#include <ios>
#include <string>
#include <unistd.h>
#include <utility>

Response::Response(StringStringMap& _headerFields) : _firstCall(true), _responseFinished(false), _headerFields(_headerFields), _hasError(false)
{
	if (DEBUG)
		std::cout << CYAN << "\nIn response constructor...\n\n" << DEF;
}

std::string	Response::getResponse()
{
	if (_firstCall) {
		tryChdir("www");
		processRequest();
		readFile();
		assembleResponse();
		_responseStream << _responseMessage;
		_firstCall = false;
	}
	char _responseBuff[BUFLEN] = {0};
	_responseStream.read(_responseBuff, BUFLEN);
	std::string response(_responseBuff, BUFLEN);

	if (_responseStream.eof()) {
		tryChdir("..");
		_responseFinished = true;
	}

	return (response);
}

void Response::processRequest()
{
	try
	{
		checkRequestErrors();
		checkMethod();
	}
	catch (const std::exception &e)
	{
		const ErrorResponse *_errorType = dynamic_cast<const ErrorResponse *>(&e);
		std::cout << "Catched exception " << e.what() << std::endl;
		if (_errorType != NULL)
			buildError(_errorType->getError());
	/* else */
		/* 	buildError(Internal_Error); */
	}
	return ;
}

void Response::checkRequestErrors()
{
	if (_hasError)
		throw _requestParsingError;
}

void Response::checkMethod()
{
	_serverConfig.printServerConfig();
	std::string route = _headerFields["Route"];
	std::string method = _headerFields["Method"];
	static StringIntMap methods;

	if (!_serverConfig.isRouteValid(route))
		throw ErrorResponse(404, "Route not configured.");

	setMethods(methods);
	if (methods.find(method) == methods.end())
		throw ErrorResponse(501, method);
	if (!_serverConfig.isRouteMethodAllowed(route, methods[method]))
		throw ErrorResponse(405, method);

	methodID(methods[method]);
}

void Response::checkDirectory()
{
	std::string route = _headerFields["Route"];

	std::string index = _serverConfig.getRouteIndex(route);
	if (index.empty())
	{
		if (!listDir())
			status200("/directory.html");
	}
	else
	{
		std::stringstream build;
		build << "/" << route << "/" << index;
		status200(build.str());
	}
}

void Response::GETMethod()
{
	struct	stat s;
	const char *path = _headerFields["Path"].c_str() + 1;
	std::cout << "path in get" << path << std::endl;
	printCWD();

	if (access(path, F_OK) == -1)
		throw ErrorResponse(404, "GET: File doesn't exist.");
	if (access(path, R_OK) == -1)
		throw ErrorResponse(403, "GET: No access rights.");
	if (stat(path, &s) != 0)
		throw ErrorResponse(500, "GET: Error fetching file status.");
	if (S_ISDIR(s.st_mode))
		checkDirectory();
	else if (S_ISREG(s.st_mode))
		status200(_headerFields["Path"]);
	else
		throw ErrorResponse(400, "GET, Not regular file.");
}

void Response::POSTMethod()
{
	const char *filename = _headerFields["Upload-Filename"].c_str();

	if (_headerFields["Path"] != "/files/")
		throw ErrorResponse(403, "POST: Not matching Path with /files/");
	tryChdir("./files");
	if (access(filename, F_OK) == 0)
		directoryUpAndThrow(409, "POST: Filename already exists.");
	std::ofstream outfile(filename);
	tryChdir("..");
	if (!outfile.is_open() || !outfile.good())
		throw ErrorResponse(500, "POST: When creating file.");
	outfile << _headerFields["Body-Text"] << std::endl;
	outfile.close();
	status201();
}

void Response::DELETEMethod()
{
	std::string path = _headerFields["Path"];

	if (path.rfind("/files/", 0) == std::string::npos)
		throw ErrorResponse(403, "DELETE: No rights to delete from other directories");

	int start = path.find_last_of('/');
	_headerFields["Filename"] = path.substr(start + 1);
	const char *filename = _headerFields["Filename"].c_str();

	tryChdir("./files");

	if (access(filename, F_OK) == -1)
		directoryUpAndThrow(404, "DELETE: File not found.");

	std::fstream file(filename, std::ios::in);
	if (!file.is_open())
		directoryUpAndThrow(409, "DELETE: File in use.");
	file.close();

	int rem = std::remove(filename);
	tryChdir("..");
	if (rem != 0)
		throw ErrorResponse(500, "DELETE: Delete file unsuccesful.");
	status200("/error_pages/deleted.html");
}

void Response::readFile()
{
	if (_response.count("Body") > 0)
		return ;
	std::ifstream	fin(_response["Path"].c_str() + 1, std::ios::binary);
	std::string	content;

	if (!fin)
		throw ErrorResponse(500, "When reading file");
	fin.seekg(0, std::ios::end);
	std::streampos fileSize = fin.tellg();
	fin.seekg(0, std::ios::beg);
	content.resize(fileSize);
	fin.read(&content[0], fileSize);
	fin.close();
	if (!fin)
		throw ErrorResponse(500, "After reading file.");
	_response["Body"] = content;
	_response["Content-Type:"] = getMimeType(_response["Path"]);
}

std::string Response::getMimeType(const std::string& filename)
{
	static StringStringMap mimeTypes;

	setMimes(mimeTypes);

	size_t	dotPos = filename.find_last_of('.');
	if (dotPos != std::string::npos)
	{
		std::string extension = filename.substr(dotPos);
		StringStringMap::iterator it = mimeTypes.find(extension);
		if (it != mimeTypes.end())
			return (it->second);
	}
	return ("application/octet-stream");
}

void Response::assembleResponse()
{
	_response["Content-Length:"] = lenToStr(_response["Body"]);
	_response["Version"] = "HTTP/1.1";

	_responseMessage += _response["Version"] + " "
		+ _response["Status code"] + "\n";

	for(StringStringMap::iterator it = _response.begin(); it != _response.end(); it++)
	{
		if ((*it).first.find(':') != std::string::npos)
			_responseMessage += (*it).first + " " + (*it).second + "\n";
	}
	_responseMessage += "Connection: close\r\n\r\n"
		+ _response["Body"];

	if (DEBUG)
		std::cout << CYAN << "RESPONSE MESSAGE: \n\n" << DEF << _responseMessage << "\n\n";
}

void Response::buildError(const t_status& _status) {
	std::stringstream ss;
	ss << _status._code << " " << _status._description;
	_response["Status code"] = ss.str();
	//use getErrorPage from config file with pair first
	//if default
	//generate html
	generateHTML(_status);
	//else
	//save string into path
}

void Response::generateHTML(const t_status& _status)
{
	std::string htmlTemplate = readTemplate();
	size_t pos = htmlTemplate.find("{{TITLE}}");
	if (pos != std::string::npos) {
		std::stringstream ss;
		ss << _status._code;
		htmlTemplate.replace(pos, 9, ss.str());
	}
	pos = htmlTemplate.find("{{DESCRIPTION}}");
	if (pos != std::string::npos) {
		htmlTemplate.replace(pos, 15, _status._description);
	}
	pos = htmlTemplate.find("{{MESSAGE}}");
	if (pos != std::string::npos) {
		htmlTemplate.replace(pos, 11, _status._message);
	}
	_response["Body"] = htmlTemplate;
	_response["Content-Type:"] = "text/html";
}

std::string Response::readTemplate() {
	std::ifstream	file("template.html");
	if (!file)
		return ("ifstream error");
	std::string templateContent, line;
	while (getline(file, line))
		templateContent += line + "\n";
	file.close();
	return (templateContent);
}

void Response::status200(std::string path)
{
	_response["Status code"] = "200 OK";
	_response["Path"] = path;
}

void Response::status201()
{
	_response["Status code"] = "201 CREATED";
	_response["Path"] = "/success.html";
	_response["Location:"] = _headerFields["Path"].append(_headerFields["Upload-Filename"]);
}

void	Response::printCWD()
{
	char cwd[256];
	std::cout << "CWD" << getcwd(cwd, 256) << std::endl;
}

/**
 * Writes a html page containing a List of files in the current directory
 * to the body of the _response object.
 *
 * Returns true if successfull, otherwise false.
*/
bool Response::listDir()
{
	char cwd[256];
	DIR *dir;

	if (getcwd(cwd, 256) == NULL)
		return false;
	dir = opendir((cwd + _headerFields["Path"]).c_str());

	if (dir == NULL)
		return false;
	struct dirent *ent;

	// TODO: using a set makes it easier to sort the entries but has longer blocking time than an unsortet list. Need to investigate if it is too long.
	std::set<std::string> files;

	// Return value of readdir is statically allocated and must not be freed!
	while ((ent = readdir(dir)) != NULL)
		files.insert(std::string(ent->d_name));

	closedir(dir);

	std::string body = "<h1>Content of " + _headerFields["Path"] + "</h1>";

	const char *insert = _headerFields["Path"][_headerFields["Path"].size() - 1] == '/' ? "" : "/";
	for (std::set<std::string>::iterator it = files.begin(); it != files.end(); it++)
		body += "<a href=\"" + _headerFields["Path"] + insert + *it + "\">" + *it + "</a><br>";

	_response["Body"] = body;
	_response["Content-Type:"] = "text/html";
	_response["Status code"] = "200 OK";
	return true;
}

void Response::directoryUpAndThrow(int error, std::string description)
{
	tryChdir("..");
	throw ErrorResponse(error, description);
}

void Response::tryChdir(const char* path)
{
	if (chdir(path) == -1)
		throw ErrorResponse(500, strerror(errno));
}

void Response::setMimes(StringStringMap& mimeTypes)
{
	if (!mimeTypes.empty())
		return ;
	mimeTypes[".txt"] = "text/plain";
	mimeTypes[".html"] = "text/html";
	mimeTypes[".htm"] = "text/html";
	mimeTypes[".css"] = "text/css";
	mimeTypes[".js"]  ="text/javascript";
	mimeTypes[".json"] = "application/json";
	mimeTypes[".xml"] = "application/xml";
	mimeTypes[".pdf"] = "application/pdf";
	mimeTypes[".zip"] = "application/zip";
	mimeTypes[".doc"] = "application/msword";
	mimeTypes[".ppt"] = "application/vnd.ms-powerpoint";
	mimeTypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	mimeTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	mimeTypes[".png"] = "image/png";
	mimeTypes[".jpg"] = "image/jpeg";
	mimeTypes[".jpeg"] = "image/jpeg";
	mimeTypes[".gif"] = "image/gif";
}

void Response::setMethods(StringIntMap& methods)
{
	if (!methods.empty())
		return ;
	methods["GET"] = 1;
	methods["POST"] = 2;
	methods["DELETE"] = 4;
}

void Response::methodID(int method)
{

	switch (method) {
		case 1:
			GETMethod();
			break ;
		case 2:
			POSTMethod();
			break ;
		case 4:
			DELETEMethod();
			break ;
	}
}

bool Response::_getResponseFinished() {
	return _responseFinished;
}

// Response &	Response::operator=(Response &assign)
// {
// 	this->_responseMessage = assign.getResponse(_headerFields);
// 	return (*this);
// }

Response::~Response(void)
{
}
