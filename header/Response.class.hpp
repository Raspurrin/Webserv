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
#include "webserv.hpp"
#include "colours.hpp"
#include "Error.class.hpp"

class Response
{
	private:

		StringStringMap	_response;
		StringStringMap&	_headerFields;
		std::string		_responseMessage;

		bool	listDir();
		void	methodID();
		std::string	getMimeType(const std::string& filename);
		void	checkRequestErrors();

		void	processRequest();
		void	assembleResponse();
		void	buildError(const t_status& _status);
		std::string	readTemplate();
		void	generateHTML(const t_status& status);

		void	GETMethod();
		void	DELETEMethod();
		void	POSTMethod();
		void	tryChdir(const char* path);

		void	status200(std::string path);
		void	status201();

		void	readFile();
		Response();

	public:

		bool	_hasError;
		ErrorResponse	_requestParsingError;
		void		postResponse(int socket, int indexToRemove);
		std::string	getResponse();

	public:
		Response(StringStringMap& _headerFields);
		//Response&	operator=(Response &assign);
		~Response(void);
};

#endif
