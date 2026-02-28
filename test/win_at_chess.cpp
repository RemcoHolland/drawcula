#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "reader/epdreader.h"
#include "search.h"
#include "stringutils.h"
#include "util/test_helper.h"

inline int startSearch(const Epd& epd, int movetime) {
	Board board = Board(epd);
	Params params;
	params.movetime = std::chrono::milliseconds(movetime);
	Search search = Search();
	return search.start(epd.color, board, params);
}

TEST(wac, all_tests) {
	std::ifstream file("wac.epd");
	if (!file.is_open()) {
		FAIL() << "Could not open file wac.epd";
	}

	std::string str;
	int success = 0;
	int failed = 0;
	testing::internal::CaptureStdout();
	
	while (std::getline(file, str)) {
		Epd epd = EpdReader::read(str);

		int movetime = 3000;
		int bestmove = startSearch(epd, movetime);
		string bestmoveStr = StringUtils::moveToStringLAN(bestmove);
		string bm = epd.operation.find("bm") -> second;
		if (bm == bestmoveStr || bm == (bestmoveStr + '+')) {  // add '+' for check
			success++;
		} else {
			failed++;
		}
	}
	testing::internal::GetCapturedStdout();
	std::cout << "SUCCESS: " << success << std::endl;
	std::cout << "FAILED: " << failed << std::endl;

	if constexpr (kTestEnvironment == Environment::MacOS) {
		std::cout << "On Mac" << std::endl;
		EXPECT_GE(success, 144);
		EXPECT_LE(failed, 57);
	} else {
		std::cout << "On Linux" << std::endl;
		EXPECT_GE(success, 110);
		EXPECT_LE(failed, 91);
	}
}