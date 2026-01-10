#pragma once

#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> ReadLinesFromFile(std::string_view filepath);
std::vector<std::string> ReadDataFromFileWithDelimiter(std::string_view filepath, char delimiter);