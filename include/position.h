#pragma once
#include <cstdint>
#include "piece.h"

class Position {
public:
	Position(uint64_t*, int, int, uint64_t, int, int);
	uint64_t piece_list[TOTAL_PIECES] = { 0 };
	int color = 0;
	int castling_rights = 0;
	uint64_t enpassant_square = 0;
	int half_moves = 0;
	int full_moves = 0;
	~Position();

private:

};

