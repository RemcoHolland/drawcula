#pragma once

enum class Environment { MacOS, Linux, Windows, Unknown };

#if defined(__APPLE__)
constexpr Environment kTestEnvironment = Environment::MacOS;
#elif defined(__linux__)
constexpr Environment kTestEnvironment = Environment::Linux;
#elif defined(_WIN32) || defined(_WIN64)
constexpr Environment kTestEnvironment = Environment::Windows;
#else
constexpr Environment kTestEnvironment = Environment::Unknown;
#endif
