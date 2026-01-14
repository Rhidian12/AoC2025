#include <algorithm>
#include <array>
#include <cstdint>
#include <expected>
#include <format>
#include <ranges>
#include <vector>

#include "FileUtilities.h"
#include "Log.h"
#include "TestUtilities.h"

Logger LOGGER{};

uint8_t FindBiggestJoltageInBank(std::vector<uint8_t> batteries)
{
  // The batteries cannot be re-ordered, and we need to make the biggest 2 digit number possible
  // First, find the biggest element that is not in the last position as that will be our tenfold
  // Next, find the biggest element after the found tenfold element to be our onefold

  std::array<uint8_t, 2> biggestJoltages{0, 0};

  auto it = std::ranges::max_element(batteries | std::views::take(batteries.size() - 1));
  biggestJoltages[0] = *it;
  it = std::ranges::max_element(it + 1, batteries.end());
  biggestJoltages[1] = *it;

  LOGGER.LogDebug(std::format("Found biggest joltages: {}, {}", biggestJoltages[0], biggestJoltages[1]));

  return biggestJoltages[0] * 10 + biggestJoltages[1];
}

std::expected<void, std::monostate> Tests()
{
  TEST(FindBiggestJoltageInBank({9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1}) == 98, "Expected biggest joltage to be 98");
  TEST(FindBiggestJoltageInBank({8,1,1,1,1,1,1,1,1,1,1,1,1,9}) == 89, "Expected biggest joltage to be 89");
  TEST(FindBiggestJoltageInBank({2,3,4,2,3,4,2,3,4,2,3,4,2,3,4,2,7,8}) == 78, "Expected biggest joltage to be 78");
  TEST(FindBiggestJoltageInBank({8,1,8,1,8,1,8,1,9,1,1,1,1,2,1,1,1}) == 92, "Expected biggest joltage to be 92");

  return {};
}

uint32_t SumUpBiggestJoltagesInBanks(std::vector<std::vector<uint8_t>> banks)
{
  uint32_t total{};
  for (auto const& bank : banks)
  {
    total += FindBiggestJoltageInBank(bank);
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

  uint32_t const totalJoltage = SumUpBiggestJoltagesInBanks(banks);
  LOGGER.LogSuccess(std::format("Total Joltage found accross {} banks: {}", banks.size(), totalJoltage));

  // Answer to part one is: 17281
  

  return 0;
}