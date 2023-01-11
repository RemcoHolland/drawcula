#include "piece.h"

Piece::Piece() {
}

char Piece::getPromotion(int piece) {
	char promotion = '\0';
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

int Piece::getPromotion(char piece, int color) {
	switch (piece) {
	case 'n': return KNIGHT;
	case 'b': return BISHOP;
	case 'r': return ROOK;
	case 'q': return QUEEN;
	default: return NO_PROMOTION;
	}
}

Piece::~Piece() {
}
