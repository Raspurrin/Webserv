#ifndef REQUEST_CLASS_HPP
# define REQUEST_CLASS_HPP

#include <iostream>
#include <map>
#include <sstream>
#include "colours.hpp"

class Request
{
	private:

		std::map<std::string, std::string>headerFields;

		const std::string	requestMessage;
		std::istringstream	iss;
		std::string	method;
		std::string path;
		std::string	protocolVersion;


		void parseHeaderSection();
		void parseStartLine(std::string startLine);
		void parseHeaderFields();

	public:

		Request(void);
		Request(const std::string requestMessage);
		Request(Request const &src);
		Request&	operator=(Request const &assign);
		~Request(void);
};

#endif

