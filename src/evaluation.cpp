#include "evaluation.h"

constexpr int PAWN_VALUE = 100;
constexpr int KNIGHT_VALUE = 300;
constexpr int BISHOP_VALUE = 300;
constexpr int ROOK_VALUE = 500;
constexpr int QUEEN_VALUE = 900;

constexpr U64 k1 = U64(0x5555555555555555); /*  -1/3   */
constexpr U64 k2 = U64(0x3333333333333333); /*  -1/5   */
constexpr U64 k4 = U64(0x0f0f0f0f0f0f0f0f); /*  -1/17  */
constexpr U64 kf = U64(0x0101010101010101); /*  -1/255 */

int popCount(U64 x) {
	x = x - ((x >> 1) & k1); /* put count of each 2 bits into those 2 bits */
	x = (x & k2) + ((x >> 2) & k2); /* put count of each 4 bits into those 4 bits */
	x = (x + (x >> 4)) & k4; /* put count of each 8 bits into those 8 bits */
	x = (x * kf) >> 56; /* returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...  */
	return (int)x;
}

int materialScore(const Board& board) {
	return
		popCount(board.piece_list[WHITE][PAWN]) * PAWN_VALUE +
		popCount(board.piece_list[WHITE][KNIGHT]) * KNIGHT_VALUE +
		popCount(board.piece_list[WHITE][BISHOP]) * BISHOP_VALUE +
		popCount(board.piece_list[WHITE][ROOK]) * ROOK_VALUE +
		popCount(board.piece_list[WHITE][QUEEN]) * QUEEN_VALUE -
		popCount(board.piece_list[BLACK][PAWN]) * PAWN_VALUE -
		popCount(board.piece_list[BLACK][KNIGHT]) * KNIGHT_VALUE -
		popCount(board.piece_list[BLACK][BISHOP]) * BISHOP_VALUE -
		popCount(board.piece_list[BLACK][ROOK]) * ROOK_VALUE -
		popCount(board.piece_list[BLACK][QUEEN]) * QUEEN_VALUE;
}

int evaluation::getScore(const Board& board) {
	return materialScore(board);
}
