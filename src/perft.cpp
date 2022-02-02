#include "perft.h"

Perft::Perft(bool divide, int search_depth) {
	Perft::divide = divide;
	Perft::search_depth = search_depth;
}

uint64_t Perft::calculate(int color, Board board) {
	negaMax(color, board, search_depth);
	std::cout << "nodes: " << nodes << std::endl;
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
	std::list<Move> moves = movelist.getMoves();

	for (std::list<Move>::iterator it = moves.begin(); it != moves.end(); ++it) {
		Move move = *it;
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

void Perft::printRootNodes(Move move) {
	// TODO: Use Move.toString() method

	int square_from = Utils::getLS1B(move.getFrom());
	int square_to = Utils::getLS1B(move.getTo());
	char promotion = Piece::getPromotion(move.getPromotion());

	std::cout << StringUtils::getFile(square_from) << StringUtils::getRank(square_from) << StringUtils::getFile(square_to) << StringUtils::getRank(square_to) << promotion << ' ' << root_nodes << std::endl;

}
