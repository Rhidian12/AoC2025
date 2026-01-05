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