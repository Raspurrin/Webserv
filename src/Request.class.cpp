#include "../header/Request.class.hpp"

void Request::parseHeaderSection()
{
	parseRequestLine();
	parseHeaderFields();
	return ;
}

void Request::parseRequestLine()
{
	return ;
}

void Request::parseHeaderFields()
{
	return ;
}

Request::Request(void)
{
	return ;
}

Request::Request(const std::string requestMessage) : requestMessage(requestMessage)
{
	parseHeaderSection();
	return ;
}

Request::Request(Request const &src)
{
	*this = src;
	return ;
}

Request &	Request::operator=(const Request &assign)
{
	return (*this);
}

Request::~Request(void)
{
	return ;
}

