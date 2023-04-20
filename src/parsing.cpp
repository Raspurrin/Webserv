#include "../header/parsing.hpp"

namespace parsing
{
	std::string	trim(std::string str)
	{
		if (str.empty())
			return ("");
		int	first = str.find_first_not_of(" \t\n\r\v\f");
		int	last = str.find_last_not_of(" \t\n\r\v\f");

		return (str.substr(first, last + 1));
	}

	std::string	validate_trim(std::string str, int (*check_func)(int))
	{
		if (std::all_of(str.begin(), str.end(), check_func) == false)
			error_handle("Configuration key or value should contain only characters");
		return (trim(str));
	}

	void	extractKeyValue(std::string line, std::string &key, std::string &value)
	{
		int	equalSign = line.find('=');

		key = line.substr(0, equalSign - 1);
		value = line.substr(equalSign + 1);
		key = trim(key);
		value = trim(value);
	}

	template <typename Key, typename Value>
	void	printMap(const char *colour_header, const char *colour_body, std::map<Key, Value> &map)
	{
		std::cout << colour_header << "\nPrinting map:\n" << DEF << std::endl;
		for (typename std::map<Key, Value>::iterator it = map.begin(); it != map.end(); ++it)
			std::cout << colour_body << it->first << " " << DEF << it->second << std::endl;
		return;
	}
}
