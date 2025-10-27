#include "position/fen.h"

Fen::Fen(U64(&piece_list)[COLORS][PIECES], int color, int castling_rights, U64 enpassant_square, int half_moves, int full_moves)
	: Position(piece_list, color, castling_rights, enpassant_square) {

	Fen::half_moves = half_moves;
	Fen::full_moves = full_moves;
}

Fen::~Fen() = default;