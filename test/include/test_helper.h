#pragma once

class TestHelper
{
public:
    enum class Environment
    {
        MacOS,
        Linux,
        Windows,
        Unknown
    };

    static constexpr Environment kTestEnvironment =
#if defined(__APPLE__)
    Environment::MacOS;
#elif defined(__linux__)
    Environment::Linux;
#elif defined(_WIN32) || defined(_WIN64)
    Environment::Windows;
#else
    Environment::Unknown;
#endif
};
