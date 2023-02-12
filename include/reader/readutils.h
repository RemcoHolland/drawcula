#pragma once
#include <string>
#include <vector>
#include "board.h"

using std::string;

namespace ReadUtils {
	Position read(std::vector<string>);
	void fillPieceList(string, U64(&)[COLORS][PIECES]);
	int getColor(char);
	int getPieceColor(char);
	int getPiece(char);
	int getCastlingRight(char);
	U64 getEnpassantSquare(char, char);
	int getNumber(char);
};