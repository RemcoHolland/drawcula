#pragma once

constexpr int NO_PIECE = 0;
constexpr int NO_PROMOTION = 0;
constexpr int PAWN = 0;
constexpr int KNIGHT = 1;
constexpr int BISHOP = 2;
constexpr int ROOK = 3;
constexpr int QUEEN = 4;
constexpr int KING = 5;

constexpr int PIECES = 6;

namespace Piece {

	constexpr char getPiece(const int piece) {
		switch (piece) {
		case KNIGHT: return 'N';
		case BISHOP: return 'B';
		case ROOK:   return 'R';
		case QUEEN:  return 'Q';
		case KING:   return 'K';
		default:     return '\0';
		}
	}

	constexpr char getPromotion(const int piece) {
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

	constexpr int getPromotion(const char piece) {
		switch (piece) {
		case 'n': return KNIGHT;
		case 'b': return BISHOP;
		case 'r': return ROOK;
		case 'q': return QUEEN;
		default: return NO_PROMOTION;
		}
	}
};
