#pragma once
#include "board.h"
#include "position/position.h"

typedef uint64_t U64;

extern U64 ZOBRIST[COLORS][PIECES][SQUARES];
extern U64 BLACK_TO_MOVE;

namespace zobrist {
	void init();
	U64 hash(const Position&);
}