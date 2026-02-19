#include <gtest/gtest.h>
#include "search.h"
#include "include/test_helper.h"

inline void startSearch(const Fen& fen, int depth) {
    Board board = Board(fen);
    Params params;
    params.max_depth = depth;
    params.movetime = std::chrono::milliseconds(LLONG_MAX);
    Search search = Search();
    testing::internal::CaptureStdout();
    search.start(fen.color, board, params);
}

TEST(performance, measure_performance_on_mac) {
    if (TestHelper::kTestEnvironment != TestHelper::Environment::MacOS) {
        std::cout << "Not on Mac" << std::endl;
        GTEST_SKIP() << "Skipping on unsupported environment";
    }
    std::cout << "On Mac" << std::endl;
    // startSearch(FenReader::read("5k2/8/5K2/8/8/8/3R4/8 w - - 0 1"), 5);
    // std::string output = testing::internal::GetCapturedStdout();
    // EXPECT_THAT(output, testing::HasSubstr("info depth 2 score mate 1"));
}
