#ifndef REQUEST_CLASS_HPP
# define REQUEST_CLASS_HPP

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <map>
#include <sstream>
#include "colours.hpp"

class Request
{
	private:

		std::map<std::string, std::string>headerFields;
		std::map<std::string, std::string>response;

		const std::string	requestMessage;
		std::string			responseMessage;
		std::istringstream	iss;

		void	parseHeaderSection();
		void	parseStartLine(std::string startLine);
		void	parseHeaderFields(std::string headerSection);
		void	buildResponse();
		void	methodID();
		void	GETMethod();

	public:

		void	printMap();
		std::string	getResponse();

	public:

		Request(void);
		Request(std::string buffer);
		Request(Request const &src);
		Request&	operator=(Request const &assign);
		~Request(void);
};

#endif
