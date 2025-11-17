#include <gtest/gtest.h>
#include "perft.h"
#include "reader/fenreader.h"

U64 perft(int depth, const Fen& fen) {
	Perft perft = Perft(false, depth);
	Board board = Board(fen);
	int color = fen.color;
	return perft.calculate(color, board);
}
TEST(complexfen, illegal_enpassant_1) {
	Fen fen = FenReader::read("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1");
	U64 nodes = perft(6, fen);
	EXPECT_EQ(nodes, 1134888);
}

TEST(complexfen, illegal_enpassant_2) {
	Fen fen = FenReader::read("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1");
	U64 nodes = perft(6, fen);
	EXPECT_EQ(nodes, 1015133);
}

TEST(complexfen, enpassant_check) {
	Fen fen = FenReader::read("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1");
	U64 nodes = perft(6, fen);
	EXPECT_EQ(nodes, 1440467);
}

TEST(complexfen, short_castling_check) {
	Fen fen = FenReader::read("5k2/8/8/8/8/8/8/4K2R w K - 0 1");
	U64 nodes = perft(6, fen);
	EXPECT_EQ(nodes, 661072);
}

TEST(complexfen, long_castling_check) {
	Fen fen = FenReader::read("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1");
	U64 nodes = perft(6, fen);
	EXPECT_EQ(nodes, 803711);
}

TEST(complexfen, castle_rights) {
	Fen fen = FenReader::read("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1");
	U64 nodes = perft(4, fen);
	EXPECT_EQ(nodes, 1274206);
}

TEST(complexfen, castling_prevented) {
	Fen fen = FenReader::read("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1");
	U64 nodes = perft(4, fen);
	EXPECT_EQ(nodes, 1720476);
}

TEST(complexfen, discovered_check) {
	Fen fen = FenReader::read("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1");
	U64 nodes = perft(5, fen);
	EXPECT_EQ(nodes, 1004658);
}

TEST(complexfen, promote_out_of_check) {
	Fen fen = FenReader::read("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1");
	U64 nodes = perft(6, fen);
	EXPECT_EQ(nodes, 3821001);
}

TEST(complexfen, promote_give_check) {
	Fen fen = FenReader::read("4k3/1P6/8/8/8/8/K7/8 w - - 0 1");
	U64 nodes = perft(6, fen);
	EXPECT_EQ(nodes, 217342);
}

TEST(complexfen, underpromote_give_check) {
	Fen fen = FenReader::read("8/P1k5/K7/8/8/8/8/8 w - - 0 1");
	U64 nodes = perft(6, fen);
	EXPECT_EQ(nodes, 92683);
}

TEST(complexfen, self_stalemate) {
	Fen fen = FenReader::read("K1k5/8/P7/8/8/8/8/8 w - - 0 1");
	U64 nodes = perft(6, fen);
	EXPECT_EQ(nodes, 2217);
}

TEST(complexfen, stalemate_and_checkmate_1) {
	Fen fen = FenReader::read("8/k1P5/8/1K6/8/8/8/8 w - - 0 1");
	U64 nodes = perft(7, fen);
	EXPECT_EQ(nodes, 567584);
}

TEST(complexfen, stalemate_and_checkmate_2) {
	Fen fen = FenReader::read("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1");
	U64 nodes = perft(4, fen);
	EXPECT_EQ(nodes, 23527);
}

TEST(complexfen, kiwipete) {
	Fen fen = FenReader::read("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
	U64 nodes = perft(4, fen);
	EXPECT_EQ(nodes, 4085603);
}

TEST(complexfen, multiple_promotions) {
	Fen fen = FenReader::read("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
	U64 nodes = perft(5, fen);
	EXPECT_EQ(nodes, 3605103);
}

TEST(complexfen, bug_catcher) {
	Fen fen = FenReader::read("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
	U64 nodes = perft(4, fen);
	EXPECT_EQ(nodes, 2103487);
}

TEST(complexfen, complex_middlegame) {
	Fen fen = FenReader::read("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
	U64 nodes = perft(5, fen);
	EXPECT_EQ(nodes, 15833292);
}

TEST(complexfen, multiple_queens) {
	Fen fen = FenReader::read("8/6kR/8/8/8/bq6/1rqqqqqq/K1nqnbrq b - - 0 1");
	U64 nodes = perft(5, fen);
	EXPECT_EQ(nodes, 4634384);
}
