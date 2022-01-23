#include "square.h"

Square::Square() {
}

bool Square::isAttacked(int color, Board board, uint64_t square) {

	int from_nr = Utils::getLS1B(square);
	int king_color = NR_PIECES * color;

	// TODO: make a pawn attacks bb just like kings and knights
	uint64_t pawnAttacks;
	if (color == WHITE) {
		pawnAttacks = ((square & ~Board::FILE_1) << 7) | ((square & ~Board::FILE_8) << 9);
	} else {
		pawnAttacks = ((square & ~Board::FILE_1) >> 9) | ((square & ~Board::FILE_8) >> 7);
	}

	uint64_t knightAttacks = board.getKnightMoves(from_nr);
	uint64_t bishopAttacks = Bmagic(from_nr, board.getOccupiedBB()) & ~board.getColorBB(color);
	uint64_t rookAttacks = Rmagic(from_nr, board.getOccupiedBB()) & ~board.getColorBB(color);
	uint64_t queenAttacks = bishopAttacks | rookAttacks;
	uint64_t kingAttacks = board.getKingMoves(from_nr);

	if ((pawnAttacks & board.getPiece(BLACK_PAWN - king_color)) || (knightAttacks & board.getPiece(BLACK_KNIGHT - king_color)) ||
		(bishopAttacks & board.getPiece(BLACK_BISHOP - king_color)) || (rookAttacks & board.getPiece(BLACK_ROOK - king_color)) ||
		(queenAttacks & board.getPiece(BLACK_QUEEN - king_color)) || (kingAttacks & board.getPiece(BLACK_KING - king_color))) {
		return true;
	}
	return false;
}

Square::~Square() {
}
