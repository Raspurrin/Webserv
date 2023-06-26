#ifndef REQUEST_CLASS_HPP
# define REQUEST_CLASS_HPP

#include "colours.hpp"
#include "Response.class.hpp"
#include "webserv.hpp"

#define BUFLEN 512
#define REACHED_HEADER_END 1
#define REACHED_BODY_END 2

class Request
{
	private:
		int				bufferFlags;
		int				readCount;
		std::string		headerBuffer;
		std::string		bodyBuffer;
		std::map<std::string, std::string>	headerFields;

		std::string		buildResponse();
		void			readingBody(int &socket);
		std::string		readingHeader(int &socket);
		void			parseHeaderSection();
		void			parseStartLine(std::string startLine);
		void			parseHeaderFields(std::string headerSection);

	public:
		void			printBody(void);
		void			printMap();
		std::string		getRequest(int &socket);
		StringStringMap	getHeaderFields();
		bool			isFlagOn(int flag);

		Request(void);
		Request(Request const &src);
		Request&	operator=(Request const &assign);
		std::string	operator[](std::string const &key);
		~Request(void);
};

#endif