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
#include "error.h"

class Response
{
	private:

		StringStringMap	_response;
		StringStringMap	_headerFields;
		std::string		_responseMessage;

		bool	listDir();
		void	methodID();
		void	checkRequestErrors();
		
		void	buildResponse();
		void	buildError(const Error);

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

	public:

		void		postResponse(int socket, int indexToRemove);
		bool		listDir();
		std::string	getResponse();

	public:
		Response();
		Response(StringStringMap _headerFields);
		//Response&	operator=(Response &assign);
		~Response(void);
};

#endif
