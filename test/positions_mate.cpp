#include <gtest/gtest.h>
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

TEST(mateposition, mate_in_1_with_king_and_rook) {
	startSearch(FenReader::read("5k2/8/5K2/8/8/8/3R4/8 w - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_NE(output.find("info depth 2 score mate 1"), std::string::npos);
	ASSERT_NE(output.find("info depth 3 score mate 1"), std::string::npos );
	ASSERT_NE(output.find("info depth 4 score mate 1"), std::string::npos);
	ASSERT_NE(output.find("info depth 5 score mate 1"), std::string::npos);
	ASSERT_NE(output.find("pv d2d8"), std::string::npos);
	ASSERT_NE(output.find("bestmove d2d8"), std::string::npos);
}

TEST(mateposition, mate_in_2_with_long_castle) {
	startSearch(FenReader::read("8/8/8/2P3R1/5B2/2rP1p2/p1P1PP2/RnQ1K2k w Q - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_NE(output.find("info depth 5 score mate 2"), std::string::npos);
	ASSERT_NE(output.find("pv c1b2 f3e2 b2b7"), std::string::npos);
	ASSERT_NE(output.find("bestmove c1b2"), std::string::npos);
}

TEST(mateposition, mate_in_2_own_king_negative_score) {
	startSearch(FenReader::read("8/8/8/5b2/3b4/8/5k2/7K w - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_NE(output.find("info depth 5 score mate -2"), std::string::npos);
	ASSERT_NE(output.find("pv h1h2 d4e5 h2h1 f5e4"), std::string::npos);
	ASSERT_NE(output.find("bestmove h1h2"), std::string::npos);
}

TEST(mateposition, mate_in_2_sacrifice_rook) {
	startSearch(FenReader::read("5Kbk/6pp/6P1/8/8/8/8/7R w - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_NE(output.find("info depth 4 score mate 2"), std::string::npos);
	ASSERT_NE(output.find("info depth 5 score mate 2"), std::string::npos);
	ASSERT_NE(output.find("pv h1h6 g7h6 g6g7"), std::string::npos);
	ASSERT_NE(output.find("bestmove h1h6"), std::string::npos);
}

TEST(mateposition, mate_in_3_by_promotion) {
	startSearch(FenReader::read("8/7b/7b/p7/Pp2k3/1P6/KP2p2p/3N4 b - - 0 1"), 6);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_NE(output.find("info depth 6 score mate 3"), std::string::npos);
	ASSERT_NE(output.find("pv e2d1b a2a1 d1b3 a1b1 h2h1q"), std::string::npos);
	ASSERT_NE(output.find("bestmove e2d1b"), std::string::npos);
}
