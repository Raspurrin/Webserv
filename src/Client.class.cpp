
struct pollfd				Client::getFd()
{
	return(pollFd.fd);
}

int							Client::getEvent()
{
	return(pollFd.event);
}

int							Client::getRevent()
{
	return(pollFd.revent);
}

class Server&				Client::getServerRef()
{
	return(Server);
}

Client::Client(struct pollfd pollFd, class Server& Server) :
	pollFd(pollFd),
	Server(Server)
{
}

Client::Client(void)
{
}
//Client(Client const &src);
//Client&	operator=(Client const &assign);
Client::~Client(void)
{
}
