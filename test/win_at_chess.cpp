#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "reader/epdreader.h"
#include "search.h"
#include "stringutils.h"

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
	int succes = 0;
	int failed = 0;
	testing::internal::CaptureStdout();
	
	while (std::getline(file, str)) {
		Epd epd = EpdReader::read(str);

		int movetime = 3000;
		int bestmove = startSearch(epd, movetime);
		string bestmoveStr = StringUtils::moveToStringLAN(bestmove);
		string bm = epd.operation.find("bm") -> second;
		if (bm == bestmoveStr || bm == (bestmoveStr + '+')) {  // add '+' for check
			succes++;
		} else {
			failed++;
		}
	}
	testing::internal::GetCapturedStdout();
	std::cout << "SUCCESS: " << succes << std::endl;
	std::cout << "FAILED: " << failed << std::endl;
	EXPECT_GE(succes, 124);
	EXPECT_LE(failed, 77);
}