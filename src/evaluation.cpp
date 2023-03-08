#include "evaluation.h"
#include "utils.h"

int popCount(U64 x) {
	x = x - ((x >> 1) & k1); /* put count of each 2 bits into those 2 bits */
	x = (x & k2) + ((x >> 2) & k2); /* put count of each 4 bits into those 4 bits */
	x = (x + (x >> 4)) & k4; /* put count of each 8 bits into those 8 bits */
	x = (x * kf) >> 56; /* returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...  */
	return (int)x;
}

int evaluation::positionalScore(const U64(&piece_list)[COLORS][PIECES]) {
	int score = 0;
	for (int color = WHITE; color < COLORS; color++) {
		for (int piece = PAWN; piece < PIECES; piece++) {
			U64 pieces = piece_list[color][piece];
			while (pieces) {
				int square = Utils::getLS1B(pieces);
				score += (-color | 1) * PIECE_SQUARE[color][piece][square];
				pieces &= (pieces - 1); // clear LSB
			}
		}
	}
	return score;
}

int evaluation::materialScore(const U64(&piece_list)[COLORS][PIECES]) {
	return
		(popCount(piece_list[WHITE][PAWN]) - popCount(piece_list[BLACK][PAWN])) * PAWN_VALUE +
		(popCount(piece_list[WHITE][KNIGHT]) - popCount(piece_list[BLACK][KNIGHT])) * KNIGHT_VALUE +
		(popCount(piece_list[WHITE][BISHOP]) - popCount(piece_list[BLACK][BISHOP])) * BISHOP_VALUE +
		(popCount(piece_list[WHITE][ROOK]) - popCount(piece_list[BLACK][ROOK])) * ROOK_VALUE +
		(popCount(piece_list[WHITE][QUEEN]) - popCount(piece_list[BLACK][QUEEN])) * QUEEN_VALUE;
}
