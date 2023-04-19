#ifndef RESPONSE_CLASS_HPP
# define RESPONSE_CLASS_HPP

#include <iostream>
#include "colours.hpp"

class Response
{
	private:


	public:

		int	getResponse(void) const;

		Response(void);
		Response(Response const &src);
		Response&	operator=(Response const &assign);
		~Response(void);
};

#endif

