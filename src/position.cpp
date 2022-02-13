#include "position.h"

Position::Position(uint64_t* piece_list, int color, int castling_rights, uint64_t enpassant_square, int half_moves, int full_moves) {
	for (int i = 0; i < 12; i++) {
		Position::piece_list[i] = piece_list[i];
	}
	Position::color = color;
	Position::castling_rights = castling_rights;
	Position::enpassant_square = enpassant_square;
	Position::half_moves = half_moves;
	Position::full_moves = full_moves;
}

Position::~Position() {
}
