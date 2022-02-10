#include "position.h"

FenInfo::FenInfo(uint64_t* piece_list, int color, int castling_rights, uint64_t enpassant_square, int half_moves, int full_moves) {
	for (int i = 0; i < 12; i++) {
		FenInfo::piece_list[i] = piece_list[i];
	}
	FenInfo::color = color;
	FenInfo::castling_rights = castling_rights;
	FenInfo::enpassant_square = enpassant_square;
	FenInfo::half_moves = half_moves;
	FenInfo::full_moves = full_moves;
}

FenInfo::~FenInfo() {
}
