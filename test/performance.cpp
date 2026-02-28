#include <gtest/gtest.h>
#include <iostream>
#include <gmock/gmock-matchers.h>

#include "search.h"
#include "stringutils.h"
#include "util/test_helper.h"
#include "reader/fenreader.h"

inline void startSearch(const Fen& fen, const int depth) {
    Board board = Board(fen);
    Params params;
    params.max_depth = depth;
    params.movetime = std::chrono::milliseconds(LLONG_MAX);
    Search search = Search();
    testing::internal::CaptureStdout();
    search.start(fen.color, board, params);
}

TEST(performance, opening) {
    constexpr int depth = 7;
    const string depth_str = std::to_string(depth);

    // Sicilian opening
    startSearch(FenReader::read("rnbqkb1r/1p2pppp/p2p1n2/8/3NP3/2N5/PPP2PPP/R1BQKB1R w KQkq - 0 1"), depth);
    std::string output = testing::internal::GetCapturedStdout();
    std::vector<std::string> commands = StringUtils::split(output.substr(output.find("info depth " + depth_str)), ' ');

    const auto pos = std::ranges::find(commands, "nps");
    const auto index = std::distance(commands.begin(), pos );
    const int nps = std::stoi(commands[index + 1]);
    std::cout << "Nodes per second depth " + depth_str + ": " << nps << std::endl;

    if constexpr (kTestEnvironment == Environment::MacOS) {
        std::cout << "On Mac" << std::endl;
        EXPECT_GE(nps, 4220000);
    } else {
        std::cout << "On Linux" << std::endl;
        EXPECT_GE(nps, 500000);
    }
}

TEST(performance, middlegame) {
    constexpr int depth = 8;
    const string depth_str = std::to_string(depth);

    // Middlegame position
    startSearch(FenReader::read("2r2rk1/1bq1bppp/p2ppn2/1p6/3NP3/1BN1B3/PP3PPP/2RQ1RK1 b - - 0 1"), depth);
    std::string output = testing::internal::GetCapturedStdout();
    std::vector<std::string> commands = StringUtils::split(output.substr(output.find("info depth " + depth_str)), ' ');

    const auto pos = std::ranges::find(commands, "nps");
    const auto index = std::distance(commands.begin(), pos );
    const int nps = std::stoi(commands[index + 1]);
    std::cout << "Nodes per second depth " + depth_str + ": " << nps << std::endl;

    if constexpr (kTestEnvironment == Environment::MacOS) {
        std::cout << "On Mac" << std::endl;
        EXPECT_GE(nps, 3160000);
    } else {
        std::cout << "On Linux" << std::endl;
        EXPECT_GE(nps, 500000);
    }
}

TEST(performance, endgame) {
    constexpr int depth = 10;
    const string depth_str = std::to_string(depth);

    // Endgame position
    startSearch(FenReader::read("8/5pk1/3p2p1/1r1Pp3/4P3/2K3P1/5P2/3R4 w - - 0 1"), depth);
    std::string output = testing::internal::GetCapturedStdout();
    std::vector<std::string> commands = StringUtils::split(output.substr(output.find("info depth " + depth_str)), ' ');

    const auto pos = std::ranges::find(commands, "nps");
    const auto index = std::distance(commands.begin(), pos );
    const int nps = std::stoi(commands[index + 1]);
    std::cout << "Nodes per second depth " + depth_str + ": " << nps << std::endl;

    if constexpr (kTestEnvironment == Environment::MacOS) {
        std::cout << "On Mac" << std::endl;
        EXPECT_GE(nps, 4120000);
    } else {
        std::cout << "On Linux" << std::endl;
        EXPECT_GE(nps, 900000);
    }
}