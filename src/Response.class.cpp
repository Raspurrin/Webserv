#include "../header/Response.class.hpp"

int Response::getResponse(void) const
{
	return (this->number);
}

Response::Response(void)
{
	return ;
}

Response::Response(Response const &src)
{
	*this = src;
	return ;
}

Response &	Response::operator=(const Response &assign)
{
	this->number = assign.getResponse();
	return (*this);
}

Response::~Response(void)
{
	return ;
}

