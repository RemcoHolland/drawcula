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

TEST(mateposition, mate_in_1_with_king_and_rook) {
	startSearch(FenReader::read("5k2/8/5K2/8/8/8/3R4/8 w - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_THAT(output, testing::HasSubstr("info depth 2 score mate 1"));
	EXPECT_THAT(output, testing::HasSubstr("info depth 3 score mate 1"));
	EXPECT_THAT(output, testing::HasSubstr("info depth 4 score mate 1"));
	EXPECT_THAT(output, testing::HasSubstr("info depth 5 score mate 1"));
	EXPECT_THAT(output, testing::HasSubstr("pv d2d8"));
	EXPECT_THAT(output, testing::HasSubstr("bestmove d2d8"));
}

TEST(mateposition, mate_in_2_with_long_castle) {
	startSearch(FenReader::read("8/8/8/2P3R1/5B2/2rP1p2/p1P1PP2/RnQ1K2k w Q - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_THAT(output, testing::HasSubstr("info depth 5 score mate 2"));
	EXPECT_THAT(output, testing::HasSubstr("pv c1b2 f3e2 b2b7"));
	EXPECT_THAT(output, testing::HasSubstr("bestmove c1b2"));
}

TEST(mateposition, mate_in_2_own_king_negative_score) {
	startSearch(FenReader::read("8/8/8/5b2/3b4/8/5k2/7K w - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_THAT(output, testing::HasSubstr("info depth 5 score mate -2"));
	EXPECT_THAT(output, testing::HasSubstr("pv h1h2 d4e5 h2h1 f5e4"));
	EXPECT_THAT(output, testing::HasSubstr("bestmove h1h2"));
}

TEST(mateposition, mate_in_2_sacrifice_rook) {
	startSearch(FenReader::read("5Kbk/6pp/6P1/8/8/8/8/7R w - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_THAT(output, testing::HasSubstr("info depth 4 score mate 2"));
	EXPECT_THAT(output, testing::HasSubstr("info depth 5 score mate 2"));
	EXPECT_THAT(output, testing::HasSubstr("pv h1h6 g7h6 g6g7"));
	EXPECT_THAT(output, testing::HasSubstr("bestmove h1h6"));
}

TEST(mateposition, mate_in_3_by_promotion) {
	startSearch(FenReader::read("8/7b/7b/p7/Pp2k3/1P6/KP2p2p/3N4 b - - 0 1"), 6);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_THAT(output, testing::HasSubstr("info depth 6 score mate 3"));
	EXPECT_THAT(output, testing::HasSubstr("pv e2d1b a2a1 d1b3 a1b1 h2h1q"));
	EXPECT_THAT(output, testing::HasSubstr("bestmove e2d1b"));
}
