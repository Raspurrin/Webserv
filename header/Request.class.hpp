#pragma once

#include "colours.hpp"
#include "Response.class.hpp"
#include "Error.class.hpp"
#include "webserv.hpp"
#include "ServerConfig.class.hpp"
#include <sstream>

#define BUFLEN 120000
#define REACHED_HEADER_END 1
#define REACHED_BODY_END 2

class Request
{
	private:

		StringStringMap	_headerFields;
		Response		_response;
		bool			_isRead;
		uint64_t		_readCount;
		bool			_first_line;
		bool			_header_done;
		bool			_isChunked;
		bool			_chunkedFinished;
		uint64_t		_content_len;
		time_t			_last_activity;
		std::string		_bodyBuffer;
		std::string		_requestBuffer;

		void			readIntoString(int &socket);
		std::string		readingRequest(int &socket);
		void			parseHeaderSection();
		void			parseStartLine(std::string startLine);
		void			parseHeaderFields(std::istringstream &iss);
		void			parseBody(std::string body);
		void			URLDecode(const std::string& encoded);
		void			checkBoundary(const std::string&);
		void			extractingFilename();
		void			doesKeyExist(int, const std::string&, const std::string&);
		void			checkHeaderFields();
		void			checkStartLine();
		void			checkValueSize(const std::string&, const std::string&);
		void			whenDoneParsingHeader();
		void			whenReadSmallerThanLen(std::istringstream&, char*);

	public:

		void			printMap();
		void			getRequest(int &socket, ServerConfig &serverConfig);
		std::string		getResponse();
		StringStringMap	getHeaderFields();
		bool			isFlagOn();
		void			setError(const ErrorResponse &error);
		time_t			getLastActivity();
		bool			responseFinished();

		Request(void);
		Request&	operator=(Request const &assign);
		std::string	operator[](std::string const &key);
		~Request(void);
};
