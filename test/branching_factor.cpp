#include <gtest/gtest.h>
#include "search.h"
#include "board.h"
#include "reader/fenreader.h"
#include "stringutils.h"

const Fen start_position = FenReader::read("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
const Fen middlegame_position = FenReader::read("r1bq1rk1/pp1n1ppp/2p1pn2/b2p2B1/2PP4/P1NBPN2/1PQ2PPP/R3K2R w KQ - 0 2");
const Fen endgame_position = FenReader::read("5n2/R7/4pk2/8/5PK1/8/8/8 b - - 0 0");

inline void startSearch(const Fen& fen, int depth) {
	Board board = Board(fen);
	Params params;
	params.max_depth = depth;
	params.movetime = std::chrono::milliseconds(LLONG_MAX);
	Search search = Search();
	testing::internal::CaptureStdout();
	search.start(fen.color, board, params);
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
	std::vector<string> splitted_fen = StringUtils::split(output, ' ');
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
	std::cout << "MEAN BRANCHING FACTOR STARTPOSITION: " << mbf << std::endl;

	//mbf should be between 8.5 and 9.5
	ASSERT_TRUE(mbf > 8.5);
	ASSERT_TRUE(mbf < 9.5);
}

TEST(branching_factor, middlegame_position) {
	int depth = 6;
	startSearch(middlegame_position, depth);
	std::vector<int> nodes = getNodesList(depth);

	double mbf = calculateMeanBranchingFactor(nodes, depth);
	std::cout << "MEAN BRANCHING FACTOR MIDDLEGAME: " << mbf << std::endl;
	//mbf should be between 5 and 6
	ASSERT_TRUE(mbf > 5);
	ASSERT_TRUE(mbf < 6);
}

TEST(branching_factor, endgame_position) {
	int depth = 6;
	startSearch(endgame_position, depth);
	std::vector<int> nodes = getNodesList(depth);

	double mbf = calculateMeanBranchingFactor(nodes, depth);
	std::cout << "MEAN BRANCHING FACTOR ENDGAME: " << mbf << std::endl;

	//mbf should be between 5 and 6
	ASSERT_TRUE(mbf > 5);
	ASSERT_TRUE(mbf < 6);
}