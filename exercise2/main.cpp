#include <__algorithm/ranges_equal_range.h>

#include <algorithm>
#include <cstdint>
#include <expected>
#include <format>
#include <ranges>
#include <vector>

#include "FileUtilities.h"
#include "Log.h"
#include "TestUtilities.h"

Logger LOGGER{};

std::string RangeToString(auto range)
{
  std::string str;
  for (auto i : range)
  {
    str += std::to_string(i) + ", ";
  }

  str.pop_back();
  str.pop_back();

  return str;
}

bool FindAdjacentPatterns(auto range, auto pattern)
{
  for (size_t i{}; i < range.size() / pattern.size(); ++i)
  {
    auto subRangeOne = range | std::views::drop(i * pattern.size()) | std::views::take(pattern.size());
    auto subRangeTwo = range | std::views::drop((i + 1) * pattern.size()) | std::views::take(pattern.size());

    LOGGER.LogDebug(std::format("SubRangeOne: {}. SubRangeTwo: {}", subRangeOne, subRangeTwo));

    if (std::ranges::equal(subRangeOne, subRangeTwo))
    {
      return true;
    }
  }

  return false;
}

// returns uint64_t because im lazy
std::vector<uint64_t> GetDigitsFromNumber(uint64_t number)
{
  std::vector<uint64_t> digits;
  while (number / 10 > 0)
  {
    digits.push_back(number % 10);
    number /= 10;
  }

  digits.push_back(number % 10);

  return digits;
}

// An Invalid ID is:
// - A number with an uneven amount of digits
// - A number consisting of 2 sequences of equal length that are equal
bool IsInvalidID(uint64_t number)
{
  std::vector<uint64_t> digits{GetDigitsFromNumber(number)};

  // A Sequence must be able to be repeated twice. So if there's isn't a even number of digits, it's guaranteed to be a valid number
  if (digits.size() % 2 != 0)
  {
    return false;
  }

  std::ranges::reverse(digits);

  auto subRangeOne = digits | std::views::take(digits.size() / 2);
  auto subRangeTwo = digits | std::views::drop(digits.size() / 2) | std::views::all;

  LOGGER.LogTrace(std::format("SubRangeOne: {}, SubRangeTwo: {}", subRangeOne, subRangeTwo));

  return std::ranges::equal(subRangeOne, subRangeTwo);

  return false;
}

// An Invalid ID is:
//  - A number with X number of sequences of equal length repeated
//    - A sequence can be 1 number long
bool IsInvalidIDPart2(uint64_t number)
{
  LOGGER.LogDebug(std::format("Checking number: {}", number));
  std::vector<uint64_t> const digits{GetDigitsFromNumber(number)};

  // We can only find a pattern with the length of the remaining digits / 2 at most
  // Pattern must be at least 1 digit
  for (size_t patternLength{1}; patternLength <= digits.size() / 2; ++patternLength)
  {
    LOGGER.LogDebug(std::format("Checking Patterns with Length {}", patternLength));
    if (digits.size() % patternLength != 0)
    {
      LOGGER.LogDebug(std::format("Skipping Pattern with Length {} as it cannot evenly fit in the range", patternLength));
      continue;
    }

    size_t const nrOfPatternFits = digits.size() / patternLength;

    // We always want to search the entire range
    // And our pattern that we're searching for has to:
    // 1. Fit in the range an even number of times
    // 2. Be actually repeated the entire time
    auto patternToSearch = digits | std::views::take(patternLength);
    LOGGER.LogDebug(std::format("We're searching for the pattern: {}. Our Pattern can fit {} times", patternToSearch, nrOfPatternFits));

    bool foundEqualPattern = true;
    for (size_t i{}; i < nrOfPatternFits; ++i)
    {
      auto rangeToSearch = digits | std::views::drop(patternLength * i) | std::views::take(patternLength);
      if (!std::ranges::equal(rangeToSearch, patternToSearch))
      {
        foundEqualPattern = false;
        break;
      }
    }

    if (foundEqualPattern)
    {
      LOGGER.LogDebug("Duplicate found!");
      LOGGER.LogDebug("\n==========\n");
      return true;
    }
  }

  LOGGER.LogDebug("\n==========\n");

  return false;
}

uint64_t AddValueOfInvalidIDsInRange(uint64_t const begin, uint64_t const end, bool partOne = true)
{
  uint64_t total{};
  for (uint64_t i{begin}; i <= end; ++i)
  {
    bool const isInvalidID = partOne ? IsInvalidID(i) : IsInvalidIDPart2(i);
    if (isInvalidID)
    {
      total += i;
    }
  }

  return total;
}

std::expected<void, std::monostate> Tests()
{
  LOGGER.LogInfo("Testing IsInvalidID function");

  TEST(IsInvalidID(11), "11 is an invalid ID");
  TEST(IsInvalidID(22), "22 is an invalid ID");
  TEST(!IsInvalidID(23), "23 is a valid ID");
  TEST(!IsInvalidID(95), "95 is a valid ID");
  TEST(!IsInvalidID(115), "115 is a valid ID");
  TEST(!IsInvalidID(333), "333 is a valid ID");
  TEST(!IsInvalidID(345), "345 is a valid ID");
  TEST(IsInvalidID(6464), "6464 is an invalid ID");
  TEST(!IsInvalidID(5359), "5359 is a valid ID");
  TEST(!IsInvalidID(123488), "123488 is a valid ID");
  TEST(IsInvalidID(1188511885), "1188511885 is an invalid ID");
  TEST(!IsInvalidID(222224), "222224 is a valid ID");
  TEST(IsInvalidID(446446), "446446 is an invalid ID");
  TEST(IsInvalidID(38593859), "38593859 is an invalid ID");

  uint64_t total{};
  total += AddValueOfInvalidIDsInRange(11, 22);
  total += AddValueOfInvalidIDsInRange(95, 115);
  total += AddValueOfInvalidIDsInRange(998, 1012);
  total += AddValueOfInvalidIDsInRange(1188511880, 1188511890);
  total += AddValueOfInvalidIDsInRange(222220, 222224);
  total += AddValueOfInvalidIDsInRange(1698522, 1698528);
  total += AddValueOfInvalidIDsInRange(446443, 446449);
  total += AddValueOfInvalidIDsInRange(38593856, 38593862);
  TEST(total == 1227775554, "Expected total of Invalid IDs to be 1227775554");

  return {};
}

std::expected<void, std::monostate> TestsPartTwo()
{
  LOGGER.LogInfo("Testing IsInvalidIDPart2 function");

  TEST(IsInvalidIDPart2(11), "11 is an invalid ID");
  TEST(IsInvalidIDPart2(22), "22 is an invalid ID");
  TEST(!IsInvalidIDPart2(23), "23 is a valid ID");
  TEST(!IsInvalidIDPart2(95), "95 is a valid ID");
  TEST(IsInvalidIDPart2(111), "111 is an invalid ID");
  TEST(!IsInvalidIDPart2(115), "115 is a valid ID");
  TEST(IsInvalidIDPart2(333), "333 is an invalid ID");
  TEST(!IsInvalidIDPart2(345), "345 is a valid ID");
  TEST(IsInvalidIDPart2(6464), "6464 is an invalid ID");
  TEST(!IsInvalidIDPart2(5359), "5359 is a valid ID");
  TEST(!IsInvalidIDPart2(123488), "123488 is a valid ID");
  TEST(IsInvalidIDPart2(1188511885), "1188511885 is an invalid ID");
  TEST(!IsInvalidIDPart2(222224), "222224 is a valid ID");
  TEST(IsInvalidIDPart2(446446), "446446 is an invalid ID");
  TEST(IsInvalidIDPart2(38593859), "38593859 is an invalid ID");
  TEST(IsInvalidIDPart2(565656), "565656 is an invalid ID");
  TEST(IsInvalidIDPart2(824824824), "824824824 is an invalid ID");
  TEST(IsInvalidIDPart2(2121212121), "2121212121 is an invalid ID");

  uint64_t total{};
  total += AddValueOfInvalidIDsInRange(11, 22, /* partOne = */ false);
  total += AddValueOfInvalidIDsInRange(95, 115, /* partOne = */ false);
  total += AddValueOfInvalidIDsInRange(998, 1012, /* partOne = */ false);
  total += AddValueOfInvalidIDsInRange(1188511880, 1188511890, /* partOne = */ false);
  total += AddValueOfInvalidIDsInRange(222220, 222224, /* partOne = */ false);
  total += AddValueOfInvalidIDsInRange(1698522, 1698528, /* partOne = */ false);
  total += AddValueOfInvalidIDsInRange(446443, 446449, /* partOne = */ false);
  total += AddValueOfInvalidIDsInRange(38593856, 38593862, /* partOne = */ false);
  total += AddValueOfInvalidIDsInRange(565653, 565659, /* partOne = */ false);
  total += AddValueOfInvalidIDsInRange(824824821, 824824827, /* partOne = */ false);
  total += AddValueOfInvalidIDsInRange(2121212118, 2121212124, /* partOne = */ false);
  TEST(total == 4174379265, std::format("Expected total of Invalid IDs to be 4174379265. It is: {}", total));

  return {};
}

int main()
{
  LOGGER.logLevel = LogLevel::Info;

  if (!Tests().has_value())
  {
    LOGGER.LogError("Tests failed, aborting");
    return 1;
  }

  if (!TestsPartTwo().has_value())
  {
    LOGGER.LogError("TestsPartTwo failed, aborting");
    return 1;
  }

  LOGGER.LogSuccess("All tests passed");

  uint64_t total{};
  std::vector<std::string> const data = ReadDataFromFileWithDelimiter("input.txt", ',');
  LOGGER.LogInfo(std::format("Read {} parts of data from the input", data.size()));
  for (std::string const& dataPart : data)
  {
    size_t const dashPos = dataPart.find('-');
    if (dashPos == std::string::npos)
    {
      LOGGER.LogError(std::format("- could not be found in {}", dataPart));
      return 1;
    }

    LOGGER.LogDebug(std::format("Begin of Range: {}, End of Range: {}", dataPart.substr(0, dashPos), dataPart.substr(dashPos + 1)));

    uint64_t const beginOfRange = std::stoull(dataPart.substr(0, dashPos));
    uint64_t const endOfRange = std::stoull(dataPart.substr(dashPos + 1));
    total += AddValueOfInvalidIDsInRange(beginOfRange, endOfRange, false);
  }

  LOGGER.LogSuccess(std::format("All Invalid IDs summed together are: {}", total));

  // Answer to part one is: 31000881061
  // Answer to part two is: 46769308485

  return 0;
}