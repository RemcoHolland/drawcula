#include <gtest/gtest.h>
#include "search.h"
#include "board.h"
#include "reader/fenreader.h"

inline void startSearch(Fen fen, int depth) {
	Board board = Board(fen);
	struct Params params;
	params.max_depth = depth;
	params.movetime = std::chrono::milliseconds(LLONG_MAX);
	Search search = Search();
	testing::internal::CaptureStdout();
	search.start(fen.color, board, params);
}

TEST(stalematepositions, stalemate_by_queen_sacrifice) {
	startSearch(FenReader::read("8/8/8/2p2p2/k1P2P1P/2Q2qP1/2K5/8 b - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(output.find("info depth 4 score cp -"));
	ASSERT_TRUE(output.find("info depth 5 score cp 0"));
	ASSERT_TRUE(output.find("pv f3d1 c2d1"));
	ASSERT_TRUE(output.find("bestmove f3d1"));
}