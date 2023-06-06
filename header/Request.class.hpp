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

#define BUFLEN 2000
#define REACHED_HEADER_END 1
#define REACHED_BODY_END 2

class Request
{
	private:
		int				bufferFlags;
		std::string		headerBuffer;
		std::string		bodyBuffer;

		void			readingBody(int &socket);
		void			readingHeader(int &socket);
		void			parseHeaderSection();
		void			parseStartLine(std::string startLine);
		void			parseHeaderFields(std::string headerSection);

	public:
		std::map<std::string, std::string>	headerFields;

		void			printMap();
		void			getRequest(int &socket);
		std::string		getResponse();

		Request(void);
		Request(std::string buffer);
		Request(Request const &src);
		Request&	operator=(Request const &assign);
		~Request(void);
};

#endif
