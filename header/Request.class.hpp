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

		int					fd;
		const std::string	requestMessage;
		std::istringstream	iss;
		std::string	method;
		std::string path;
		std::string	protocolVersion;

		void	parseHeaderSection();
		void	parseStartLine(std::string startLine);
		void	parseHeaderFields(std::string headerSection);

	public:
		void	printMap();

	public:

		Request(void);
		Request(int fd);
		Request(Request const &src);
		Request&	operator=(Request const &assign);
		~Request(void);
};

#endif
