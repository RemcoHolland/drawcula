#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "search.h"
#include "board.h"
#include "reader/fenreader.h"

inline void startSearch(const Fen& fen, int depth) {
    Board board = Board(fen);
    Params params;
    params.max_depth = depth;
    params.movetime = std::chrono::milliseconds(LLONG_MAX);
    Search search = Search();
    testing::internal::CaptureStdout();
    search.start(fen.color, board, params);
}

/**********************************************************************************
 * Test for an illegal enpassant move in this position which the engine sometimes *
 * made because of a bug in the enpassant handling.                               *
 * FEN: 8/7p/8/6P1/8/8/1K2k3/8 w - - 0 1                                          *
 *********************************************************************************/
TEST(bugposition, enpassant_bug) {
    startSearch(FenReader::read("8/7p/8/6P1/8/8/1K2k3/8 w - - 0 1"), 3);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_THAT(output, Not(testing::HasSubstr("pv g5h6")));
    EXPECT_THAT(output, Not(testing::HasSubstr("bestmove g5h6")));
}