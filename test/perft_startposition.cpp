#include <gtest/gtest.h>
#include "perft.h"
#include "fenreader.h"

Position start_position = FenReader::read("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

uint64_t perft(int depth) {
	Perft perft = Perft(false, depth);
	Board board = Board(start_position);
	int color = start_position.color;
	return perft.calculate(color, board);
}

TEST(startposition, perft_1) {
	uint64_t nodes = perft(1);
	EXPECT_EQ(nodes, 20);
}

TEST(startposition, perft_2) {
	uint64_t nodes = perft(2);
	EXPECT_EQ(nodes, 400);
}

TEST(startposition, perft_3) {
	uint64_t nodes = perft(3);
	EXPECT_EQ(nodes, 8902);
}

TEST(startposition, perft_4) {
	uint64_t nodes = perft(4);
	EXPECT_EQ(nodes, 197281);
}

//TEST(startposition, perft_5) {
//	uint64_t nodes = perft(5);
//	EXPECT_EQ(nodes, 4865609);
//}

//TEST(startposition, perft_6) {
//	uint64_t nodes = perft(6);
//	EXPECT_EQ(nodes, 119060324);
//}

//TEST(startposition, perft_7) {
//	uint64_t nodes = perft(7);
//	EXPECT_EQ(nodes, 3195901860);
//}

//TEST(startposition, perft_8) {
//	uint64_t nodes = perft(8);
//	EXPECT_EQ(nodes, 84998978956);
//}






