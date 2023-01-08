#include <gtest/gtest.h>
#include <chrono>
#include <cassert> 
#include "magicmoves.h"

int main(int argc, char** argv) {
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

	::testing::InitGoogleTest(&argc, argv);
	initmagicmoves();
	int result = RUN_ALL_TESTS();

	// current performance for perft 5 startposition: ~ 8,000,000 nps in x64-Debug mode
	long long testtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
	assert(testtime < 10500, "Total testing time must be lower than 10,5 seconds");
	return result;
}	