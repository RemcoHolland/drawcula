#include "square.h"
#include "magicmoves.h"
#include "utils.h"

bool square::isAttacked(const int color, const Board& board, const U64 square) {

	const int enemy_color = color ^ 1;
	const int from_nr = Utils::getLS1B(square);

	const U64 bishopAttacks = Bmagic(from_nr, board.occupiedBB);
	const U64 rookAttacks = Rmagic(from_nr, board.occupiedBB);

	return (bishopAttacks | rookAttacks) & board.piece_list[enemy_color][QUEEN] // queen attacks
		|| rookAttacks & board.piece_list[enemy_color][ROOK] // rook attacks
		|| bishopAttacks & board.piece_list[enemy_color][BISHOP] // bishop attacks
		|| KNIGHT_MOVES[from_nr] & board.piece_list[enemy_color][KNIGHT] // knight attacks
		|| PAWN_ATTACKS[color][from_nr] & board.piece_list[enemy_color][PAWN] // pawn attacks
		|| KING_MOVES[from_nr] & board.piece_list[enemy_color][KING]; // king attacks
}