#pragma once

constexpr int NO_PIECE = 0;
constexpr int NO_PROMOTION = 0;
constexpr int PAWN = 0;
constexpr int KNIGHT = 1;
constexpr int BISHOP = 2;
constexpr int ROOK = 3;
constexpr int QUEEN = 4;
constexpr int KING = 5;

constexpr int PIECES = 6;

class Piece {
public:
	Piece();
	static char getPiece(int);
	static char getPromotion(int);
	static int getPromotion(char);
	~Piece();
};
