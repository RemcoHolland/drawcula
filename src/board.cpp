#pragma warning( disable : 6385)
#pragma warning( disable : 6386)
#include "board.h"

Board::Board(const Position& fenInfo) {
	init(fenInfo);
}

void Board::setPosition(const Position& fenInfo) {
	init(fenInfo);
}

const int Board::makeMove(int color, int move) {
	uint64_t from = Utils::getPower((move & FROM_MASK) >> 6);
	uint64_t to = Utils::getPower((move & TO_MASK) >> 12);

	occupiedBB = occupiedBB - from + to;
	colorBB[color] = colorBB[color] - from + to;
	piece_list[move & PIECE_MASK] = piece_list[move & PIECE_MASK] - from + to;
	int captured_piece = NO_PIECE;

	if ((move & FLAG_MASK) == CAPTURE) {
		int piece_color = color * NR_PIECES;
		for (int piece = BLACK_PAWN - piece_color; piece <= BLACK_QUEEN - piece_color; piece++) {
			if ((piece_list[piece] & to)) {
				occupiedBB -= to;
				colorBB[color ^ 1] -= to;
				piece_list[piece] -= to;
				captured_piece = piece;
				break;
			}
		}
	} else if ((move & FLAG_MASK) == EN_PASSANT) {
		int captured_pawn = BLACK_PAWN - (color * NR_PIECES);
		uint64_t capture_square = color == WHITE ? to >> 8 : to << 8;
		occupiedBB -= capture_square;
		colorBB[color ^ 1] -= capture_square;
		piece_list[captured_pawn] -= capture_square;
	} else if ((move & FLAG_MASK) == CASTLING) {
		uint64_t king_from = from;
		uint64_t king_to = to;
		if (king_from < king_to) { // castling short			
			uint64_t rook_shift = (king_to << 1) - (king_from << 1); // difference between rook_to (king_to << 1) and rook_from (king_from << 1) position 
			occupiedBB -= rook_shift;
			colorBB[color] -= rook_shift;
			piece_list[WHITE_ROOK + color * NR_PIECES] -= rook_shift;
		} else { // castling long
			uint64_t rook_shift = (king_from >> 1) - (king_to >> 2); // difference between rook_to (king_from >> 1) and rook_from (king_to >> 2) position 
			occupiedBB += rook_shift;
			colorBB[color] += rook_shift;
			piece_list[WHITE_ROOK + color * NR_PIECES] += rook_shift;
		}
	}
	if ((move & PROMOTION_MASK) >> 24 != NO_PROMOTION) {
		piece_list[move & PIECE_MASK] -= to;
		piece_list[(move & PROMOTION_MASK) >> 24] += to;
	}

	setEnPassantSquare(color, move);
	int current_castling_rights = castling_rights;
	setCastlingRights(color, move & PIECE_MASK, move & FLAG_MASK, from, to);
	int unmake_info = captured_piece + (enpassant_square << 6) + (current_castling_rights << 12);
	return unmake_info;
}

void Board::unmakeMove(int color, int move, int unmake_info) {
	uint64_t from = Utils::getPower((move & FROM_MASK) >> 6);
	uint64_t to = Utils::getPower((move & TO_MASK) >> 12);

	occupiedBB = occupiedBB + from - to;
	colorBB[color] = colorBB[color] + from - to;
	piece_list[move & PIECE_MASK] = piece_list[move & PIECE_MASK] + from - to;

	if ((move & FLAG_MASK) == CAPTURE) {
		occupiedBB += to;
		colorBB[color ^ 1] += to;
		piece_list[unmake_info & CAPTURE_MASK] += to;
	} else if ((move & FLAG_MASK) == EN_PASSANT) {
		int captured_pawn = BLACK_PAWN - (color * NR_PIECES);
		uint64_t capture_square = color == WHITE ? to >> 8 : to << 8;
		occupiedBB += capture_square;
		colorBB[color ^ 1] += capture_square;
		piece_list[captured_pawn] += capture_square;
	} else if ((move & FLAG_MASK) == CASTLING) {
		uint64_t king_from = from;
		uint64_t king_to = to;
		if (king_from < king_to) { // castling short			
			uint64_t rook_shift = (king_to << 1) - (king_from << 1); // difference between rook_to (king_to << 1) and rook_from (king_from << 1) position 
			occupiedBB += rook_shift;
			colorBB[color] += rook_shift;
			piece_list[WHITE_ROOK + color * NR_PIECES] += rook_shift;
		} else { // castling long
			uint64_t rook_shift = (king_from >> 1) - (king_to >> 2); // difference between rook_to (king_from >> 1) and rook_from (king_to >> 2) position 
			occupiedBB -= rook_shift;
			colorBB[color] -= rook_shift;
			piece_list[WHITE_ROOK + color * NR_PIECES] -= rook_shift;
		}
	}
	if ((move & PROMOTION_MASK) >> 24 != NO_PROMOTION) {
		piece_list[move & PIECE_MASK] += to;
		piece_list[(move & PROMOTION_MASK) >> 24] -= to;
	}

	enpassant_square = (unmake_info & ENPASSANT_MASK) >> 6;
	castling_rights = (unmake_info & CASTLING_MASK) >> 12;
}

void Board::init(const Position& fenInfo) {
	for (int i = 0; i < TOTAL_PIECES; i++) {
		piece_list[i] = fenInfo.piece_list[i];
	}
	for (int i = WHITE_PAWN; i <= WHITE_KING; i++) {
		colorBB[0] += piece_list[i];
	}
	for (int i = BLACK_PAWN; i <= BLACK_KING; i++) {
		colorBB[1] += piece_list[i];
	}

	occupiedBB = colorBB[0] | colorBB[1];
	enpassant_square = fenInfo.enpassant_square;
	castling_rights = fenInfo.castling_rights;
}

void Board::setEnPassantSquare(int color, int move) {
	if ((move & FLAG_MASK) == DOUBLE_PUSH) {
		enpassant_square = color == WHITE ? ((move & TO_MASK) >> 12) - 8 : ((move & TO_MASK) >> 12) + 8;
	} else {
		enpassant_square = 0;
	}
}

void Board::setCastlingRights(int color, int piece, int flag, uint64_t from, uint64_t to) {
	if (castling_rights) {
		// only update castling rights after a friendly rook or king move or a capture
		if (piece == (WHITE_ROOK + (color * NR_PIECES)) || (piece == WHITE_KING + (color * NR_PIECES)) || (flag == CAPTURE)) {

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


