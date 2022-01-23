#include "consoleutils.h"

ConsoleUtils::ConsoleUtils() {
}

char ConsoleUtils::getPromotion(int piece) {
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

int ConsoleUtils::getPromotion(char piece, int color) {
	// TODO: remove color for this promotion check. Only use KNIGHT, BISHOP, etc...
	switch (piece) {
	case 'n': return color == WHITE ? WHITE_KNIGHT : BLACK_KNIGHT;
	case 'b': return color == WHITE ? WHITE_BISHOP : BLACK_BISHOP;
	case 'r': return color == WHITE ? WHITE_ROOK : BLACK_ROOK;
	case 'q': return color == WHITE ? WHITE_QUEEN : BLACK_QUEEN;
	default: return NO_PIECE;
	}
}

int ConsoleUtils::getSquare(char file, char rank) {
	int square = (file - 97) + (rank - 49) * 8;	// 97 and 49 are ascii codes
	return square;
}

char ConsoleUtils::getRank(int square) {
	return (int)(square / 8) + 49;
}

char ConsoleUtils::getFile(int square) {
	return (int)(square % 8) + 97;;
}

ConsoleUtils::~ConsoleUtils() {
}
