#include "piece.h"

Piece::Piece() = default;

char Piece::getPiece(const int piece) {
	switch (piece) {
	case KNIGHT:
		return 'N';
	case BISHOP:
		return 'B';
	case ROOK:
		return 'R';
	case QUEEN:
		return 'Q';
	case KING:
		return 'K';
	default: return '\0';
	}
}

char Piece::getPromotion(const int piece) {
	switch (piece) {
	case KNIGHT:
		return 'n';
	case BISHOP:
		return 'b';
	case ROOK:
		return 'r';
	case QUEEN:
		return 'q';
	default: return '\0';
	}
}

int Piece::getPromotion(const char piece) {
	switch (piece) {
	case 'n': return KNIGHT;
	case 'b': return BISHOP;
	case 'r': return ROOK;
	case 'q': return QUEEN;
	default: return NO_PROMOTION;
	}
}

Piece::~Piece() = default;
