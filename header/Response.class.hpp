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
		void	GETMethod();
		void	buildResponse();
		void	buildError(const Error);
		int	status404();
		int	status403();
		void	status500();
		int	checkStat();
		void	status200();
		void	readHTML();
		std::string	lenToStr(std::string body);

	public:
		void		postResponse(int socket, int indexToRemove);
		std::string	getResponse();

	public:
		Response();
		Response(StringStringMap _headerFields);
		//Response&	operator=(Response &assign);
		~Response(void);
};

#endif
