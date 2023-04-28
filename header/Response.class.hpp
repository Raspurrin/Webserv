#ifndef RESPONSE_CLASS_HPP
# define RESPONSE_CLASS_HPP

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <map>
#include "colours.hpp"

class Response
{
	private:

		std::map<std::string, std::string>	response;
		std::map<std::string, std::string>	request;
		std::string							responseMessage;

		void		methodID();
		void		GETMethod();
		void		buildResponse();
		void		status404();
		void		status403();
		void		status200();
		std::string	lenToStr(std::string body);

	public:
		void		postResponse(int socket, int indexToRemove);
		std::string	getResponse(void) const;

	public:
		Response();
		Response&	operator=(Response const &assign);
		~Response(void);
};

#endif
