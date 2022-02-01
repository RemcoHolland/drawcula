#include "move.h"

Move::Move() {
}

Move::Move(string input, int color) {
	Move::piece = NO_PIECE;
	Move::from = Utils::getPower(StringUtils::getSquare(input[0], input[1]));
	Move::to = Utils::getPower(StringUtils::getSquare(input[2], input[3]));
	Move::flag = Flag::NO_FLAG;
	Move::promotion = input.length() == 5 ? Move::promotion = Piece::getPromotion(input[4], color) : NO_PIECE;
}

Move::Move(uint64_t from, uint64_t to) {
	Move::piece = NO_PIECE;
	Move::from = from;
	Move::to = to;
	Move::flag = Flag::NO_FLAG;
	Move::promotion = NO_PIECE;
}

Move::Move(int piece, uint64_t from, uint64_t to, Flag flag) {
	Move::piece = piece;
	Move::from = from;
	Move::to = to;
	Move::flag = flag;
	Move::promotion = NO_PIECE;
}

Move::Move(int piece, uint64_t from, uint64_t to, Flag flag, int promotion) {
	Move::piece = piece;
	Move::from = from;
	Move::to = to;
	Move::flag = flag;
	Move::promotion = promotion;
}

bool Move::operator==(const Move other) {
	return from == other.from && to == other.to && promotion == other.promotion;
}

string Move::toString() {
	int square_from = Utils::getLS1B(from);
	int square_to = Utils::getLS1B(to);
	char promotion_char = Piece::getPromotion(promotion);

	string moveStr("");
	moveStr += StringUtils::getFile(square_from);
	moveStr += StringUtils::getRank(square_from);
	moveStr += StringUtils::getFile(square_to);
	moveStr += StringUtils::getRank(square_to);
	if (promotion_char != 0) {
		moveStr += promotion_char;
	}

	return moveStr;
}

int Move::getPiece() {
	return piece;
}

uint64_t Move::getFrom() {
	return from;
}

uint64_t Move::getTo() {
	return to;
}

Flag Move::getFlag() {
	return flag;
}

int Move::getPromotion() {
	return promotion;
}

Move::~Move() {
}
