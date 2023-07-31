#ifndef REQUEST_CLASS_HPP
# define REQUEST_CLASS_HPP

#include "colours.hpp"
#include "Response.class.hpp"
#include "Error.class.hpp"
#include "webserv.hpp"

#define BUFLEN 120000
#define REACHED_HEADER_END 1
#define REACHED_BODY_END 2

class Request
{
	private:

		bool			_isRead;
		int				_readCount;
		std::string		_requestBuffer;
		StringStringMap	_headerFields;
		Response	_response;

		void			readIntoString(int &socket);
		std::string		readingRequest(int &socket);
		void			parseHeaderSection();
		void			parseStartLine(std::string startLine);
		void			parseHeaderFields(std::string headerSection);
		void			parseBody(std::string body);

	public:

		void			printMap();
		void		getRequest(int &socket);
		std::string		getResponse();
		StringStringMap	getHeaderFields();
		bool			isFlagOn();

		Request(void);
		Request&	operator=(Request const &assign);
		std::string	operator[](std::string const &key);
		~Request(void);
};

#endif
