#include "stringutils.h"

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

string StringUtils::moveToString(int move) {
	int from = (move & 0b111111 << 4) >> 4;
	int to = (move & (0b111111 << 10)) >> 10;
	int promotion = (move & (0b1111 << 27)) >> 27;
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

//what does this template do?
template <typename Out>
void StringUtils::split(const std::string& s, char delim, Out result) {
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> StringUtils::split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}