#include "piece.h"

Piece::Piece() {
}

char Piece::getPromotion(int piece) {
	char promotion = '\0';
	switch (piece) {
	case WHITE_KNIGHT:
	case BLACK_KNIGHT:
		return 'n';
	case WHITE_BISHOP:
	case BLACK_BISHOP:
		return 'b';
	case WHITE_ROOK:
	case BLACK_ROOK:
		return 'r';
	case WHITE_QUEEN:
	case BLACK_QUEEN:
		return 'q';
	default: return '\0';
	}
}

int Piece::getPromotion(char piece, int color) {
	// TODO: remove color for this promotion check. Only use KNIGHT, BISHOP, etc...
	switch (piece) {
	case 'n': return WHITE_KNIGHT + color * NR_PIECES;
	case 'b': return WHITE_BISHOP + color * NR_PIECES;
	case 'r': return WHITE_ROOK + color * NR_PIECES;
	case 'q': return WHITE_QUEEN + color * NR_PIECES;
	default: return NO_PIECE;
	}
}

Piece::~Piece() {
}
