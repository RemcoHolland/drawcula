#pragma once
#include <cstdint>
#include "piece.h"
#include "color.h"

typedef uint64_t U64;

class Position {
public:
	Position(U64(&)[COLORS][PIECES], int, int, U64);
	U64 piece_list[COLORS][PIECES] = { {} };
	int color = 0;
	int castling_rights = 0;
	U64 enpassant_square = 0;
	~Position();
};

