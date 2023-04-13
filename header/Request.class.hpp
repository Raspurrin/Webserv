#ifndef REQUEST_CLASS_HPP
# define REQUEST_CLASS_HPP

#include <iostream>
#include "colours.hpp"

class Request
{
	private:

		std::string	request;

	public:

		Request(void);
		Request(std::string request);
		Request(Request const &src);
		Request&	operator=(Request const &assign);
		~Request(void);
};

#endif

