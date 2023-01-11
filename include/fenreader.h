#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include "board.h"
#include "piece.h"
#include "position.h"
#include "color.h"
#include "castling.h"
#include "stringutils.h"

using std::string;
using StringUtils::getSquare;
using StringUtils::split;


class FenReader {
public:
	FenReader();
	static Position read(string fen);
	~FenReader();

private:
	static void fillPieceList(string, uint64_t(&)[COLORS][PIECES]);
	static int getColor(char);
	static int getPieceColor(char);
	static int getPiece(char);
	static int getCastlingRight(char);
	static int getEnpassantSquare(char, char);
	static int getNumber(char);
};

