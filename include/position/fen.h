#pragma once
#include "position/position.h"
#include "piece.h"
#include "color.h"

class Fen : public Position {
public:
	Fen(U64(&)[COLORS][PIECES], int, int, U64, int, int);
	int half_moves = 0;
	int full_moves = 0;
	~Fen();
};