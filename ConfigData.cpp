#include "ConfigData.hpp"
#include "utils.hpp"

ConfigData::ConfigData(){}
ConfigData::~ConfigData(){}

std::vector<std::string> ConfigData::getContent()
{
    return _content;
}

std::string ConfigData::getHost()
{
    return _host;
}

int ConfigData::getPort()
{
    return _port;
}

std::string ConfigData::getServerName()
{
    return _server_name;
}

std::string ConfigData::getRoot()
{
    return _root;
}

std::vector<std::string> ConfigData::getIndex()
{
    return _index;
}

bool ConfigData::getAutoindex()
{
    return _autoindex;
}

size_t ConfigData::getClientMaxBodySize()
{
    return _client_max_body_size;
}

std::map<int, std::string> ConfigData::getErrorPages()
{
    return _error_pages;
}

std::vector<std::string> ConfigData::getMethods()
{
    return _methods;
}

std::map<std::string, std::string> ConfigData::getCgi()
{
    return _cgi;
}

void ConfigData::setContent(std::string content)
{
    _content.push_back(content);
}

void ConfigData::setHost(std::string host)
{
    _host = host;
}

void ConfigData::setPort(int port)
{
    _port = port;
}

void ConfigData::setServerName(std::string server_name)
{
    _server_name = server_name;
}

void ConfigData::setRoot(std::string root)
{
    _root = root;
}

void ConfigData::setIndex(std::vector<std::string> index)
{
    _index = index;
}

void ConfigData::addIndex(std::string index)
{
    _index.push_back(index);
}

void ConfigData::setAutoindex(bool autoindex)
{
    _autoindex = autoindex;
}

void ConfigData::setClientMaxBodySize(size_t client_max_body_size)
{
    _client_max_body_size = client_max_body_size;
}

void ConfigData::setErrorPages(std::map<int, std::string> error_pages)
{
    _error_pages = error_pages;
}

void ConfigData::addErrorPage(int code, std::string path)
{
    _error_pages[code] = path;
}

void ConfigData::setMethods(std::vector<std::string> methods)
{
    _methods = methods;
}

void ConfigData::addMethod(std::string method)
{
    _methods.push_back(method);
}

void ConfigData::setCgi(std::map<std::string, std::string> cgi)
{
    _cgi = cgi;
}

void ConfigData::addCgi(std::string extension, std::string interpreter)
{
    _cgi[extension] = interpreter;
}

void ConfigData::parseConfigData()
{
    bool inLocations = false;
    bool baseIndent = 0;
    Location* currentLocation = NULL;
    std::vector<std::string>::iterator it;

    _port = -1;
    for (it = _content.begin(); it != _content.end(); ++it)
    {
        std::string line = rtrim(*it);    
        if (line.find("server:") == 0 || line.empty())
            continue;
        size_t colonPos = line.find(':');
        std::string key;

        if(!baseIndent)
            baseIndent = countIndent(line);
        if (baseIndent != countIndent(line))
            throw WebservException("Configuration file : wrong indentation");
        if (colonPos == std::string::npos)
            throw WebservException("Configuration file : Invalid format");

        key = line.substr(0, colonPos);
        std::string value = trim(line.substr(colonPos + 1));
        if (key == "host")
        {
            if (!_host.empty())
                throw WebservException("Configuration file : duplicated");
            _host = value;
        }
        else if (key == "port")
        {
            // check if post is duplicated and check for the port if is in the range
            _port = atoi(value.c_str());
        }
        else if (key == "server_name")
        {
            if (!_server_name.empty())
                throw WebservException("Configuration file : duplicated");
            _server_name = value;
        }
        else if (key == "client_max_body_size")
        {
            parseBodySize(value);
        }
        else if (key == "locations")
        {
            createNewLocation(value, currentLocation);
            while (1)
            {

            }
            parseLocation(value);
        }
        else if (key == "error_page" || inErrorPage == true)
        {
            parseErrorPage(value);
        }
    }
}

void ConfigData::createNewLocation(std::string value, Location*& currentLocation)
{
    std::string path = trim(value, " \"'");
    _locations[path] = Location();
    _locations[path].path = path;
    currentLocation = &_locations[path];
}

void ConfigData::printData()
{
    std::cout << "Host: " << _host << std::endl;
    std::cout << "Port: " << _port << std::endl;
    std::cout << "Server Name: " << _server_name << std::endl;
    std::cout << "Client Max Body Size: " << _client_max_body_size << std::endl;

    std::cout << "Error Pages: ";
    std::map<int, std::string>::const_iterator it;
    for (it = _error_pages.begin(); it != _error_pages.end(); ++it)
        std::cout << it->first << ":" << it->second << (it != --_error_pages.end() ? ", " : "");
    std::cout << std::endl;

    std::cout << "Locations:" << std::endl;
    std::map<std::string, Location>::const_iterator loc_it;
    for (loc_it = _locations.begin(); loc_it != _locations.end(); ++loc_it)
    {
        std::cout << "-------------------------------------------------\n";
        std::cout << "  " << loc_it->first << ":" << std::endl;
        std::cout << "    Root: " << loc_it->second.root << std::endl;
        
        std::cout << "    Index: ";
        for (size_t i = 0; i < loc_it->second.index.size(); ++i)
            std::cout << loc_it->second.index[i] << (i != loc_it->second.index.size() - 1 ? ", " : "");
        std::cout << std::endl;

        std::cout << "    Methods: ";
        for (size_t i = 0; i < loc_it->second.methods.size(); ++i)
            std::cout << loc_it->second.methods[i] << (i != loc_it->second.methods.size() - 1 ? ", " : "");
        std::cout << std::endl;

        std::cout << "    Autoindex: " << (loc_it->second.autoindex ? "on" : "off") << std::endl;

        std::cout << "    CGI: ";
        std::map<std::string, std::string>::const_iterator cgi_it;
        for (cgi_it = loc_it->second.cgi.begin(); cgi_it != loc_it->second.cgi.end(); ++cgi_it)
            std::cout << cgi_it->first << ":" << cgi_it->second << (cgi_it != --loc_it->second.cgi.end() ? ", " : "");
        std::cout << std::endl;
        std::cout << "-------------------------------------------------\n";
    }
}
