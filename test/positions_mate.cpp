#include <gtest/gtest.h>
#include "search.h"
#include "board.h"
#include "fenreader.h"

inline void startSearch(Position position, int depth) {	
	Board board = Board(position);
	struct Time time;
	time.movetime = std::chrono::milliseconds(LLONG_MAX);
	Search search = Search();
	testing::internal::CaptureStdout();
	search.start(position.color, depth, board, time);
}

TEST(mateposition, mate_in_1_with_king_and_rook) {
	startSearch(FenReader::read("5k2/8/5K2/8/8/8/3R4/8 w - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(output.find("info depth 2 score mate 1") != std::string::npos);
	ASSERT_TRUE(output.find("info depth 3 score mate 1") != std::string::npos);
	ASSERT_TRUE(output.find("info depth 4 score mate 1") != std::string::npos);
	ASSERT_TRUE(output.find("info depth 5 score mate 1") != std::string::npos);
	ASSERT_TRUE(output.find("pv d2d8") != std::string::npos);
	ASSERT_TRUE(output.find("bestmove d2d8") != std::string::npos);
}

TEST(mateposition, mate_in_2_with_king_and_queen) {
	startSearch(FenReader::read("8/8/8/8/8/2kq4/8/K7 w - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(output.find("info depth 5 score mate -2") != std::string::npos);
	ASSERT_TRUE(output.find("pv a1a2 d3d1 a2a3 d1a1") != std::string::npos);
	ASSERT_TRUE(output.find("bestmove a1a2") != std::string::npos);
}

TEST(mateposition, mate_in_2_sacrifice_rook) {
	startSearch(FenReader::read("5Kbk/6pp/6P1/8/8/8/8/7R w - - 0 1"), 5);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(output.find("info depth 4 score mate 2") != std::string::npos);
	ASSERT_TRUE(output.find("info depth 5 score mate 2") != std::string::npos);
	ASSERT_TRUE(output.find("pv h1h6 g7h6 g6g7") != std::string::npos);
	ASSERT_TRUE(output.find("bestmove h1h6") != std::string::npos);
}

TEST(mateposition, mate_in_3_by_promotion) {
	startSearch(FenReader::read("8/7b/7b/p7/Pp2k3/1P6/KP2p2p/3N4 b - - 0 1"), 6);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(output.find("info depth 6 score mate 3") != std::string::npos);
	ASSERT_TRUE(output.find("pv e2d1b a2a1 d1b3 a1b1 h2h1q") != std::string::npos);
	ASSERT_TRUE(output.find("bestmove e2d1b") != std::string::npos);
}
