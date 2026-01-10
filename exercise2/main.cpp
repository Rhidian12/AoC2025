#include <__algorithm/ranges_equal_range.h>
#include <algorithm>
#include <expected>
#include <ranges>
#include <vector>
#include <format>

#include "Log.h"
#include "TestUtilities.h"
#include "FileUtilities.h"

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

// bool FindAdjacentPatterns(auto range, auto pattern)
// {
//   for (size_t i{}; i < range.size() / pattern.size(); ++i)
//   {
//     auto subRangeOne = range | std::views::drop(i * pattern.size()) | std::views::take(pattern.size());
//     auto subRangeTwo = range | std::views::drop((i + 1) * pattern.size()) | std::views::take(pattern.size());

//     LOGGER.LogDebug(std::format("SubRangeOne: {}. SubRangeTwo: {}", subRangeOne, subRangeTwo));

//     if (std::ranges::equal(subRangeOne, subRangeTwo))
//     {
//       return true;
//     }
//   }

//   return false;
// }

// An Invalid ID is:
// - A number with an uneven amount of digits
// - A number consisting of 2 sequences of equal length that are equal
bool IsInvalidID(uint64_t number)
{
  std::vector<uint64_t> digits;
  while (number / 10 > 0)
  {
    digits.push_back(number % 10);
    number /= 10;
  }

  digits.push_back(number % 10);

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

  // for (size_t currentDigitIndex{}; currentDigitIndex < digits.size(); ++currentDigitIndex)
  // {
  //   LOGGER.LogDebug(std::format("Current Digit: {}. Index: {}/{}", digits[currentDigitIndex], currentDigitIndex, digits.size()));
  //   // If the digit only appears once it cannot be part of a duplicate
  //   if (std::ranges::count(digits, digits[currentDigitIndex]) == 1)
  //   {
  //     LOGGER.LogDebug(std::format("Current Digit {}. (Index {}) appears only once. Skipping...", digits[currentDigitIndex], currentDigitIndex));
  //     LOGGER.LogDebug("\n==========\n");
  //     continue;
  //   }

  //   // We can only find a pattern with the length of the remaining digits / 2 at most
  //   // Pattern must be at least 1 digit
  //   for (size_t patternLength{1}; patternLength <= (digits.size() - currentDigitIndex); ++patternLength)
  //   {
  //     LOGGER.LogDebug(std::format("Checking Patterns with Length {}", patternLength));
  //     // We can only find a pattern in the digits coming after 'currentDigitIndex' as everything before
  //     // 'currentDigitIndex' should have been checked already
  //     for (size_t otherDigitIndex{currentDigitIndex + 1}; otherDigitIndex < digits.size(); ++otherDigitIndex)
  //     {
  //       // Now check if we can find the pattern
  //       auto rangeToSearch = digits | std::views::drop(std::max(static_cast<int>(currentDigitIndex) - 1, 0));
  //       auto patternToSearch = digits | std::views::drop(std::max(static_cast<int>(currentDigitIndex) - 1, 0)) | std::views::take(patternLength);
  //       LOGGER.LogDebug(std::format("The range we're searching: {}. We're searching for the pattern: {}", rangeToSearch, patternToSearch));
  //       bool const adjacentPatterns = FindAdjacentPatterns(rangeToSearch, patternToSearch);
  //       // auto const foundRange = std::ranges::includes(rangeToSearch, patternToSearch);
  //       // LOGGER.LogDebug(std::format("Our found range: {}", foundRange));
  //       if (adjacentPatterns)
  //       {
  //         LOGGER.LogDebug("Duplicate found!");
  //         LOGGER.LogDebug("\n==========\n");
  //         return true;
  //       }
  //     }
  //   }

  //   LOGGER.LogDebug("\n==========\n");
  // }

  return false;
}

uint64_t AddValueOfInvalidIDsInRange(uint64_t const begin, uint64_t const end)
{
  uint64_t total{};
  for (uint64_t i{begin}; i <= end; ++i)
  {
    if (IsInvalidID(i))
    {
      total += i;
    }
  }

  return total;
}

std::expected<void, std::monostate> Tests()
{
  LOGGER.LogInfo("Testing IsDuplicateNumber function");

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

int main()
{
  LOGGER.logLevel = LogLevel::Debug;

  if (!Tests().has_value())
  {
    LOGGER.LogError("Tests failed, aborting");
    return 1;
  }

  LOGGER.LogSuccess("All tests passed");

  uint64_t total{};
  std::vector<std::string> const data = ReadDataFromFileWithDelimiter("input.txt", ',');
  LOGGER.LogInfo(std::format("Read {} parts of data from the input", data.size()));
  for (std::string const & dataPart : data)
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
    total += AddValueOfInvalidIDsInRange(beginOfRange, endOfRange);
  }

  LOGGER.LogSuccess(std::format("All Invalid IDs summed together are: {}", total));

  // Answer to part one is: 31000881061

  return 0;
}