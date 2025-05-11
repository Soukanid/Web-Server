#include "ConfigParsing.hpp"
#include <string>
#include <cctype>
#include <sstream>

int	ft_atoi(const char *nptr)
{
	int	i;
	int	res;
	int	sign;

	i = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	res = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + nptr[i] - 48;
		i++;
	}
	return (res * sign);
}

std::string& rtrim(std::string& str)
{
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    
    if (end == std::string::npos)
        str.clear();
    else
        str.erase(end + 1);
    return str;
}

std::string rtrim_copy(std::string str)
{
    rtrim(str);
    return str;
}


int countIndent(const std::string& line)
{
    int spaces = 0;

    for(int i = 0; i < (int)line.length(); i++)
    {
        if (line[i] == ' ')
            spaces++;
        else if (line[i] == '\t')
            throw std::runtime_error("Tabs are not allowed for indentation");
        else
            break;
    }
    return spaces;
}

std::string trim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
        return ("");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (str.substr(start, end - start + 1));
}

std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter))
    {
        token = trim(token);
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }
    return tokens;
}

std::string trim(const std::string& str, const std::string& chars_to_trim)
{
    size_t start = str.find_first_not_of(chars_to_trim);
    if (start == std::string::npos)
        return "";
    
    size_t end = str.find_last_not_of(chars_to_trim);
    return str.substr(start, end - start + 1);
}
