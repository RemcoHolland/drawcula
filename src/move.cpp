#include "move.h"

Move::Move(const string& input, int color) {
	Move::piece = NO_PIECE;
	Move::from = StringUtils::getSquare(input[0], input[1]);
	Move::to = StringUtils::getSquare(input[2], input[3]);
	Move::flag = Flag::NO_FLAG;
	Move::promotion = input.length() == 5 ? Move::promotion = Piece::getPromotion(input[4], color) : NO_PIECE;
}

Move::Move(int from, int to) {
	Move::piece = NO_PIECE;
	Move::from = from;
	Move::to = to;
	Move::flag = Flag::NO_FLAG;
	Move::promotion = NO_PIECE;
}

Move::Move(int piece, int from, int to, Flag flag) {
	Move::piece = piece;
	Move::from = from;
	Move::to = to;
	Move::flag = flag;
	Move::promotion = NO_PIECE;
}

Move::Move(int piece, int from, int to, Flag flag, int promotion) {
	Move::piece = piece;
	Move::from = from;
	Move::to = to;
	Move::flag = flag;
	Move::promotion = promotion;
}

bool Move::operator==(const Move& other) {
	return from == other.from && to == other.to && promotion == other.promotion;
}

string Move::toString() {
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

Move::~Move() {
}
