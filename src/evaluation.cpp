#include "evaluation.h"
#include "utils.h"

int popCount(U64 x) {
	x = x - ((x >> 1) & k1); /* put count of each 2 bits into those 2 bits */
	x = (x & k2) + ((x >> 2) & k2); /* put count of each 4 bits into those 4 bits */
	x = (x + (x >> 4)) & k4; /* put count of each 8 bits into those 8 bits */
	x = (x * kf) >> 56; /* returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...  */
	return (int)x;
}

int pawnPositionalScore(const U64(&piece_list)[COLORS][PIECES]) {
	int score = 0;
	U64 white_pawns = piece_list[WHITE][PAWN];
	U64 black_pawns = piece_list[BLACK][PAWN];

	while (white_pawns) {
		int square = Utils::getLS1B(white_pawns);
		score += PIECE_SQUARE[WHITE][PAWN][square];
		white_pawns &= (white_pawns - 1); // clear LSB
	}
	while (black_pawns) {
		int square = Utils::getLS1B(black_pawns);
		score -= PIECE_SQUARE[BLACK][PAWN][square];
		black_pawns &= (black_pawns - 1); // clear LSB
	}
	return score;
}

int knightPositionalScore(const U64(&piece_list)[COLORS][PIECES]) {
	int score = 0;
	U64 white_knights = piece_list[WHITE][KNIGHT];
	U64 black_knights = piece_list[BLACK][KNIGHT];

	while (white_knights) {
		int square = Utils::getLS1B(white_knights);
		score += PIECE_SQUARE[WHITE][KNIGHT][square];
		white_knights &= (white_knights - 1); // clear LSB
	}
	while (black_knights) {
		int square = Utils::getLS1B(black_knights);
		score -= PIECE_SQUARE[BLACK][KNIGHT][square];
		black_knights &= (black_knights - 1); // clear LSB
	}
	return score;
}

int bishopPositionalScore(const U64(&piece_list)[COLORS][PIECES]) {
	int score = 0;
	U64 white_bishops = piece_list[WHITE][BISHOP];
	U64 black_bishops = piece_list[BLACK][BISHOP];

	while (white_bishops) {
		int square = Utils::getLS1B(white_bishops);
		score += PIECE_SQUARE[WHITE][BISHOP][square];
		white_bishops &= (white_bishops - 1); // clear LSB
	}
	while (black_bishops) {
		int square = Utils::getLS1B(black_bishops);
		score -= PIECE_SQUARE[BLACK][BISHOP][square];
		black_bishops &= (black_bishops - 1); // clear LSB
	}
	return score;
}

int rookPositionalScore(const U64(&piece_list)[COLORS][PIECES]) {
	int score = 0;
	U64 white_rooks = piece_list[WHITE][ROOK];
	U64 black_rooks = piece_list[BLACK][ROOK];

	while (white_rooks) {
		int square = Utils::getLS1B(white_rooks);
		score += PIECE_SQUARE[WHITE][ROOK][square];
		white_rooks &= (white_rooks - 1); // clear LSB
	}
	while (black_rooks) {
		int square = Utils::getLS1B(black_rooks);
		score -= PIECE_SQUARE[BLACK][ROOK][square];
		black_rooks &= (black_rooks - 1); // clear LSB
	}
	return score;
}

int queenPositionalScore(const U64(&piece_list)[COLORS][PIECES]) {
	int score = 0;
	U64 white_queens = piece_list[WHITE][QUEEN];
	U64 black_queens = piece_list[BLACK][QUEEN];

	while (white_queens) {
		int square = Utils::getLS1B(white_queens);
		score += PIECE_SQUARE[WHITE][QUEEN][square];
		white_queens &= (white_queens - 1); // clear LSB
	}
	while (black_queens) {
		int square = Utils::getLS1B(black_queens);
		score -= PIECE_SQUARE[BLACK][QUEEN][square];
		black_queens &= (black_queens - 1); // clear LSB
	}
	return score;
}

int kingPositionalScore(const U64(&piece_list)[COLORS][PIECES]) {
	int score = 0;
	int white_king_square = Utils::getLS1B(piece_list[WHITE][KING]);
	int black_king_square = Utils::getLS1B(piece_list[BLACK][KING]);

	score += PIECE_SQUARE[WHITE][KING][white_king_square];

	score -= PIECE_SQUARE[BLACK][KING][black_king_square];

	return score;
}

int evaluation::positionalScore(const U64(&piece_list)[COLORS][PIECES]) {
	return pawnPositionalScore(piece_list) + knightPositionalScore(piece_list) + bishopPositionalScore(piece_list) + rookPositionalScore(piece_list) + queenPositionalScore(piece_list) + kingPositionalScore(piece_list);
}

int evaluation::materialScore(const U64(&piece_list)[COLORS][PIECES]) {
	return
		(popCount(piece_list[WHITE][PAWN]) - popCount(piece_list[BLACK][PAWN])) * PAWN_VALUE +
		(popCount(piece_list[WHITE][KNIGHT]) - popCount(piece_list[BLACK][KNIGHT])) * KNIGHT_VALUE +
		(popCount(piece_list[WHITE][BISHOP]) - popCount(piece_list[BLACK][BISHOP])) * BISHOP_VALUE +
		(popCount(piece_list[WHITE][ROOK]) - popCount(piece_list[BLACK][ROOK])) * ROOK_VALUE +
		(popCount(piece_list[WHITE][QUEEN]) - popCount(piece_list[BLACK][QUEEN])) * QUEEN_VALUE;
}

int evaluation::getScore(const Board& board) {
	return board.material_score + board.positional_score;
}
