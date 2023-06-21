 #include "ParsingConfig.hpp"
 #include "ServerConfig.class.hpp"

 //hardcoded for testing xd
std::vector<ServerConfig>    ParsingConfig::parsing(int fd)
{
    std::map<std::string, std::string> map1;
    map1["404"] = "Not Found";
    t_route route1;
    std::vector<ServerConfig>    serverConfigs;
    ServerConfig config(8080, "MyServer", 1024, \
                        map1, \
                        {{"route1", {1, true, "http://example.com", "/root", "index.html"}}});
    // ServerConfig config2(8080, "MyServer", 1024, {{"404", "Not Found"}, {"500", "Internal Server Error"}}, \
    //                 {{"route1", {1, true, "http://example.com", "/root", "index.html"}}, \
    //                  {"route2", {2, false, "", "/path", "default.html"}}});
    serverConfigs.push_back(config);
    // serverConfigs.push_back(config2);
    return (serverConfigs);
}

 ParsingConfig::ParsingConfig(void)
 {

 }

 ParsingConfig::~ParsingConfig(void)
{

}