#include <gtest/gtest.h>
#include "uci.h"
#include "fenreader.h"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

TEST(play_game, startpos) {
	Uci uci = Uci();
	testing::internal::CaptureStdout();
	uci.position({ "position", "startpos", "moves", "e2e4" "c7c5" "g1f3" });
	uci.go({ "go", "movetime" ,"1000" });
	std::this_thread::sleep_for(1500ms);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(output.find("bestmove a7a6") != std::string::npos);
}

TEST(play_game, fen) {
	Uci uci = Uci();
	testing::internal::CaptureStdout();
	uci.position({ "position", "fen", "2bqkbn1/2pppp2/np2N3/r3P1p1/p2N2B1/5Q2/PPPPKPP1/RNB2r2", "w", "KQkq", "-", "0", "1" });
	uci.go({ "go", "movetime" ,"5000" });
	std::this_thread::sleep_for(5500ms);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(output.find("bestmove f3f7") != std::string::npos);
}