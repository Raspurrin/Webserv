#ifndef REQUEST_CLASS_HPP
# define REQUEST_CLASS_HPP

#include <iostream>
#include "colours.hpp"

class Request
{
	private:

		const std::string	requestMessage;
		std::string	method;
		std::string path;
		std::string	protocolVersion;


		void parseHeaderSection();
		void parseRequestLine();
		void parseHeaderFields();

	public:

		Request(void);
		Request(const std::string requestMessage);
		Request(Request const &src);
		Request&	operator=(Request const &assign);
		~Request(void);
};

#endif

