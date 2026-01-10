#pragma once

#define TEST(condition, message)                               \
  do                                                           \
  {                                                            \
    if (!(condition))                                          \
    {                                                          \
      Logger logger;                                           \
      logger.LogError("Test failed: " + std::string(message)); \
      return std::unexpected(std::monostate{});                \
    }                                                          \
  } while (0)