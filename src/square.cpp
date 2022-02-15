#include "square.h"

bool square::isAttacked(int color, const Board& board, uint64_t square) {

	int from_nr = Utils::getLS1B(square);
	int king_color = NR_PIECES * color;

	uint64_t pawnAttacks = PAWN_ATTACKS[color][from_nr];
	uint64_t knightAttacks = KNIGHT_MOVES[from_nr];
	uint64_t bishopAttacks = Bmagic(from_nr, board.occupiedBB);
	uint64_t rookAttacks = Rmagic(from_nr, board.occupiedBB);
	uint64_t queenAttacks = bishopAttacks | rookAttacks;
	uint64_t kingAttacks = KING_MOVES[from_nr];

	return (queenAttacks & board.piece_list[BLACK_QUEEN - king_color])
		|| (rookAttacks & board.piece_list[BLACK_ROOK - king_color])
		|| (bishopAttacks & board.piece_list[BLACK_BISHOP - king_color])
		|| (knightAttacks & board.piece_list[BLACK_KNIGHT - king_color])
		|| (pawnAttacks & board.piece_list[BLACK_PAWN - king_color])
		|| (kingAttacks & board.piece_list[BLACK_KING - king_color]);
}