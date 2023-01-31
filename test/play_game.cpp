#include <gtest/gtest.h>
#include "uci.h"
#include "fenreader.h"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

TEST(play_game, startpos) {
	Uci uci = Uci();
	testing::internal::CaptureStdout();
	uci.position({ "position", "startpos", "moves", "e2e4", "b7b6", "d2d4", "e7e6", "g1f3", "c7c6", "b1c3", "f8b4", "f1c4", "b6b5", "c4b3", "a7a6", "e1g1", "g8f6", "a2a3", "b4c3", "b2c3", "f6e4", "d1d3", "e4d6", "c1f4", "d6b7", "c3c4", "d7d6", "f1e1", "a6a5", "c4b5", "a5a4", "b3a2", "d6d5", "a1b1", "c6c5", "a2d5", "d8d5", "f4e5", "c5c4", "d3c3", "f7f6", "e5g3", "e6e5", "d4e5", "f6f5", "e5e6", "h8g8", "f3e5", "f5f4", "g3f4", "g7g5", "f4g3", "b7a5", "e5g4", "g8g6", "c3h8", "e8e7", "h8c8", "b8a6", "c8d7", "d5d7", "e6d7", "e7d7", "g4e5", "d7c8", "e5g6", "h7g6", "b5a6", "a8a6", "b1b8", "c8d7", "h2h4", "g5g4", "e1d1", "d7e7", "b8b4", "g6g5", "h4g5", "a6g6", "b4a4", "a5b7", "a4a7", "g6b6", "g5g6", "c4c3", "g6g7", "e7f7", "d1d7", "f7g8", "d7b7", "b6b7", "a7b7", "g8h7", "a3a4", "h7g8", "a4a5", "g8h7", "a5a6", "h7g6", "g7g8b", "g6f5", "a6a7", "f5e4", "f2f3", "g4f3", "g2f3", "e4e3", "a7a8n", "e3d2", "a8b6", "d2d1", "b6d5", "d1c2", "d5c3", "c2c3", "g8c4", "c3c4", "f3f4", "c4c3", "f4f5", "c3c2", "f5f6", "c2c1", "g3e1", "c1d1", "f6f7", "d1e1", "f7f8q", "e1d1", "b7b1", "d1c2", "f8c5", "c2b1", "c5f2", "b1a1", "f2d2", "a1b1", "g1f2", "b1a1", "f2e3", "a1b1", "e3d3", "b1a1", "d3c3" });
	uci.go({ "go", "movetime" ,"200" });
	std::this_thread::sleep_for(300ms);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(output.find("bestmove a1b1"));
}

TEST(play_game, fen) {
	Uci uci = Uci();
	testing::internal::CaptureStdout();
	// FEN: 2bqkbn1/2pppp2/np2N3/r3P1p1/p2N2B1/5Q2/PPPPKPP1/RNB2r2 w - - 0 1
	uci.position({ "position", "fen", "2bqkbn1/2pppp2/np2N3/r3P1p1/p2N2B1/5Q2/PPPPKPP1/RNB2r2", "w", "-", "-", "0", "1" });
	uci.go({ "go", "movetime" ,"1000" });
	std::this_thread::sleep_for(1100ms);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(output.find("bestmove f3f7"));
}

TEST(play_game, stop_thinking) {
	Uci uci = Uci();
	testing::internal::CaptureStdout();
	uci.go({ "go", "infinite" });
	std::this_thread::sleep_for(100ms);
	uci.stop();
	std::this_thread::sleep_for(100ms);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(output.find("bestmove"));
}