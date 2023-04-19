#ifndef RESPONSE_CLASS_HPP
# define RESPONSE_CLASS_HPP

#include <iostream>
#include <map>
#include "colours.hpp"

class Response
{
	private:

		std::map<std::string, std::string> response;
		std::map<std::string, std::string> request;
		std::string	responseMessage;

		void	methodID();
		void	GETMethod();
		void	buildResponse();


	public:

		std::string	getResponse(void) const;

		Response(std::map<std::string, std::string> request);
		Response&	operator=(Response const &assign);
		~Response(void);
};

#endif

