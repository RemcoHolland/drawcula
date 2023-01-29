#include "evaluation.h"
#include "utils.h"

int popCount(U64 x) {
	x = x - ((x >> 1) & k1); /* put count of each 2 bits into those 2 bits */
	x = (x & k2) + ((x >> 2) & k2); /* put count of each 4 bits into those 4 bits */
	x = (x + (x >> 4)) & k4; /* put count of each 8 bits into those 8 bits */
	x = (x * kf) >> 56; /* returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...  */
	return (int)x;
}

int materialScore(const Board& board) {
	return
		(popCount(board.piece_list[WHITE][PAWN]) - popCount(board.piece_list[BLACK][PAWN])) * PAWN_VALUE +
		(popCount(board.piece_list[WHITE][KNIGHT]) - popCount(board.piece_list[BLACK][KNIGHT])) * KNIGHT_VALUE +
		(popCount(board.piece_list[WHITE][BISHOP]) - popCount(board.piece_list[BLACK][BISHOP])) * BISHOP_VALUE +
		(popCount(board.piece_list[WHITE][ROOK]) - popCount(board.piece_list[BLACK][ROOK])) * ROOK_VALUE +
		(popCount(board.piece_list[WHITE][QUEEN]) - popCount(board.piece_list[BLACK][QUEEN])) * QUEEN_VALUE;
}

int pawnPositionalScore(const Board& board) {
	int score = 0;
	U64 white_pawns = board.piece_list[WHITE][PAWN];
	U64 black_pawns = board.piece_list[BLACK][PAWN];

	while (white_pawns) {
		int square = Utils::getLS1B(white_pawns);
		score += WHITE_PAWN_PSQT[square];
		white_pawns &= (white_pawns - 1); // clear LSB
	}
	while (black_pawns) {
		int square = Utils::getLS1B(black_pawns);
		score -= BLACK_PAWN_PSQT[square];
		black_pawns &= (black_pawns - 1); // clear LSB
	}
	return score;
}

int knightPositionalScore(const Board& board) {
	int score = 0;
	U64 white_knights = board.piece_list[WHITE][KNIGHT];
	U64 black_knights = board.piece_list[BLACK][KNIGHT];

	while (white_knights) {
		int square = Utils::getLS1B(white_knights);
		score += WHITE_KNIGHT_PSQT[square];
		white_knights &= (white_knights - 1); // clear LSB
	}
	while (black_knights) {
		int square = Utils::getLS1B(black_knights);
		score -= BLACK_KNIGHT_PSQT[square];
		black_knights &= (black_knights - 1); // clear LSB
	}
	return score;
}

int bishopPositionalScore(const Board& board) {
	int score = 0;
	U64 white_bishops = board.piece_list[WHITE][BISHOP];
	U64 black_bishops = board.piece_list[BLACK][BISHOP];

	while (white_bishops) {
		int square = Utils::getLS1B(white_bishops);
		score += WHITE_BISHOP_PSQT[square];
		white_bishops &= (white_bishops - 1); // clear LSB
	}
	while (black_bishops) {
		int square = Utils::getLS1B(black_bishops);
		score -= BLACK_BISHOP_PSQT[square];
		black_bishops &= (black_bishops - 1); // clear LSB
	}
	return score;
}

int rookPositionalScore(const Board& board) {
	int score = 0;
	U64 white_rooks = board.piece_list[WHITE][ROOK];
	U64 black_rooks = board.piece_list[BLACK][ROOK];

	while (white_rooks) {
		int square = Utils::getLS1B(white_rooks);
		score += WHITE_ROOK_PSQT[square];
		white_rooks &= (white_rooks - 1); // clear LSB
	}
	while (black_rooks) {
		int square = Utils::getLS1B(black_rooks);
		score -= BLACK_ROOK_PSQT[square];
		black_rooks &= (black_rooks - 1); // clear LSB
	}
	return score;
}

int queenPositionalScore(const Board& board) {
	int score = 0;
	U64 white_queens = board.piece_list[WHITE][QUEEN];
	U64 black_queens = board.piece_list[BLACK][QUEEN];

	while (white_queens) {
		int square = Utils::getLS1B(white_queens);
		score += WHITE_QUEEN_PSQT[square];
		white_queens &= (white_queens - 1); // clear LSB
	}
	while (black_queens) {
		int square = Utils::getLS1B(black_queens);
		score -= BLACK_QUEEN_PSQT[square];
		black_queens &= (black_queens - 1); // clear LSB
	}
	return score;
}

int kingPositionalScore(const Board& board) {
	int score = 0;
	U64 white_king = board.piece_list[WHITE][KING];
	U64 black_king = board.piece_list[BLACK][KING];

	score += WHITE_KING_PSQT[Utils::getLS1B(white_king)];

	score -= BLACK_KING_PSQT[Utils::getLS1B(black_king)];

	return score;
}

int positionalScore(const Board& board) {
	return pawnPositionalScore(board) + knightPositionalScore(board) + bishopPositionalScore(board) + rookPositionalScore(board) + queenPositionalScore(board) + kingPositionalScore(board);
}

int evaluation::getScore(const Board& board) {
	return materialScore(board) + positionalScore(board);
}
