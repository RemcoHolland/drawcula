#include "evaluation.h"
#include "piece.h"

Evaluation::Evaluation() {
}

int Evaluation::getScore(const Board& board) {
	return materialScore(board);
}

int Evaluation::materialScore(const Board& board) {
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

int Evaluation::popCount(uint64_t pieces) {
	int count = 0;
	while (pieces) {
		count++;
		pieces &= pieces - 1; // reset LS1B
	}
	return count;
}

Evaluation::~Evaluation() {
}

