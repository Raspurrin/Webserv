#ifndef RESPONSE_CLASS_HPP
# define RESPONSE_CLASS_HPP

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <dirent.h>
#include <set>
#include <dirent.h>
#include "ServerConfig.class.hpp"
#include "webserv.hpp"
#include "colours.hpp"
#include "Error.class.hpp"

class Response
{
	private:
		bool				_firstCall;
		bool				_responseFinished;
		StringStringMap		_response;
		StringStringMap&	_headerFields;
		std::string			_responseMessage;
		std::stringstream	_responseStream;

		void	processRequest();
		void	checkRequestErrors();
		void	checkMethod();
		void	methodID(int method);
		void	setMethods(StringIntMap& methods);
		void	GETMethod();
		void	checkDirectory();
		bool	listDir();
		void	DELETEMethod();
		void	POSTMethod();


		void	readFile();
		std::string	getMimeType(const std::string& filename);
		void	setMimes(StringStringMap&);

		void	assembleResponse();

		void	buildError(const t_status& _status);
		void	generateHTML(const t_status& status);
		std::string	readTemplate(const t_status& _status);

		void	status200(std::string path);
		void	status201();

		void	printCWD();
		void	tryChdir(const char* path);
		void	directoryUpAndThrow(int error, std::string description);

		Response();

	public:

		bool			_hasError;
		bool			_getResponseFinished();
		ErrorResponse	_requestParsingError;
		ServerConfig	_serverConfig;
		void			postResponse(int socket, int indexToRemove);
		std::string		getResponse();

	public:
		Response(StringStringMap& _headerFields);
		//Response&	operator=(Response &assign);
		~Response(void);
};

#endif
