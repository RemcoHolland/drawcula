#include <sstream>
#include <iterator>
#include "stringutils.h"
#include "piece.h"
#include "board.h"
#include "flag.h"

const string whiteSpaces(" \f\n\r\t\v");

int StringUtils::getSquare(char file, char rank) {
	int square = (file - 97) + (rank - 49) * 8;	// 97 and 49 are ascii codes
	return square;
}

char StringUtils::getRank(int square) {
	return (int)(square / 8) + 49;
}

char StringUtils::getFile(int square) {
	return (int)(square % 8) + 97;;
}

// Move to string in Algebraic Notation (AN), e.g. e2e4
string StringUtils::moveToStringAN(int move) {
	int from = (move & FROM_MASK) >> 6;
	int to = (move & TO_MASK) >> 12;
	int promotion = (move & PROMOTION_MASK) >> 27;
	char promotion_char = Piece::getPromotion(promotion);

	string moveStr("");
	moveStr += StringUtils::getFile(from);
	moveStr += StringUtils::getRank(from);
	moveStr += StringUtils::getFile(to);
	moveStr += StringUtils::getRank(to);
	if (promotion_char != 0) {
		moveStr += promotion_char;
	}

	return moveStr;
}

// Move to string in Long Algebraic Notation (LAN), e.g. Nxf6+
// TODO: Also account for moves like Ree5 or R3e5
string StringUtils::moveToStringLAN(int move) {
	int from = (move & FROM_MASK) >> 6;
	int to = (move & TO_MASK) >> 12;
	int piece = (move & PIECE_MASK) >> 18;
	int flag = move & FLAG_MASK;
	int promotion = (move & PROMOTION_MASK) >> 27;
	char promotion_char = Piece::getPromotion(promotion);

	string moveStr("");
	if (piece != PAWN) {
		moveStr += Piece::getPiece(piece);
	}
	if (flag == CAPTURE) {
		moveStr += 'x';
	}
	moveStr += StringUtils::getFile(to);
	moveStr += StringUtils::getRank(to);
	if (promotion_char != 0) {
		moveStr += promotion_char;
	}
	return moveStr;
}

//what does this template do?
template <typename Out>
void StringUtils::split(const string& s, char delim, Out result) {
	std::istringstream iss(s);
	string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> StringUtils::split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

int StringUtils::nthOccurrence(const string& str, const string& findMe, int nth) {
	size_t  pos = 0;
	int     cnt = 0;

	while (cnt != nth) {
		pos += 1;
		pos = str.find(findMe, pos);
		if (pos == std::string::npos)
			return -1;
		cnt++;
	}
	return pos;
}