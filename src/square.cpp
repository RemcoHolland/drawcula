#include "square.h"

bool square::isAttacked(int color, const Board& board, U64 square) {

	int enemy_color = color ^ 1;
	int from_nr = Utils::getLS1B(square);

	U64 bishopAttacks = Bmagic(from_nr, board.occupiedBB);
	U64 rookAttacks = Rmagic(from_nr, board.occupiedBB);

	return ((bishopAttacks | rookAttacks) & board.piece_list[enemy_color][QUEEN]) // queen attacks
		|| (rookAttacks & board.piece_list[enemy_color][ROOK]) // rook attacks
		|| (bishopAttacks & board.piece_list[enemy_color][BISHOP]) // bishop attacks
		|| (KNIGHT_MOVES[from_nr] & board.piece_list[enemy_color][KNIGHT]) // knight attacks
		|| (PAWN_ATTACKS[color][from_nr] & board.piece_list[enemy_color][PAWN]) // pawn attacks
		|| (KING_MOVES[from_nr] & board.piece_list[enemy_color][KING]); // king attacks
}