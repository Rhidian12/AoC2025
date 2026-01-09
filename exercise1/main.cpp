#include <__expected/expected.h>

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
  bool countZeroDuringWraps = true;
  bool bruteForce = false;  // Sanity Checks

  void Reset()
  {
    position = 50;
    zeroCounter = 0;
  }
};

DialPointer& operator+=(DialPointer& dial, int offset)
{
  if (dial.bruteForce)
  {
    for (int i{}; i < std::abs(offset); ++i)
    {
      if (offset > 0)
      {
        dial.position = (dial.position + 1) % 100;
        if (dial.position == 0)
        {
          dial.zeroCounter++;
        }
      }
      else
      {
        dial.position = (dial.position - 1) % 100;
        if (dial.position == 0)
        {
          dial.zeroCounter++;
        }
      }
    }

    return dial;
  }

  int const originalDialPosition = dial.position;
  dial.position = (dial.position + offset) % 100;

  if (dial.position < 0)
  {
    dial.position += 100;
  }

  if (originalDialPosition == 0 && std::abs(offset) < 100)
  {
    return dial;
  }

  if (dial.countZeroDuringWraps && offset + originalDialPosition != 100)
  {
    dial.zeroCounter += std::abs(offset) / 100;
    if (offset < 0 && dial.position > originalDialPosition)
    {
      ++dial.zeroCounter;
    }
    else if (offset > 0 && dial.position < originalDialPosition)
    {
      ++dial.zeroCounter;
    }
    else if (dial.position == 0)
    {
      ++dial.zeroCounter;
    }
  }
  else if (dial.position == 0)
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
    LOGGER.LogError("Test failed: dial zero counter should be " + std::to_string(expectedZeroCount) + ". Dial zero counter is " +
                    std::to_string(dial.zeroCounter));
    return std::unexpected(std::monostate{});
  }

  return {};
}

std::expected<void, std::monostate> Tests()
{
  DialPointer dial;
  dial.countZeroDuringWraps = false;

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

std::expected<void, std::monostate> TestsDialCountZeroWraps()
{
  DialPointer dial;

  if (!RunDialTest(dial, 68, 82, 1, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  if (!RunDialTest(dial, 30, 52, 1, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  if (!RunDialTest(dial, 48, 0, 2).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  if (!RunDialTest(dial, 5, 95, 2, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  if (!RunDialTest(dial, 60, 55, 3).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  if (!RunDialTest(dial, 55, 0, 4, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  if (!RunDialTest(dial, 1, 99, 4, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  if (!RunDialTest(dial, 99, 0, 5, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  if (!RunDialTest(dial, 14, 14, 5).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  if (!RunDialTest(dial, 82, 32, 6, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  dial.Reset();
  if (!RunDialTest(dial, 258, 8, 3).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  if (!RunDialTest(dial, 258, 50, 6, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  dial.Reset();
  dial.zeroCounter = 0;

  if (!RunDialTest(dial, 1000, 50, 10).has_value())
  {
    return std::unexpected(std::monostate{});
  }
  if (!RunDialTest(dial, 1000, 50, 20, false).has_value())
  {
    return std::unexpected(std::monostate{});
  }
  if (!RunDialTest(dial, 50, 0, 21).has_value())
  {
    return std::unexpected(std::monostate{});
  }

  return {};
}

int main()
{
  if (!Tests().has_value())
  {
    LOGGER.LogError("Tests failed, aborting");
    return 1;
  }
  LOGGER.LogSuccess("First tests passed");

  if (!TestsDialCountZeroWraps().has_value())
  {
    LOGGER.LogError("Dial wrap counting tests failed, aborting");
    return 1;
  }
  LOGGER.LogSuccess("Second tests passed");
  LOGGER.LogSuccess("All tests passed");

  LOGGER.logLevel = LogLevel::Info;

  std::vector<std::string> lines = ReadLinesFromFile("input.txt");

  LOGGER.LogInfo("Read " + std::to_string(lines.size()) + " lines from input.txt");

  DialPointer dial;
  dial.bruteForce = true;

  // int dial = 50;
  // int zeroCounter = 0;

  for (std::string const& line : lines)
  {
    if (line.empty())
    {
      LOGGER.LogError("THIS SHOULDN'T HAPPEN");
      continue;
    }

    // int distance = std::stoi(line.substr(1));
    switch (line[0])
    {
      case 'L':
        // for (int i{}; i < distance; ++i)
        // {
        //   dial = (dial - 1 + 100) % 100;

        //   if (dial == 0)
        //   {
        //     zeroCounter++;
        //   }
        // }
        LOGGER.LogDebug("Moving dial left by " + line.substr(1));
        dial -= std::stoi(line.substr(1));
        LOGGER.LogDebug("Dial position is now " + std::to_string(dial.position));
        break;
      case 'R':
        // for (int i{}; i < distance; ++i)
        // {
        //   dial = (dial + 1) % 100;

        //   if (dial == 0)
        //   {
        //     zeroCounter++;
        //   }
        // }
        LOGGER.LogDebug("Moving dial right by " + line.substr(1));
        dial += std::stoi(line.substr(1));
        LOGGER.LogDebug("Dial position is now " + std::to_string(dial.position));
        break;
    }
  }

  LOGGER.LogSuccess("Dial's zero counter is: " + std::to_string(dial.zeroCounter));
  // LOGGER.LogSuccess("Dial's zero counter is: " + std::to_string(zeroCounter));
  // Answer to part 1 is 1147
  // Answer to part 2 is 6789

  return 0;
}