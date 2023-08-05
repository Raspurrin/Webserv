#include "../header/Response.class.hpp"
#include "../header/Cgi.class.hpp"
#include "../header/utils.hpp"
#include <ios>

Response::Response(StringStringMap& _headerFields) : _headerFields(_headerFields), _hasError(false)
{
	if (DEBUG)
		std::cout << CYAN << "\nIn response constructor...\n\n" << DEF;
}

void Response::checkRequestErrors()
{
	if (_hasError)
		throw _requestParsingError;
}

void Response::methodID()
{
	// "/" will always be a directory, so maybe we should solve this with a route later on?
	if (_headerFields["Path"] == "/")
		_headerFields["Path"] = "/index.html";

	if (_headerFields["Method"] == "GET")
		GETMethod();
	if (_headerFields["Method"] == "POST")
		POSTMethod();
	if (_headerFields["Method"] == "DELETE")
		DELETEMethod();
	return ;
}

void Response::GETMethod()
{
	struct	stat s;
	const char *path = _headerFields["Path"].c_str() + 1;

	if (access(path, F_OK) == -1)
		throw ErrorResponse(Not_Found, "in GETMethod, file doesnt exist");
	if (access(path, R_OK) == -1)
		throw ErrorResponse(Forbidden, "No access rights, in GETMethod");
	if (stat(path, &s) == 0)
	{
		if (S_ISDIR(s.st_mode))
		{
			if (listDir())
				status200("/temp.html");
			else
				status200("/directory.html");
		}
		else if (S_ISREG(s.st_mode))
			status200(_headerFields["Path"]);
		else
			throw ErrorResponse(Internal_Error, "in GETMethod()");
	}
	else
		throw ErrorResponse(Internal_Error);
}

void Response::POSTMethod()
{
	if (_headerFields["Path"] != "/files/")
		throw ErrorResponse(Forbidden, "Not matching Path with /files/ in POSTMethod()");
	chdir("./files");
	std::ofstream outfile(_headerFields["Filename"].c_str());
	if (outfile.good())
	{
		outfile << _headerFields["Body-Text"] << std::endl;
		outfile.close();
		chdir("..");
		status201();
	}
	else
	{
		chdir("..");
		throw ErrorResponse(Internal_Error, "Internal Error when creating file in POSTMethod");
	}
}

void Response::DELETEMethod()
{
	std::string path = _headerFields["Path"];

	if (path.rfind("/files/", 0) == std::string::npos)
		throw ErrorResponse(Forbidden);

	int start = path.find_last_of('/');
	_headerFields["Filename"] = path.substr(start + 1);
	const char *filename = _headerFields["Filename"].c_str();

	chdir("./files");

	if (access(filename, F_OK) == -1)
	{
		chdir("..");
		throw ErrorResponse(Not_Found, "File not found in DELETEMethod()");
	}

	std::fstream file(filename, std::ios::in);
	if (!file.is_open())
	{
		chdir("..");
		throw ErrorResponse(Conflict, "File in use, DELETEMethod");
	}
	file.close();

	int rem = std::remove(filename);
	chdir("..");
	if (rem != 0)
		throw ErrorResponse(Internal_Error, "Removing file didnt work, in DELETEMethod");
	else
		status200("/error_pages/deleted.html");
}

void Response::buildError(const ErrorType _errorType) {
	switch (_errorType)
	{
	case Bad_Request:
		status400();
		break;

	case Unsupported_Media_Type:
		status415();
		break;

	case Not_Implemented:
		status501();
		break;

	case HTTP_Version_Not_Supported:
		status505();
		break;

	case Forbidden:
		status403();
		break;

	case Not_Found:
		status404();
		break;

	case Request_Timeout:
		status408();
		break;

	case Conflict:
		status409();
		break;

	case Internal_Error:
		status500();
		break;

	default:
		break;

	}
}

void Response::assembleResponse()
{
	_responseMessage += _response["Version"] + " "
		+ _response["Status code"] + "\n";

	for(StringStringMap::iterator it = _response.begin(); it != _response.end(); it++)
	{
		if ((*it).first.find(':') != std::string::npos)
		{
			_responseMessage += (*it).first + " " + (*it).second + "\n";
		}
	}
	_responseMessage += "Connection: close\r\n\r\n"
		+ _response["Body"];

	if (DEBUG)
		std::cout << CYAN << "RESPONSE MESSAGE: \n\n" << DEF << _responseMessage << "\n\n";
}

void Response::processRequest()
{
	try
	{
		_response["Version"] = _headerFields["Version"];
		checkRequestErrors();
		methodID();
	}
	catch (const std::exception &e)
	{
		const ErrorResponse *_errorType = dynamic_cast<const ErrorResponse *>(&e);
		std::cout << "Catched exception " << e.what() << std::endl;
		if (_errorType != NULL)
			buildError(_errorType->getError());
		else
			buildError(Internal_Error);
	}
	return ;
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

	if (getcwd(cwd, 256) != NULL)
		dir = opendir((cwd + _headerFields["Path"]).c_str());
	else
		return false;

	if (dir != NULL)
	{
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

		std::ofstream outfile("temp.html");
		outfile << body << std::endl;
		outfile.close();
		return true;
	}
	else
		return false;
}

void Response::readFile()
{
	std::ifstream	fin(_response["Path"].c_str() + 1, std::ios::binary);
	std::string	content;

	if (!fin)
		throw ErrorResponse(Internal_Error, "Internal Error in readHtml");
	fin.seekg(0, std::ios::end);
	std::streampos fileSize = fin.tellg();
	fin.seekg(0, std::ios::beg);
	content.resize(fileSize);
	fin.read(&content[0], fileSize);
	fin.close();
	if (!fin)
		throw ErrorResponse(Internal_Error, "Internal Error in readHtml");
	_response["Body"] = content;
	_response["Content-Length:"] = lenToStr(content);
	_response["Content-Type:"] = getMimeType(_response["Path"]);
}

std::string Response::getMimeType(const std::string& filename)
{
	static StringStringMap mimeTypes;

	if (mimeTypes.empty())
	{
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

void Response::status200(std::string path)
{
	_response["Status code"] = "200 OK";
	_response["Path"] = path;
}

void Response::status201()
{
	_response["Status code"] = "201 CREATED";
	_response["Path"] = "/success.html";
	_response["Location:"] = _headerFields["Path"].append(_headerFields["Filename"]);
}

void Response::status400()
{
	_response["Status code"] = "400 Bad Request";
	_response["Path"] = "/error_pages/400.html";
}

void Response::status403()
{
	_response["Status code"] = "403 Forbidden";
	_response["Path"] = "/error_pages/403.html";
}

void Response::status404()
{
	_response["Status code"] = "404 Not Found";
	_response["Path"] = "/error_pages/404.html";
}

void Response::status408() {
	_response["Status code"] = "408 Request Timeout";
	_response["Path"] = "/error_pages/408.html";
}

void Response::status409()
{
	_response["Status code"] = "409 Conflict";
	_response["Path"] = "/error_pages/409.html";
}

void Response::status415()
{
	_response["Status code"] = "415 Unsupported Media Type";
	_response["Path"] = "/error_pages/415.html";
}

void Response::status500()
{
	_response["Status code"] = "500 Internal Server Error";
	_response["Path"] = "/error_pages/500.html";
}

void Response::status501()
{
	_response["Status code"] = "501 Not Implemented";
	_response["Path"] = "/error_pages/501.html";
}

void Response::status505()
{
	_response["Status code"] = "505 HTTP Version Not Supported";
	_response["Version"] = "HTTP/1.1";
	_response["Path"] = "/error_pages/505.html";
}

std::string	Response::getResponse()
{
	processRequest();
	readFile();
	assembleResponse();
	return (_responseMessage);
}

// Response &	Response::operator=(Response &assign)
// {
// 	this->_responseMessage = assign.getResponse(_headerFields);
// 	return (*this);
// }


Response::~Response(void)
{
}
