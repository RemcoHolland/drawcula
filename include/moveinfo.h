#pragma once
#include <cstdint>

// temporary class for storing move information
class MoveInfo {
public:	
	int captured_piece;
	uint64_t enpassant_square;
	int castling_rights;

	MoveInfo(int, uint64_t, int);
	~MoveInfo();

private:

};

