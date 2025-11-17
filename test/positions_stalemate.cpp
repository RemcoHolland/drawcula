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

TEST(stalematepositions, stalemate_by_queen_sacrifice) {
	startSearch(FenReader::read("8/8/8/2p2p2/k1P2P1P/2Q2qP1/2K5/8 b - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_THAT(output, testing::HasSubstr("info depth 4 score cp -"));
	EXPECT_THAT(output, testing::HasSubstr("info depth 5 score cp 0"));
	EXPECT_THAT(output, testing::HasSubstr("pv f3d1 c2d1"));
	EXPECT_THAT(output, testing::HasSubstr("bestmove f3d1"));
}