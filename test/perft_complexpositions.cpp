#include <gtest/gtest.h>
#include "perft.h"
#include "fenreader.h"

uint64_t perft(int depth, FenInfo position) {
	Perft perft = Perft(false, depth);
	Board board = Board(position);
	int color = position.getColor();
	return perft.calculate(color, board);
}

TEST(complexposition, illegal_enpassant_1) {
	FenInfo position = FenReader::read("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1");
	uint64_t nodes = perft(6, position);
	EXPECT_EQ(nodes, 1134888);
}

TEST(complexposition, illegal_enpassant_2) {
	FenInfo position = FenReader::read("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1");
	uint64_t nodes = perft(6, position);
	EXPECT_EQ(nodes, 1015133);
}

TEST(complexposition, enpassant_check) {
	FenInfo position = FenReader::read("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1");
	uint64_t nodes = perft(6, position);
	EXPECT_EQ(nodes, 1440467);
}

TEST(complexposition, short_castling_check) {
	FenInfo position = FenReader::read("5k2/8/8/8/8/8/8/4K2R w K - 0 1");
	uint64_t nodes = perft(6, position);
	EXPECT_EQ(nodes, 661072);
}

TEST(complexposition, long_castling_check) {
	FenInfo position = FenReader::read("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1");
	uint64_t nodes = perft(6, position);
	EXPECT_EQ(nodes, 803711);
}

TEST(complexposition, castle_rights) {
	FenInfo position = FenReader::read("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1");
	uint64_t nodes = perft(4, position);
	EXPECT_EQ(nodes, 1274206);
}

TEST(complexposition, castling_prevented) {
	FenInfo position = FenReader::read("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1");
	uint64_t nodes = perft(4, position);
	EXPECT_EQ(nodes, 1720476);
}

TEST(complexposition, discovered_check) {
	FenInfo position = FenReader::read("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1");
	uint64_t nodes = perft(5, position);
	EXPECT_EQ(nodes, 1004658);
}

TEST(complexposition, promote_out_of_check) {
	FenInfo position = FenReader::read("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1");
	uint64_t nodes = perft(6, position);
	EXPECT_EQ(nodes, 3821001);
}

TEST(complexposition, promote_give_check) {
	FenInfo position = FenReader::read("4k3/1P6/8/8/8/8/K7/8 w - - 0 1");
	uint64_t nodes = perft(6, position);
	EXPECT_EQ(nodes, 217342);
}

TEST(complexposition, underpromote_give_check) {
	FenInfo position = FenReader::read("8/P1k5/K7/8/8/8/8/8 w - - 0 1");
	uint64_t nodes = perft(6, position);
	EXPECT_EQ(nodes, 92683);
}

TEST(complexposition, self_stalemate) {
	FenInfo position = FenReader::read("K1k5/8/P7/8/8/8/8/8 w - - 0 1");
	uint64_t nodes = perft(6, position);
	EXPECT_EQ(nodes, 2217);
}

TEST(complexposition, stalemate_and_checkmate_1) {
	FenInfo position = FenReader::read("8/k1P5/8/1K6/8/8/8/8 w - - 0 1");
	uint64_t nodes = perft(7, position);
	EXPECT_EQ(nodes, 567584);
}

TEST(complexposition, stalemate_and_checkmate_2) {
	FenInfo position = FenReader::read("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1");
	uint64_t nodes = perft(4, position);
	EXPECT_EQ(nodes, 23527);
}

TEST(complexposition, kiwipete) {
	FenInfo position = FenReader::read("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
	uint64_t nodes = perft(4, position);
	EXPECT_EQ(nodes, 4085603);
}

TEST(complexposition, multiple_promotions) {
	FenInfo position = FenReader::read("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
	uint64_t nodes = perft(5, position);
	EXPECT_EQ(nodes, 3605103);
}

TEST(complexposition, bug_catcher) {
	FenInfo position = FenReader::read("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
	uint64_t nodes = perft(4, position);
	EXPECT_EQ(nodes, 2103487);
}