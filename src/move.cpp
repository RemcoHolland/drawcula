#include "move.h"


string Move::moveToString(int move) {
	int from = (move & (0x3F << 6)) >> 6;
	int to = (move & (0x3F << 12)) >> 12;
	int promotion = (move & (0x3F << 24)) >> 24;
	char promotion_char = Piece::getPromotion((move & (0x3F << 24)) >> 24);

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