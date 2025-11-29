#pragma warning( disable : 6385)
#pragma warning( disable : 6386)
#include <string>
#include "board.h"
#include "evaluation.h"
#include "flag.h"
#include "piece.h"
#include "castling.h"
#include "zobrist.h"

using std::string;

Board::Board(const Position& position) {
	init(position);
}

void Board::setPosition(const Position& position) {
	init(position);
}

int Board::makeMove(const int color, const int move)
{
	const int from_nr = (move & FROM_MASK) >> 6;
	const int to_nr = (move & TO_MASK) >> 12;
	const U64 from = A1 << from_nr;
	const U64 to = A1 << to_nr;

	const int piece = (move & PIECE_MASK) >> 18;
	const int flag = move & FLAG_MASK;
	const int captured_piece = (move & CAPTURED_PIECE_MASK) >> 24;

	const U64 fromTo = from ^ to;
	colorBB[color] ^= fromTo;
	piece_list[color][piece] ^= fromTo;
	enpassant_square = 0;    // clear previous enpassant square

	if (flag == NO_FLAG) {
		occupiedBB ^= fromTo;
		positional_score += (-color | 1) * (PIECE_SQUARE[color][piece][to_nr] - PIECE_SQUARE[color][piece][from_nr]);
		zobrist_key ^= ZOBRIST[color][piece][from_nr] ^ ZOBRIST[color][piece][to_nr];
	} else if (flag == DOUBLE_PUSH) {
		occupiedBB ^= fromTo;
		enpassant_square = color == WHITE ? to >> 8 : to << 8;
		positional_score += (-color | 1) * (PIECE_SQUARE[color][PAWN][to_nr] - PIECE_SQUARE[color][PAWN][from_nr]);
		zobrist_key ^= ZOBRIST[color][PAWN][from_nr] ^ ZOBRIST[color][PAWN][to_nr];
	} else if (flag == CAPTURE) {
		occupiedBB ^= from;
		colorBB[color ^ 1] ^= to;
		piece_list[color ^ 1][captured_piece] ^= to;
		material_score += (-color | 1) * MATERIAL_VALUE[captured_piece];
		positional_score += (-color | 1) * (PIECE_SQUARE[color][piece][to_nr] - PIECE_SQUARE[color][piece][from_nr] + PIECE_SQUARE[color ^ 1][captured_piece][to_nr]);
		zobrist_key ^= ZOBRIST[color][piece][from_nr] ^ ZOBRIST[color][piece][to_nr] ^ ZOBRIST[color ^ 1][captured_piece][to_nr];
	} else if (flag == EN_PASSANT) {
		const int capture_square_nr = color == WHITE ? to_nr - 8 : to_nr + 8;
		const U64 capture_square = A1 << capture_square_nr;
		occupiedBB ^= fromTo | capture_square;
		colorBB[color ^ 1] ^= capture_square;
		piece_list[color ^ 1][PAWN] ^= capture_square;
		material_score += (-color | 1) * MATERIAL_VALUE[PAWN];
		positional_score += (-color | 1) * (PIECE_SQUARE[color][PAWN][to_nr] - PIECE_SQUARE[color][PAWN][from_nr] + PIECE_SQUARE[color ^ 1][PAWN][capture_square_nr]);
		zobrist_key ^= ZOBRIST[color][PAWN][from_nr] ^ ZOBRIST[color][PAWN][to_nr] ^ ZOBRIST[color ^ 1][PAWN][capture_square_nr];
	} else if (flag == CASTLING) {
		U64 rook_shift;
		int rook_start_square_nr;
		int rook_end_square_nr;
		if (from < to) { // castling short			
			rook_shift = to << 1 ^ from << 1;
			rook_start_square_nr = to_nr + 1;
			rook_end_square_nr = from_nr + 1;
		} else { // castling long
			rook_shift = from >> 1 ^ to >> 2;
			rook_start_square_nr = to_nr - 2;
			rook_end_square_nr = from_nr - 1;
		}
		occupiedBB ^= fromTo | rook_shift;
		colorBB[color] ^= rook_shift;
		piece_list[color][ROOK] ^= rook_shift;
		positional_score += (-color | 1) * (PIECE_SQUARE[color][KING][to_nr] - PIECE_SQUARE[color][KING][from_nr]
			+ PIECE_SQUARE[color][ROOK][rook_end_square_nr] - PIECE_SQUARE[color][ROOK][rook_start_square_nr]);
		zobrist_key ^= ZOBRIST[color][KING][from_nr] ^ ZOBRIST[color][KING][to_nr] ^ ZOBRIST[color][ROOK][rook_start_square_nr] ^ ZOBRIST[color][ROOK][rook_end_square_nr];
	} else if (flag == PROMOTION) {
		const int promoted_piece = (move & PROMOTION_MASK) >> 27;
		occupiedBB ^= fromTo;
		piece_list[color][PAWN] ^= to;
		piece_list[color][promoted_piece] ^= to;
		material_score += (-color | 1) * (MATERIAL_VALUE[promoted_piece] - MATERIAL_VALUE[PAWN]);
		positional_score += (-color | 1) * (PIECE_SQUARE[color][promoted_piece][to_nr] - PIECE_SQUARE[color][PAWN][from_nr]);
		zobrist_key ^= ZOBRIST[color][PAWN][from_nr] ^ ZOBRIST[color][promoted_piece][to_nr];
	} else if (flag == PROMOCAPT) {
		const int promoted_piece = (move & PROMOTION_MASK) >> 27;
		occupiedBB ^= from;
		colorBB[color ^ 1] ^= to;
		piece_list[color][PAWN] ^= to;
		piece_list[color ^ 1][captured_piece] ^= to;
		piece_list[color][promoted_piece] ^= to;
		material_score += (-color | 1) * (MATERIAL_VALUE[promoted_piece] + MATERIAL_VALUE[captured_piece] - MATERIAL_VALUE[PAWN]);
		positional_score += (-color | 1) * (PIECE_SQUARE[color][promoted_piece][to_nr] - PIECE_SQUARE[color][PAWN][from_nr] + PIECE_SQUARE[color ^ 1][captured_piece][to_nr]);
		zobrist_key ^= ZOBRIST[color][PAWN][from_nr] ^ ZOBRIST[color][promoted_piece][to_nr] ^ ZOBRIST[color ^ 1][captured_piece][to_nr];
	}
	const int unmake_info = captured_piece | castling_rights << 3;
	setCastlingRights(color, piece, flag, from, to);
	zobrist_key ^= color == BLACK ? BLACK_TO_MOVE : 0;
	return unmake_info;
}

void Board::unmakeMove(const int color, const int move, const int unmake_info) {
	const int from_nr = (move & FROM_MASK) >> 6;
	const int to_nr = (move & TO_MASK) >> 12;
	const U64 from = A1 << from_nr;
	const U64 to = A1 << to_nr;

	const int piece = (move & PIECE_MASK) >> 18;
	const int flag = move & FLAG_MASK;
	const U64 fromTo = from ^ to;
	colorBB[color] ^= fromTo;
	piece_list[color][piece] ^= fromTo;
	castling_rights = (unmake_info & CASTLING_MASK) >> 3;

	if (flag == NO_FLAG) {
		occupiedBB ^= fromTo;
		positional_score -= (-color | 1) * (PIECE_SQUARE[color][piece][to_nr] - PIECE_SQUARE[color][piece][from_nr]);
		zobrist_key ^= ZOBRIST[color][piece][from_nr] ^ ZOBRIST[color][piece][to_nr];
	} else if (flag == DOUBLE_PUSH) {
		occupiedBB ^= fromTo;
		enpassant_square = 0;
		positional_score -= (-color | 1) * (PIECE_SQUARE[color][piece][to_nr] - PIECE_SQUARE[color][piece][from_nr]);
		zobrist_key ^= ZOBRIST[color][piece][from_nr] ^ ZOBRIST[color][piece][to_nr];
	} else if (flag == CAPTURE) {
		const int captured_piece = unmake_info & CAPTURE_MASK;
		occupiedBB ^= from;
		colorBB[color ^ 1] ^= to;
		piece_list[color ^ 1][captured_piece] ^= to;
		material_score -= (-color | 1) * MATERIAL_VALUE[captured_piece];
		positional_score -= (-color | 1) * (PIECE_SQUARE[color][piece][to_nr] - PIECE_SQUARE[color][piece][from_nr] + PIECE_SQUARE[color ^ 1][captured_piece][to_nr]);
		zobrist_key ^= ZOBRIST[color][piece][from_nr] ^ ZOBRIST[color][piece][to_nr] ^ ZOBRIST[color ^ 1][captured_piece][to_nr];
	} else if (flag == EN_PASSANT) {
		const int capture_square_nr = color == WHITE ? to_nr - 8 : to_nr + 8;
		const U64 capture_square = A1 << capture_square_nr;
		occupiedBB ^= capture_square | fromTo;
		colorBB[color ^ 1] ^= capture_square;
		piece_list[color ^ 1][PAWN] ^= capture_square;
		material_score -= (-color | 1) * MATERIAL_VALUE[PAWN];
		positional_score -= (-color | 1) * (PIECE_SQUARE[color][PAWN][to_nr] - PIECE_SQUARE[color][PAWN][from_nr] + PIECE_SQUARE[color ^ 1][PAWN][capture_square_nr]);
		zobrist_key ^= ZOBRIST[color][PAWN][from_nr] ^ ZOBRIST[color][PAWN][to_nr] ^ ZOBRIST[color ^ 1][PAWN][capture_square_nr];
	} else if (flag == CASTLING) {
		U64 rook_shift;
		int rook_start_square_nr;
		int rook_end_square_nr;
		if (from < to) { // castling short			
			rook_shift = to << 1 ^ from << 1;
			rook_start_square_nr = to_nr + 1;
			rook_end_square_nr = from_nr + 1;
		} else { // castling long
			rook_shift = from >> 1 ^ to >> 2;
			rook_start_square_nr = to_nr - 2;
			rook_end_square_nr = from_nr - 1;
		}
		occupiedBB ^= fromTo | rook_shift;
		colorBB[color] ^= rook_shift;
		piece_list[color][ROOK] ^= rook_shift;
		positional_score -= (-color | 1) * (PIECE_SQUARE[color][KING][to_nr] - PIECE_SQUARE[color][KING][from_nr]
			+ PIECE_SQUARE[color][ROOK][rook_end_square_nr] - PIECE_SQUARE[color][ROOK][rook_start_square_nr]);
		zobrist_key ^= ZOBRIST[color][KING][from_nr] ^ ZOBRIST[color][KING][to_nr] ^ ZOBRIST[color][ROOK][rook_start_square_nr] ^ ZOBRIST[color][ROOK][rook_end_square_nr];
	} else if (flag == PROMOTION) {
		const int promoted_piece = (move & PROMOTION_MASK) >> 27;
		occupiedBB ^= fromTo;
		piece_list[color][PAWN] ^= to;
		piece_list[color][promoted_piece] ^= to;
		material_score -= (-color | 1) * (MATERIAL_VALUE[promoted_piece] - MATERIAL_VALUE[PAWN]);
		positional_score -= (-color | 1) * (PIECE_SQUARE[color][promoted_piece][to_nr] - PIECE_SQUARE[color][PAWN][from_nr]);
		zobrist_key ^= ZOBRIST[color][PAWN][from_nr] ^ ZOBRIST[color][promoted_piece][to_nr];
	} else if (flag == PROMOCAPT) {
		const int promoted_piece = (move & PROMOTION_MASK) >> 27;
		const int captured_piece = unmake_info & CAPTURE_MASK;
		occupiedBB ^= from;
		colorBB[color ^ 1] ^= to;
		piece_list[color][PAWN] ^= to;
		piece_list[color ^ 1][captured_piece] ^= to;
		piece_list[color][promoted_piece] ^= to;
		material_score -= (-color | 1) * (MATERIAL_VALUE[promoted_piece] + MATERIAL_VALUE[captured_piece] - MATERIAL_VALUE[PAWN]);
		positional_score -= (-color | 1) * (PIECE_SQUARE[color][promoted_piece][to_nr] - PIECE_SQUARE[color][PAWN][from_nr] + PIECE_SQUARE[color ^ 1][captured_piece][to_nr]);
		zobrist_key ^= ZOBRIST[color][PAWN][from_nr] ^ ZOBRIST[color][promoted_piece][to_nr] ^ ZOBRIST[color ^ 1][captured_piece][to_nr];
	}
	//check if this works
	zobrist_key ^= color == BLACK ? BLACK_TO_MOVE : 0;
}

void Board::init(const Position& position) {
	for (int i = 0; i < COLORS; i++) {
		for (int j = 0; j < PIECES; j++) {
			piece_list[i][j] = position.piece_list[i][j];
			colorBB[i] += piece_list[i][j];
		}
	}
	material_score = evaluation::materialScore(piece_list);
	positional_score = evaluation::positionalScore(piece_list);
	occupiedBB = colorBB[0] | colorBB[1];
	enpassant_square = position.enpassant_square;
	castling_rights = position.castling_rights;

	zobrist_key = zobrist::hash(position);
}

void Board::setCastlingRights(const int color, const int piece, const int flag, const U64 from, const U64 to) {
	if (castling_rights) {
		// only update castling rights after a friendly rook or king move or a capture
		if (piece == ROOK || piece == KING) {
			// check own kingside castling rights when move is a king or rook move
			if (castling_rights & KING_SIDE[color]) {
				if (from & E_SQUARE[color] || from & H_SQUARE[color]) {
					castling_rights ^= KING_SIDE[color];
				}
			}
			// check own queenside castling rights when move is a king or rook move
			if (castling_rights & QUEEN_SIDE[color]) {
				if (from & E_SQUARE[color] || from & A_SQUARE[color]) {
					castling_rights ^= QUEEN_SIDE[color];
				}
			}
		}
		if (flag == CAPTURE) {
			// check enemies kingside castling rights when move is a rook capture
			if (castling_rights & KING_SIDE[color ^ 1]) {
				if (to & H_SQUARE[color ^ 1]) {
					castling_rights ^= KING_SIDE[color ^ 1];
				}
			}
			// check enemies queenside castling rights when move is a rook capture
			if (castling_rights & QUEEN_SIDE[color ^ 1]) {
				if (to & A_SQUARE[color ^ 1]) {
					castling_rights ^= QUEEN_SIDE[color ^ 1];
				}
			}
		}
	}
}

int Board::getScore() const {
	return material_score + positional_score;
}

Board::~Board() = default;


