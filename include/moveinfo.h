#pragma once
#include <cstdint>

// temporary class for storing move information
class MoveInfo {
public:	
	int captured_piece;
	int enpassant_square;
	int castling_rights;

	MoveInfo(int, int, int);
	~MoveInfo();

private:

};

