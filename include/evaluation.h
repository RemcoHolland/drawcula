#include "board.h"

#pragma once
class Evaluation {
public:
	Evaluation();
	int getScore(const Board&);
	~Evaluation();

private:
	const int PAWN_VALUE = 100;
	const int KNIGHT_VALUE = 300;
	const int BISHOP_VALUE = 300;
	const int ROOK_VALUE = 500;
	const int QUEEN_VALUE = 900;

	int materialScore(const Board&);
	int popCount(uint64_t);
};

