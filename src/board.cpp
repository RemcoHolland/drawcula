#include "board.h"

Board::Board(FenInfo fenInfo) {
	init(fenInfo);
}

void Board::setPosition(FenInfo fenInfo) {
	init(fenInfo);
}

MoveInfo Board::makeMove(int color, Move move) {
	occupiedBB = occupiedBB - move.from + move.to;
	colorBB[color] = colorBB[color] - move.from + move.to;
	piece_list[move.piece] = piece_list[move.piece] - move.from + move.to;
	int captured_piece = NO_PIECE;

	if (move.flag == Flag::CAPTURE) {
		int piece_color = color * NR_PIECES;
		for (int piece = BLACK_PAWN - piece_color; piece <= BLACK_QUEEN - piece_color; piece++) {
			if ((piece_list[piece] & move.to) != 0) {
				occupiedBB -= move.to;
				colorBB[color ^ 1] -= move.to;
				piece_list[piece] -= move.to;
				captured_piece = piece;
				break;
			}
		}
	} else if (move.flag == Flag::EN_PASSANT) {
		int captured_pawn = BLACK_PAWN - (color * NR_PIECES);
		uint64_t capture_square = color == WHITE ? move.to >> 8 : move.to << 8;
		occupiedBB -= capture_square;
		colorBB[color ^ 1] -= capture_square;
		piece_list[captured_pawn] -= capture_square;
	} else if (move.flag == Flag::CASTLING) {
		uint64_t king_from = move.from;
		uint64_t king_to = move.to;
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
	if (move.promotion != NO_PIECE) {
		piece_list[move.piece] -= move.to;
		piece_list[move.promotion] += move.to;
	}

	setEnPassantSquare(color, move);
	int current_castling_rights = castling_rights;
	setCastlingRights(color, move.from, move.to);

	return MoveInfo(captured_piece, enpassant_square, current_castling_rights);
}

void Board::unmakeMove(int color, Move move, MoveInfo moveInfo) {
	// Unmake can be made faster by setting a pointer to the unmake info. 
	occupiedBB = occupiedBB + move.from - move.to;
	colorBB[color] = colorBB[color] + move.from - move.to;
	piece_list[move.piece] = piece_list[move.piece] + move.from - move.to;
	int piece_color = color * NR_PIECES;

	if (move.flag == Flag::CAPTURE) {
		occupiedBB += move.to;
		colorBB[color ^ 1] += move.to;
		piece_list[moveInfo.getCapturedPiece()] += move.to;
	} else if (move.flag == Flag::EN_PASSANT) {
		int captured_pawn = BLACK_PAWN - (color * NR_PIECES);
		uint64_t capture_square = color == WHITE ? move.to >> 8 : move.to << 8;
		occupiedBB += capture_square;
		colorBB[color ^ 1] += capture_square;
		piece_list[captured_pawn] += capture_square;
	} else if (move.flag == Flag::CASTLING) {
		uint64_t king_from = move.from;
		uint64_t king_to = move.to;
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
	if (move.promotion != NO_PIECE) {
		piece_list[move.piece] += move.to;
		piece_list[move.promotion] -= move.to;
	}

	enpassant_square = moveInfo.getEnpassantSquare();
	castling_rights = moveInfo.getCastlingRights();
}

void Board::init(FenInfo fenInfo) {

	for (int i = 0; i < 12; i++) {
		piece_list[i] = fenInfo.getPieceList()[i];
	}
	piece_list[WHITE_PAWN] = fenInfo.getPieceList()[WHITE_PAWN];

	for (int i = WHITE_PAWN; i <= WHITE_KING; i++) {
		colorBB[0] += piece_list[i];
	}
	for (int i = BLACK_PAWN; i <= BLACK_KING; i++) {
		colorBB[1] += piece_list[i];
	}

	occupiedBB = colorBB[0] | colorBB[1];
	enpassant_square = fenInfo.getEnpassantSquare();
	castling_rights = fenInfo.getCastlingRights();
}

void Board::setEnPassantSquare(int color, Move move) {
	if (move.flag == Flag::DOUBLE_PUSH) {
		enpassant_square = color == WHITE ? move.to >> 8 : move.to << 8;
	} else {
		enpassant_square = 0;
	}
}

void Board::setCastlingRights(int color, uint64_t from, uint64_t to) {
	if (castling_rights != 0) {
		int color_shift = color * 56;

		// check own kingside castling rights when move is a king or rook move
		if (castling_rights & (Castling::KING_SIDE << color)) {
			if (from & (E1 << color_shift) || from & (H1 << color_shift)) {
				castling_rights ^= Castling::KING_SIDE << color;
			}
		}
		// check enemies kingside castling rights when move is a capture
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
		// check enemies queenside castling rights when move is a capture
		if (castling_rights & (Castling::QUEEN_SIDE << (color ^ 1))) {
			if (to & (A8 >> color_shift)) {
				castling_rights ^= Castling::QUEEN_SIDE << (color ^ 1);
			}
		}
	}
}

Board::~Board() {
}


