#include "square.h"

bool square::isAttacked(int color, const Board& board, uint64_t square) {

	int enemy_color = color ^ 1;
	int from_nr = Utils::getLS1B(square);

	uint64_t bishopAttacks = Bmagic(from_nr, board.occupiedBB);
	uint64_t rookAttacks = Rmagic(from_nr, board.occupiedBB);

	return ((bishopAttacks | rookAttacks) & board.piece_list[enemy_color][QUEEN]) // queen attacks
		|| (rookAttacks & board.piece_list[enemy_color][ROOK]) // rook attacks
		|| (bishopAttacks & board.piece_list[enemy_color][BISHOP]) // bishop attacks
		|| (KNIGHT_MOVES[from_nr] & board.piece_list[enemy_color][KNIGHT]) // knight attacks
		|| (PAWN_ATTACKS[color][from_nr] & board.piece_list[enemy_color][PAWN]) // pawn attacks
		|| (KING_MOVES[from_nr] & board.piece_list[enemy_color][KING]); // king attacks
}