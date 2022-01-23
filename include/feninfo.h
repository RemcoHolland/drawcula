#pragma once
#include <cstdint>
#include "piece.h"

class FenInfo {
public:
	FenInfo(uint64_t*, int, int, uint64_t, int, int);
	uint64_t* getPieceList();
	int getColor();
	int getCastlingRights();
	uint64_t getEnpassantSquare();
	int getHalfMoves();
	int getFullMoves();
	~FenInfo();

private:
	uint64_t piece_list[TOTAL_PIECES] = { 0 };
	int color = 0;
	int castling_rights = 0;
	uint64_t enpassant_square = 0;
	int half_moves;
	int full_moves;
};

