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
		int	status404();
		int	status403();
		int	checkStat();
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

