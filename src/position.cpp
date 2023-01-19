#include "position.h"

Position::Position(U64(&piece_list)[COLORS][PIECES], int color, int castling_rights, U64 enpassant_square, int half_moves, int full_moves) {
	for (int i = 0; i < COLORS; i++) {
		for (int j = 0; j < PIECES; j++) {
			Position::piece_list[i][j] = piece_list[i][j];
		}
	}
	Position::color = color;
	Position::castling_rights = castling_rights;
	Position::enpassant_square = enpassant_square;
	Position::half_moves = half_moves;
	Position::full_moves = full_moves;
}

Position::~Position() {
}
