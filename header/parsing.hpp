#ifndef PARSING_HPP
# define PARSING_HPP

#include "../header/webserv.hpp"

namespace parsing
{
	template <typename Key, typename Value>
	void		printMap(const char *colour_header, const char *colour_body, std::map<Key, Value> &map);
	void		extractKeyValue(std::string line, std::string &key, std::string &value);
	std::string	validate_trim(std::string str, int (*check_func)(int));
	std::string	trim(std::string str);
}

#endif
