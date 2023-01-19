#pragma once
#include <cstdint>
#include "piece.h"

typedef uint64_t U64;

class Position {
public:
	Position(U64(&)[COLORS][PIECES], int, int, U64, int, int);
	U64 piece_list[COLORS][PIECES] = { {0} };
	int color = 0;
	int castling_rights = 0;
	U64 enpassant_square = 0;
	int half_moves = 0;
	int full_moves = 0;
	~Position();

private:

};

