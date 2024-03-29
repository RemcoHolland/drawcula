#include <chrono>
#include <iostream>
#include "perft.h"
#include "square.h"
#include "stringutils.h"
#include "movegen.h"

Perft::Perft(bool divide, int search_depth) {
	Perft::divide = divide;
	Perft::search_depth = search_depth;
}

U64 Perft::calculate(int color, Board board) {
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	negaMax(color, board, search_depth);
	long long searchtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
	searchtime = std::max(1LL, searchtime);
	long long nps = nodes / searchtime * 1000;
	std::cout << "nodes: " << nodes << " searchtime: " << searchtime << " ms" << " nps: " << nps << std::endl;
	return nodes;
}

Perft::~Perft() {
}

//Improvement: make a root negaMax, see chess programming network.
void Perft::negaMax(int color, Board& board, int depth) {
	if (depth == 0) {
		root_nodes++;
		nodes++;
		return;
	}

	Movegen movegen = Movegen();
	movegen.generateMoves(color, board);

	for (int move : movegen.moves) {
		int unmake_info = board.makeMove(color, move);

		if (!square::isAttacked(color, board, board.piece_list[color][KING])) {
			negaMax(color ^ 1, board, depth - 1);

			if (divide && depth == search_depth) {
				printRootNodes(move);
				root_nodes = 0;
			}
		}
		board.unmakeMove(color, move, unmake_info);
	}
	return;
}

void Perft::printRootNodes(int move) {
	std::cout << StringUtils::moveToStringAN(move) << ' ' << root_nodes << std::endl;
}
