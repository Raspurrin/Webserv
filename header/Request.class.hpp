#ifndef REQUEST_CLASS_HPP
# define REQUEST_CLASS_HPP

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <map>
#include <sstream>
#include "colours.hpp"
#include "Response.class.hpp"
#include "Client.class.hpp"

class Request
{
	private:
		std::map<std::string, std::string>	headerFields;
		const std::string					requestMessage;
		std::string							responseMessage;

	private:
		void	parseHeaderSection(Client &client);
		void	parseStartLine(std::string startLine);
		void	parseHeaderFields(std::string headerSection);

	public:
		void								printMap();
		std::string							getResponse();
		void								getRequest(int new_socket);
		std::map<std::string, std::string>	getMap();

	public:
		Request(void);
		Request(std::string buffer);
		Request(Request const &src);
		Request&	operator=(Request const &assign);
		~Request(void);
};

#endif
