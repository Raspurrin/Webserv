#ifndef RESPONSE_CLASS_HPP
# define RESPONSE_CLASS_HPP

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <dirent.h>
#include "colours.hpp"

class Response
{
	private:

		std::map<std::string, std::string> response;
		std::map<std::string, std::string> request;
		std::string	responseMessage;

		bool	listDir();
		void	methodID();
		void	GETMethod();
		void	buildResponse();
		void	status404();
		void	status403();
		void	status200();
		void	readHTML();
		std::string	lenToStr(std::string body);


	public:

		std::string	getResponse(void) const;

		Response(std::map<std::string, std::string> request);
		Response&	operator=(Response const &assign);
		~Response(void);
};

#endif

