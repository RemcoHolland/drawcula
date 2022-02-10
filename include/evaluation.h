#include "board.h"

constexpr int PAWN_VALUE = 100;
constexpr int KNIGHT_VALUE = 300;
constexpr int BISHOP_VALUE = 300;
constexpr int ROOK_VALUE = 500;
constexpr int QUEEN_VALUE = 900;

#pragma once
class Evaluation {
public:
	Evaluation();
	int getScore(const Board&);
	~Evaluation();

private:
	int materialScore(const Board&);
	int popCount(uint64_t);
};

