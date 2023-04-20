#include "../header/Route.class.hpp"

Route::Route(std::string extractedRoute, std::string line)
{
	std::string key, value;
	std::stringstream extractedStream(extractedRoute);
	line = parsing::trim(line);
	name = line.substr(1, line.size() - 1);
	getline(extractedStream, line);
	parsing::extractKeyValue(line, key, value);
	if (key != "methods")
		error_handle("Route should start with  methods");
	
}

Route::Route(void)
{
}


// Route::Route(Route const &src)
// {
// }

// Route&	Route::operator=(Route const &assign)
// {
// }

Route::~Route(void)
{
}
