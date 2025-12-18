#pragma once
#include <string>
#include <vector>
#include "board.h"

using std::string;

namespace ReadUtils {
	Position read(const std::vector<string>&);
	void fillPieceList(const string&, U64(&)[COLORS][PIECES]);
	constexpr int getColor(char);
	constexpr int getPieceColor(char);
	constexpr int getPiece(char);
	constexpr int getCastlingRight(char);
	constexpr U64 getEnpassantSquare(char, char);
	constexpr int getNumber(char);
};