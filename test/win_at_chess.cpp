#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "reader/epdreader.h"
#include "search.h"
#include "stringutils.h"

#include<iostream>
#include<filesystem>

inline int startSearch(const Epd& epd, int movetime) {
	Board board = Board(epd);
	Params params;
	params.movetime = std::chrono::milliseconds(movetime);
	Search search = Search();
	return search.start(epd.color, board, params);
}
namespace fs = std::filesystem;
TEST(wac, all_tests) {

	std::string path = "."; // Current directory
	for(const auto&entry:fs::directory_iterator(path)) {
		std::cout<<entry.path().filename()<<std::endl;
	}


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

		int movetime = 2500;
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
	std::cout << "SUCCES: " << succes << std::endl;
	std::cout << "FAILED: " << failed << std::endl;
	EXPECT_GE(succes, 146);
	EXPECT_LE(failed, 55);
}