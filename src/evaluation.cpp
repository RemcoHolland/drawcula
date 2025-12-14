#include "evaluation.h"
#include "utils.h"

int PIECE_SQUARE[COLORS][PIECES][SQUARES] = { {{{}}} };

void evaluation::initPieceSquareTable()
{
	for (int color = 0; color < COLORS; color++) {
		for (int piece = 0; piece < PIECES; piece++) {
			for (int square = 0; square < SQUARES; square++) {
				if (color == WHITE) {
					PIECE_SQUARE[color][piece][square] =
						piece == PAWN ? PAWN_PSQT[FLIP[square]] :
						piece == KNIGHT ? KNIGHT_PSQT[FLIP[square]] :
						piece == BISHOP ? BISHOP_PSQT[FLIP[square]] :
						piece == ROOK ? ROOK_PSQT[FLIP[square]] :
						piece == QUEEN ? QUEEN_PSQT[FLIP[square]] :
						KING_PSQT[FLIP[square]];
				} else {
					PIECE_SQUARE[color][piece][square] =
						piece == PAWN ? PAWN_PSQT[square] :
						piece == KNIGHT ? KNIGHT_PSQT[square] :
						piece == BISHOP ? BISHOP_PSQT[square] :
						piece == ROOK ? ROOK_PSQT[square] :
						piece == QUEEN ? QUEEN_PSQT[square] :
						KING_PSQT[square];
				}
			}
		}
	}
}

int evaluation::positionalScore(const U64(&piece_list)[COLORS][PIECES]) {
	int score = 0;
	for (int color = WHITE; color < COLORS; color++) {
		for (int piece = PAWN; piece < PIECES; piece++) {
			U64 pieces = piece_list[color][piece];
			while (pieces) {
				const int square = Utils::getLS1B(pieces);
				score += (-color | 1) * PIECE_SQUARE[color][piece][square];
				pieces &= pieces - 1; // clear LSB
			}
		}
	}
	return score;
}
