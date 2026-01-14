#include <algorithm>
#include <array>
#include <cstdint>
#include <expected>
#include <format>
#include <iterator>
#include <ranges>
#include <vector>

#include "FileUtilities.h"
#include "Log.h"
#include "TestUtilities.h"

Logger LOGGER{};

template <size_t JoltageSize>
uint64_t FindBiggestJoltageInBank(std::vector<uint8_t> batteries)
{
  // The batteries cannot be re-ordered, and we need to make the biggest 2 digit number possible
  // First, find the biggest element that is not in the last position as that will be our tenfold
  // Next, find the biggest element after the found tenfold element to be our onefold

  std::array<uint8_t, JoltageSize> biggestJoltages{};

  auto currIt = batteries.begin();
  for (size_t i{}; i < JoltageSize; ++i)
  {
    auto end = batteries.end();
    auto foundElement = batteries.end();
    while (true)
    {
      foundElement = std::ranges::max_element(currIt, end);
      LOGGER.LogTrace(std::format("Found element: {} at distance {} from end", *foundElement, std::distance(foundElement, batteries.end() - 1)));

      // Ensure that we still have enough elements to search over for the rest of the 'JoltageSize'
      if (std::distance(foundElement, batteries.end() - 1) >= static_cast<int32_t>(JoltageSize - i - 1))
      {
        break;
      }
      
      LOGGER.LogTrace(std::format("Element is too close to end, rejecting..."));
      --end;
    }
        
    // auto it = std::ranges::max_element(currIt, batteries.end());
    biggestJoltages[i] = *foundElement;
    currIt = foundElement + 1;
  }

  uint64_t totalJoltage{};
  for (size_t i{ 0 }; i < JoltageSize; ++i)
  {
    totalJoltage *= 10;
    totalJoltage += biggestJoltages[i];
  }

  LOGGER.LogDebug(std::format("Found biggest Joltage with Size {}: {}", JoltageSize, totalJoltage));

  LOGGER.LogDebug("\n==================\n");

  return totalJoltage;
}

std::expected<void, std::monostate> Tests()
{
  TEST(FindBiggestJoltageInBank<2>({9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1}) == 98, "Expected biggest joltage to be 98");
  TEST(FindBiggestJoltageInBank<2>({8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9}) == 89, "Expected biggest joltage to be 89");
  TEST(FindBiggestJoltageInBank<2>({2, 3, 4, 2, 3, 4, 2, 3, 4, 2, 3, 4, 2, 3, 4, 2, 7, 8}) == 78, "Expected biggest joltage to be 78");
  TEST(FindBiggestJoltageInBank<2>({8, 1, 8, 1, 8, 1, 8, 1, 9, 1, 1, 1, 1, 2, 1, 1, 1}) == 92, "Expected biggest joltage to be 92");

  TEST(FindBiggestJoltageInBank<12>({9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1}) == 987654321111, "Expected biggest joltage to be 987654321111");
  TEST(FindBiggestJoltageInBank<12>({8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9}) == 811111111119, "Expected biggest joltage to be 811111111119");
  TEST(FindBiggestJoltageInBank<12>({2, 3, 4, 2, 3, 4, 2, 3, 4, 2, 3, 4, 2, 7, 8}) == 434234234278, "Expected biggest joltage to be 434234234278");
  TEST(FindBiggestJoltageInBank<12>({8, 1, 8, 1, 8, 1, 8, 1, 9, 1, 1, 1, 1, 2, 1, 1, 1}) == 888911112111, "Expected biggest joltage to be 888911112111");

  return {};
}

template <size_t JoltageSize>
uint64_t SumUpBiggestJoltagesInBanks(std::vector<std::vector<uint8_t>> banks)
{
  uint64_t total{};
  for (auto const& bank : banks)
  {
    total += FindBiggestJoltageInBank<JoltageSize>(bank);
  }

  return total;
}

int main()
{
  LOGGER.logLevel = LogLevel::Info;
  if (!Tests().has_value())
  {
    LOGGER.LogError("Tests failed, aborting");
    return 1;
  }

  LOGGER.LogSuccess("All tests succeeded");

  std::vector<std::string> const lines = ReadLinesFromFile("input.txt");
  LOGGER.LogInfo(std::format("Read {} lines from input.txt", lines.size()));

  std::vector<std::vector<uint8_t>> banks;
  for (std::string const& line : lines)
  {
    std::vector<uint8_t> batteries;
    for (char const c : line)
    {
      batteries.push_back(static_cast<uint8_t>(c - '0'));
    }

    banks.push_back(batteries);
  }

  uint64_t const totalJoltage = SumUpBiggestJoltagesInBanks<2>(banks);
  LOGGER.LogSuccess(std::format("Total Joltage found across {} banks counting {} batteries: {}", banks.size(), 2, totalJoltage));
  uint64_t const totalJoltagePartTwo = SumUpBiggestJoltagesInBanks<12>(banks);
  LOGGER.LogSuccess(std::format("Total Joltage found across {} banks counting {} batteries: {}", banks.size(), 12, totalJoltagePartTwo));

  // Answer to part one is: 17281
  // Answer to part two is: 171388730430281

  return 0;
}