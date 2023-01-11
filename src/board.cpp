#pragma warning( disable : 6385)
#pragma warning( disable : 6386)
#include "board.h"

Board::Board(const Position& postion) {
	init(postion);
}

void Board::setPosition(const Position& postion) {
	init(postion);
}

const int Board::makeMove(int color, int move) {
	uint64_t from = (uint64_t)1 << ((move & FROM_MASK) >> 4);
	uint64_t to = (uint64_t)1 << ((move & TO_MASK) >> 10);

	int piece = (move & PIECE_MASK) >> 16;
	int flag = move & FLAG_MASK;
	int captured_piece = (move & CAPTURED_PIECE_MASK) >> 23;
	int promotion_piece = (move & PROMOTION_MASK) >> 27;
	uint64_t fromTo = from ^ to;
	colorBB[color] ^= fromTo;
	piece_list[color][piece] ^= fromTo;
	enpassant_square = 0;    // clear previous enpassant square

	if (flag == NO_FLAG) {
		occupiedBB ^= fromTo;
	} else if (flag == DOUBLE_PUSH) {
		occupiedBB ^= fromTo;
		enpassant_square = ((move & TO_MASK) >> 10) - (-color | 1) * 8; // (-color | 1) changes to 1 or -1 when color is either 0 or 1
	} else if (flag == CAPTURE) {
		occupiedBB ^= from;
		colorBB[color ^ 1] ^= to;
		piece_list[color ^ 1][captured_piece] ^= to;
	} else if (flag == EN_PASSANT) {
		uint64_t capture_square = color == WHITE ? to >> 8 : to << 8;
		occupiedBB ^= fromTo | capture_square;
		colorBB[color ^ 1] ^= capture_square;
		piece_list[color ^ 1][captured_piece] ^= capture_square;
	} else if (flag == CASTLING) {
		uint64_t rook_shift = 0;
		if (from < to) { // castling short			
			rook_shift = (to << 1) ^ (from << 1);
		} else { // castling long
			rook_shift = (from >> 1) ^ (to >> 2);
		}
		occupiedBB ^= fromTo | rook_shift;
		colorBB[color] ^= rook_shift;
		piece_list[color][ROOK] ^= rook_shift;
	} else if (flag == PROMOTION) {
		occupiedBB ^= fromTo;
		piece_list[color][piece] ^= to;
		piece_list[color][promotion_piece] ^= to;
	} else if (flag == PROMOCAPT) {
		occupiedBB ^= from;
		colorBB[color ^ 1] ^= to;
		piece_list[color][piece] ^= to;
		piece_list[color ^ 1][captured_piece] ^= to;
		piece_list[color][promotion_piece] ^= to;
	}

	int current_castling_rights = castling_rights;
	setCastlingRights(color, piece, flag, from, to);
	int unmake_info = captured_piece | (enpassant_square << 6) | (current_castling_rights << 12);
	return unmake_info;
}

void Board::unmakeMove(int color, int move, int unmake_info) {
	uint64_t from = (uint64_t)1 << ((move & FROM_MASK) >> 4);
	uint64_t to = (uint64_t)1 << ((move & TO_MASK) >> 10);

	int piece = (move & PIECE_MASK) >> 16;
	int flag = move & FLAG_MASK;
	int promotion_piece = (move & PROMOTION_MASK) >> 27;
	uint64_t fromTo = from ^ to;
	colorBB[color] ^= fromTo;
	piece_list[color][(move & PIECE_MASK) >> 16] ^= fromTo;

	if (flag == NO_FLAG || flag == DOUBLE_PUSH) {
		occupiedBB ^= fromTo;
	} else if (flag == CAPTURE) {
		occupiedBB ^= from;
		colorBB[color ^ 1] ^= to;
		piece_list[color ^ 1][unmake_info & CAPTURE_MASK] ^= to;
	} else if (flag == EN_PASSANT) {
		uint64_t capture_square = color == WHITE ? to >> 8 : to << 8;
		occupiedBB ^= capture_square | fromTo;
		colorBB[color ^ 1] ^= capture_square;
		piece_list[color ^ 1][unmake_info & CAPTURE_MASK] ^= capture_square;
	} else if (flag == CASTLING) {
		uint64_t rook_shift{ 0 };
		if (from < to) { // castling short			
			rook_shift = (to << 1) ^ (from << 1);
		} else { // castling long
			rook_shift = (from >> 1) ^ (to >> 2);
		}
		occupiedBB ^= fromTo | rook_shift;
		colorBB[color] ^= rook_shift;
		piece_list[color][ROOK] ^= rook_shift;
	} else if (flag == PROMOTION) {
		occupiedBB ^= fromTo;
		piece_list[color][piece] ^= to;
		piece_list[color][promotion_piece] ^= to;
	} else if (flag == PROMOCAPT) {
		occupiedBB ^= from;
		colorBB[color ^ 1] ^= to;
		piece_list[color][piece] ^= to;
		piece_list[color ^ 1][unmake_info & CAPTURE_MASK] ^= to;
		piece_list[color][promotion_piece] ^= to;
	}

	enpassant_square = (unmake_info & ENPASSANT_MASK) >> 6;
	castling_rights = (unmake_info & CASTLING_MASK) >> 12;
}

void Board::init(const Position& fenInfo) {
	for (int i = 0; i < COLORS; i++) {
		for (int j = 0; j < PIECES; j++) {
			piece_list[i][j] = fenInfo.piece_list[i][j];
			colorBB[i] += piece_list[i][j];
		}
	}
	occupiedBB = colorBB[0] | colorBB[1];
	enpassant_square = fenInfo.enpassant_square;
	castling_rights = fenInfo.castling_rights;
}

void Board::setCastlingRights(int color, int piece, int flag, uint64_t from, uint64_t to) {
	if (castling_rights) {
		// only update castling rights after a friendly rook or king move or a capture
		if (piece == ROOK || piece == KING || flag == CAPTURE) {

			int color_shift = color * 56;

			// check own kingside castling rights when move is a king or rook move
			if (castling_rights & (Castling::KING_SIDE << color)) {
				if (from & (E1 << color_shift) || from & (H1 << color_shift)) {
					castling_rights ^= Castling::KING_SIDE << color;
				}
			}
			// check enemies kingside castling rights when move is a rook capture
			if (castling_rights & (Castling::KING_SIDE << (color ^ 1))) {
				if (to & (H8 >> color_shift)) {
					castling_rights ^= Castling::KING_SIDE << (color ^ 1);
				}
			}
			// check own queenside castling rights when move is a king or rook move
			if (castling_rights & (Castling::QUEEN_SIDE << color)) {
				if (from & (E1 << color_shift) || from & (A1 << color_shift)) {
					castling_rights ^= Castling::QUEEN_SIDE << color;
				}
			}
			// check enemies queenside castling rights when move is a rook capture
			if (castling_rights & (Castling::QUEEN_SIDE << (color ^ 1))) {
				if (to & (A8 >> color_shift)) {
					castling_rights ^= Castling::QUEEN_SIDE << (color ^ 1);
				}
			}
		}
	}
}

Board::~Board() {
}


