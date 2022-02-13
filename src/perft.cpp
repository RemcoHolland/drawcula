#include "perft.h"

Perft::Perft(bool divide, int search_depth) {
	Perft::divide = divide;
	Perft::search_depth = search_depth;
}

uint64_t Perft::calculate(int color, Board board) {
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	negaMax(color, board, search_depth);
	long long searchtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
	std::cout << "nodes: " << nodes << " searchtime: " << searchtime << " ms" << std::endl;
	return nodes;
}

Perft::~Perft() {
}

//Improvement: make a root negaMax, see chess programming network.
void Perft::negaMax(int color, Board board, int depth) {
	if (depth == 0) {
		root_nodes++;
		nodes++;
		return;
	}

	Movelist movelist = Movelist();
	movelist.generateMoves(color, board);

	for (int move : movelist.moves) {
		MoveInfo moveInfo = board.makeMove(color, move);

		if (!Square::isAttacked(color, board, board.piece_list[color == WHITE ? WHITE_KING : BLACK_KING])) {
			negaMax(color ^ 1, board, depth - 1);

			if (divide && depth == search_depth) {
				printRootNodes(move);
				root_nodes = 0;
			}
		}
		board.unmakeMove(color, move, moveInfo);
	}
	return;
}

void Perft::printRootNodes(int move) {
	std::cout << StringUtils::moveToString(move) << ' ' << root_nodes << std::endl;
}
