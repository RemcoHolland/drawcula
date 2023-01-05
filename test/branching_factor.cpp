#include <gtest/gtest.h>
#include "search.h"
#include "board.h"
#include "fenreader.h"

const Position start_position = FenReader::read("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

inline void startSearch(Position position, int depth) {
	Board board = Board(position);
	struct Time time;
	time.movetime = std::chrono::milliseconds(LLONG_MAX);
	Search search = Search();
	testing::internal::CaptureStdout();
	search.start(position.color, depth, board, time);
}

double calculateMeanBranchingFactor(std::vector<int> nodes, int depth) {
	double ebf_sum{ 0 };
	for (int i = 2; i < nodes.size(); i++) {
		double ebf = std::sqrt(nodes[i] / nodes[i - 2]); // calculate effective branching factor
		ebf_sum += ebf;
	}
	double mbf = ebf_sum / (depth - 2); // calculate mean branching factor
	return mbf;
}

std::vector<int> getNodesList(int depth) {
	std::string output = testing::internal::GetCapturedStdout();
	std::vector<string> splitted_fen = split(output, ' ');
	std::vector<int> nodes;
	for (int i = 0; i < splitted_fen.size(); i++) {
		if (splitted_fen[i] == "nodes") {
			nodes.push_back(std::stoi(splitted_fen[i + 1]));  // make a list of all nodes
		}
	}
	EXPECT_EQ(nodes.size(), depth);
	return nodes;
}

TEST(branching_factor, start_position) {
	int depth = 6;	
	startSearch(start_position, depth);
	std::vector<int> nodes = getNodesList(depth);

	double mbf = calculateMeanBranchingFactor(nodes, depth);

	//mbf should be between 9 and 10
	ASSERT_TRUE(mbf > 9);
	ASSERT_TRUE(mbf < 10);
}

TEST(branching_factor, middlegame_position) {
	int depth = 6;
	startSearch(FenReader::read("r1bq1rk1/pp1n1ppp/2p1pn2/b2p2B1/2PP4/P1NBPN2/1PQ2PPP/R3K2R w KQ - 0 2"), depth);
	std::vector<int> nodes = getNodesList(depth);

	double mbf = calculateMeanBranchingFactor(nodes, depth);

	//mbf should be between 10 and 11
	ASSERT_TRUE(mbf > 10);
	ASSERT_TRUE(mbf < 11);
}


TEST(branching_factor, endgame_position) {
	int depth = 6;
	startSearch(FenReader::read("5n2/R7/4pk2/8/5PK1/8/8/8 b - - 0 0"), depth);
	std::vector<int> nodes = getNodesList(depth);

	double mbf = calculateMeanBranchingFactor(nodes, depth);

	//mbf should be between 4 and 5
	ASSERT_TRUE(mbf > 4);
	ASSERT_TRUE(mbf < 5);
}