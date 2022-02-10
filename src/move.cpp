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



//string Move::toString() {
	//char promotion_char = Piece::getPromotion(promotion);

	//string moveStr("");
	//moveStr += StringUtils::getFile(from);
	//moveStr += StringUtils::getRank(from);
	//moveStr += StringUtils::getFile(to);
	//moveStr += StringUtils::getRank(to);
	//if (promotion_char != 0) {
	//	moveStr += promotion_char;
	//}

	//return moveStr;
//	return "bla";
//}

//int Move::getPiece() {
//	return move & 0x3F;
//}
//int Move::getFrom() {
//	return (move & (0x3F << 6)) >> 6;
//}
//
//int Move::getTo() {
//	return (move & (0x3F << 12)) >> 12;
//}
//
//int Move::getFlag() {
//	return (move & (0x3F << 18)) >> 18;
//}
//
//int Move::getPromotion() {
//	return (move & (0x3F << 24)) >> 24;
//}

