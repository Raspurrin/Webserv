#ifndef RESPONSE_CLASS_HPP
# define RESPONSE_CLASS_HPP

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
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
		void	buildError(const ErrorType);

		void	GETMethod();
		void	DELETEMethod();
		void	POSTMethod();

		void	status200(std::string path);
		void	status201();
		void	status400();
		void	status403();
		void	status404();
		void	status409();
		void	status415();
		void	status500();
		void	status501();
		void	status505();

		void	readHTML();
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
