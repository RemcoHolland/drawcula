#pragma once
#include "color.h"

constexpr int NO_PIECE = 0;
constexpr int NO_PROMOTION = 0;
constexpr int WHITE_PAWN = 0;
constexpr int WHITE_KNIGHT = 1;
constexpr int WHITE_BISHOP = 2;
constexpr int WHITE_ROOK = 3;
constexpr int WHITE_QUEEN = 4;
constexpr int WHITE_KING = 5;
constexpr int BLACK_PAWN = 6;
constexpr int BLACK_KNIGHT = 7;
constexpr int BLACK_BISHOP = 8;
constexpr int BLACK_ROOK = 9;
constexpr int BLACK_QUEEN = 10;
constexpr int BLACK_KING = 11;

constexpr int NR_PIECES = 6;
constexpr int TOTAL_PIECES = 12;

class Piece {
public:
	Piece();
	static char getPromotion(int);
	static int getPromotion(char, int);
	~Piece();
};
