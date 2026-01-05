#include <expected>
#include <string>
#include <variant>

#include "FileUtilities.h"
#include "Log.h"

// https://adventofcode.com/2025/day/1

Logger LOGGER{};

struct DialPointer
{
  int position = 50;
  int zeroCounter = 0;

  void Reset()
  {
    position = 50;
    zeroCounter = 0;
  }
};

DialPointer& operator+=(DialPointer& dial, int offset)
{
  dial.position = (dial.position + offset) % 100;

  if (dial.position < 0)
  {
    dial.position += 100;
  }

  if (dial.position == 0)
  {
    ++dial.zeroCounter;
  }

  return dial;
}

DialPointer& operator-=(DialPointer& dial, int offset)
{
  return dial += -offset;
}

std::expected<void, std::monostate> RunDialTest(DialPointer& dial, int offset, int expectedPosition, int expectedZeroCount, bool isAddition = true)
{
  if (isAddition)
  {
    dial += offset;
  }
  else
  {
    dial -= offset;
  }

  if (dial.position != expectedPosition)
  {
    LOGGER.LogError("Test failed: dial position should be " + std::to_string(expectedPosition) + ". Dial position is " + std::to_string(dial.position));
    return std::unexpected(std::monostate{});
  }

  if (dial.zeroCounter != expectedZeroCount)
  {
    LOGGER.LogError("Test failed: dial zero counter should be " + std::to_string(expectedZeroCount) + ". Dial zero counter is " + std::to_string(dial.zeroCounter));
    return std::unexpected(std::monostate{});
  }

  return {};
}

std::expected<void, std::monostate> Tests()
{
  DialPointer dial;

  dial.position = 99;
  if (!RunDialTest(dial, 1, 0, 1).has_value())
  {
    return std::unexpected(std::monostate{});
  }
  LOGGER.LogInfo("First Test passed");

  dial.Reset();
  dial.position = 11;
  if (!RunDialTest(dial, 8, 19, 0).has_value())
  {
    return std::unexpected(std::monostate{});
  }
  LOGGER.LogInfo("Second Test passed");

  if (!RunDialTest(dial, 19, 0, 1, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }
  LOGGER.LogInfo("Third Test passed");

  dial.Reset();
  dial.position = 0;
  if (!RunDialTest(dial, 1, 99, 0, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }
  LOGGER.LogInfo("Fourth Test passed");

  // Reset dial
  dial.Reset();

  dial -= 68;
  dial -= 30;
  dial += 48;
  dial -= 5;
  dial += 60;
  dial -= 55;
  dial -= 1;
  dial -= 99;
  dial += 14;
  dial -= 82;
  if (!RunDialTest(dial, 0, 32, 3).has_value())
  {
    return std::unexpected(std::monostate{});
  }
  LOGGER.LogInfo("Sixth Test passed");

  dial.Reset();
  if (!RunDialTest(dial, 658, 8, 0).has_value())
  {
    return std::unexpected(std::monostate{});
  }
  LOGGER.LogInfo("Seventh Test passed");

  dial.Reset();
  if (!RunDialTest(dial, 658, 92, 0, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }
  LOGGER.LogInfo("Eighth Test passed");

  return {};
}

int main()
{
  if (!Tests().has_value())
  {
    LOGGER.LogError("Tests failed, aborting");
    return 1;
  }

  LOGGER.logLevel = LogLevel::Info;

  LOGGER.LogSuccess("All tests passed");
  std::vector<std::string> lines = ReadLinesFromFile("input.txt");

  LOGGER.LogInfo("Read " + std::to_string(lines.size()) + " lines from input.txt");

  DialPointer dial;

  for (std::string const& line : lines)
  {
    if (line.empty())
    {
      continue;
    }

    switch (line[0])
    {
      case 'L':
        LOGGER.LogDebug("Moving dial left by " + line.substr(1));
        dial -= std::stoi(line.substr(1));
        LOGGER.LogDebug("Dial position is now " + std::to_string(dial.position));
        break;
      case 'R':
        LOGGER.LogDebug("Moving dial right by " + line.substr(1));
        dial += std::stoi(line.substr(1));
        LOGGER.LogDebug("Dial position is now " + std::to_string(dial.position));
        break;
    }
  }

  LOGGER.LogSuccess("Dial's zero counter is: " + std::to_string(dial.zeroCounter));

  return 0;
}