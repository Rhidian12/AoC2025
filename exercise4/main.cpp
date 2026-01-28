#include <algorithm>
#include <expected>
#include <format>
#include <ranges>
#include <string>

#include "FileUtilities.h"
#include "Log.h"
#include "TestUtilities.h"

Logger LOGGER{};

// lines is a std::vector with max size 3
// Index 0 is the current line
// Index 1 - 2 are the adjacent lines
uint32_t CountAdjacentRolls(std::vector<std::string> const& lines, uint32_t lineIndex, uint32_t maxLineLength)
{
  if (lines.size() == 2)
  {
    LOGGER.LogTrace(std::format("Line To Check: {}", lines[0]));
    LOGGER.LogTrace(std::format("Adjacent line: {}", lines[1]));
  }
  else
  {
    LOGGER.LogTrace(std::format("Line To Check: {}", lines[1]));
    LOGGER.LogTrace(std::format("Adjacent line 1: {}", lines[0]));
    LOGGER.LogTrace(std::format("Adjacent line 2: {}", lines[2]));
  }

  LOGGER.LogTrace(std::format("LineIndex: {}, maxLineLength: {}", lineIndex, maxLineLength));

  uint32_t const minIndex = (lineIndex == 0) ? 0 : lineIndex - 1;
  uint32_t const indicesToTake = (lineIndex == maxLineLength || lineIndex == 0) ? 2 : 3;

  LOGGER.LogTrace(std::format("MinIndex: {}, IndicesToTake: {}", minIndex, indicesToTake));

  // Per line, get the chars from index `minIndex` to `maxIndex`
  // Do -1 to account for our own roll
  uint32_t adjacentRolls = 0;
  for (std::string const& line : lines)
  {
    adjacentRolls += std::ranges::count(line | std::views::drop(minIndex) | std::views::take(indicesToTake), '@');
  }
  adjacentRolls -= 1;                           // Remove our own roll
  adjacentRolls = std::max(0u, adjacentRolls);  // Ensure we don't go negative

  LOGGER.LogTrace(std::format("Found {} adjacent rolls for lineIndex: {}\n", adjacentRolls, lineIndex));

  return adjacentRolls;
}

uint32_t FindAccessibleRollsOfPaper(std::vector<std::string> const& lines)
{
  // A roll of paper is accessible if there are fewer than 4 rolls of paper around it
  // (horizontally, vertically and diagonally).
  uint32_t accessibleRolls = 0;

  for (size_t lineNr{}; lineNr < lines.size(); ++lineNr)
  {
    LOGGER.LogTrace(std::format("Handling line nr {}", lineNr));
    std::string const& currentLine{lines[lineNr]};
    for (size_t lineIndex{}; lineIndex < currentLine.size(); ++lineIndex)
    {
      if (currentLine[lineIndex] != '@')
      {
        continue;
      }

      uint32_t const linesToTake = (lineNr == 0 || lineNr == lines.size() - 1) ? 2 : 3;

      if (CountAdjacentRolls(
              std::ranges::to<std::vector<std::string>>(lines | std::views::drop((lineNr == 0) ? 0 : lineNr - 1) | std::views::take(linesToTake)), lineIndex,
              currentLine.size()) < 4)
      {
        ++accessibleRolls;
      }
    }

    LOGGER.LogDebug(std::format("Line nr {} has {} rolls of paper", lineNr, accessibleRolls));
  }

  LOGGER.LogDebug(std::format("Got {} rolls of paper", accessibleRolls));

  return accessibleRolls;
}

std::expected<void, std::monostate> Tests()
{
  // clang-format off
  std::vector<std::string> const testData = {
    "..@@.@@@@.", 
    "@@@.@.@.@@", 
    "@@@@@.@.@@", 
    "@.@@@@..@.", 
    "@@.@@@@.@@",
    ".@@@@@@@.@", 
    ".@.@.@.@@@", 
    "@.@@@.@@@@", 
    ".@@@@@@@@.", 
    "@.@.@@@.@."
  };
  // clang-format on

  TEST(FindAccessibleRollsOfPaper(testData) == 13, "Expected 13 accessible rolls of paper");

  return {};
}

int main()
{
  LOGGER.logLevel = LogLevel::Info;
  if (!Tests())
  {
    LOGGER.LogError("Tests failed!");
    return 1;
  }

  LOGGER.LogSuccess("All tests passed");

  std::vector<std::string> const lines = ReadLinesFromFile("input.txt");
  LOGGER.LogInfo(std::format("Read {} lines from input.txt", lines.size()));

  uint32_t const nrOfRolls = FindAccessibleRollsOfPaper(lines);

  LOGGER.LogSuccess(std::format("Found {} accessible rolls of paper", nrOfRolls));

  // Answer to first exercise is 1551

  return 0;
}