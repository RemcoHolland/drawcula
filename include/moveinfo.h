#pragma once
#include <cstdint>

// temporary class for storing move information
class MoveInfo {
public:
	MoveInfo(int, uint64_t, int);
	int getCapturedPiece();
	uint64_t getEnpassantSquare();
	int getCastlingRights();
	~MoveInfo();

private:
	int captured_piece;
	uint64_t enpassant_square;	
	int castling_rights;
};

