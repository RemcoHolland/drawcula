#pragma once
#include "color.h"

const int NO_PIECE = -1;
const int WHITE_PAWN = 0;
const int WHITE_KNIGHT = 1;
const int WHITE_BISHOP = 2;
const int WHITE_ROOK = 3;
const int WHITE_QUEEN = 4;
const int WHITE_KING = 5;
const int BLACK_PAWN = 6;
const int BLACK_KNIGHT = 7;
const int BLACK_BISHOP = 8;
const int BLACK_ROOK = 9;
const int BLACK_QUEEN = 10;
const int BLACK_KING = 11;

const int NR_PIECES = 6;
const int TOTAL_PIECES = 12;

class Piece {
public:
	Piece();
	static char getPromotion(int);
	static int getPromotion(char, int);
	~Piece();
};
