#include "ConfigParsing.hpp"
#include "Server.hpp"
#include "WebservException.hpp"
#include <cstdlib>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
		std::cerr << "Error\n  Usage : ./webserv <config file>" << std::endl;
		return EXIT_FAILURE;
    }
    /*Server server;*/
    ConfigParsing configParser;

    std::vector<ConfigData>::iterator server_it;

    for (server_it = configParser.configData.begin(); server_it != configParser.configData.end(); server_it++)
    {
        std::cout << "i was here\n";
        server_it->printData();
    }
    try{
        configParser.start(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << e.what();
    }
    return (0);
}
