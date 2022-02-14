#include "evaluation.h"

constexpr int PAWN_VALUE = 100;
constexpr int KNIGHT_VALUE = 300;
constexpr int BISHOP_VALUE = 300;
constexpr int ROOK_VALUE = 500;
constexpr int QUEEN_VALUE = 900;

int popCount(uint64_t pieces) {
	int count = 0;
	while (pieces) {
		count++;
		pieces &= pieces - 1; // reset LS1B
	}
	return count;
}

int materialScore(const Board& board) {
	return
		popCount(board.piece_list[WHITE_PAWN]) * PAWN_VALUE +
		popCount(board.piece_list[WHITE_KNIGHT]) * KNIGHT_VALUE +
		popCount(board.piece_list[WHITE_BISHOP]) * BISHOP_VALUE +
		popCount(board.piece_list[WHITE_ROOK]) * ROOK_VALUE +
		popCount(board.piece_list[WHITE_QUEEN]) * QUEEN_VALUE -
		popCount(board.piece_list[BLACK_PAWN]) * PAWN_VALUE -
		popCount(board.piece_list[BLACK_KNIGHT]) * KNIGHT_VALUE -
		popCount(board.piece_list[BLACK_BISHOP]) * BISHOP_VALUE -
		popCount(board.piece_list[BLACK_ROOK]) * ROOK_VALUE -
		popCount(board.piece_list[BLACK_QUEEN]) * QUEEN_VALUE;
}

int evaluation::getScore(const Board& board) {
	return materialScore(board);
}
