#include "square.h"
#include "magicmoves.h"
#include "utils.h"

bool square::isAttacked(const int color, const Board& board, const U64 square) {

	const int enemy_color = color ^ 1;
	const int from_nr = Utils::getLS1B(square);

	return Qmagic(from_nr, board.occupiedBB) & board.piece_list[enemy_color][QUEEN] // queen attacks
		|| Rmagic(from_nr, board.occupiedBB) & board.piece_list[enemy_color][ROOK] // rook attacks
		|| Bmagic(from_nr, board.occupiedBB) & board.piece_list[enemy_color][BISHOP] // bishop attacks
		|| KNIGHT_MOVES[from_nr] & board.piece_list[enemy_color][KNIGHT] // knight attacks
		|| PAWN_ATTACKS[color][from_nr] & board.piece_list[enemy_color][PAWN] // pawn attacks
		|| KING_MOVES[from_nr] & board.piece_list[enemy_color][KING]; // king attacks
}