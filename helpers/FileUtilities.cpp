#include "FileUtilities.h"

#include <fstream>

std::vector<std::string> ReadLinesFromFile(std::string_view const filepath)
{
    std::vector<std::string> lines;
    std::ifstream file{std::string{filepath}};

    if (!file.is_open())
    {
        return lines;
    }

    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    return lines;
}

std::vector<std::string> ReadDataFromFileWithDelimiter(std::string_view filepath, char delimiter)
{
    std::vector<std::string> data;
    std::ifstream file{std::string{filepath}};

    if (!file.is_open())
    {
        return data;
    }

    std::string part;
    while (std::getline(file, part, delimiter))
    {
        data.push_back(part);
    }

    return data;
}