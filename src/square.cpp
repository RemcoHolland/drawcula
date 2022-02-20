#include "square.h"

bool square::isAttacked(int color, const Board& board, uint64_t square) {

	int from_nr = Utils::getLS1B(square);
	int king_color = NR_PIECES * color;

	uint64_t bishopAttacks = Bmagic(from_nr, board.occupiedBB);
	uint64_t rookAttacks = Rmagic(from_nr, board.occupiedBB);

	return ((bishopAttacks | rookAttacks) & board.piece_list[BLACK_QUEEN - king_color]) // queen attacks
		|| (rookAttacks & board.piece_list[BLACK_ROOK - king_color]) // rook attacks
		|| (bishopAttacks & board.piece_list[BLACK_BISHOP - king_color]) // bishop attacks
		|| (KNIGHT_MOVES[from_nr] & board.piece_list[BLACK_KNIGHT - king_color]) // knight attacks
		|| (PAWN_ATTACKS[color][from_nr] & board.piece_list[BLACK_PAWN - king_color]) // pawn attacks
		|| (KING_MOVES[from_nr] & board.piece_list[BLACK_KING - king_color]); // king attacks
}