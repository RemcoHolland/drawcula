#include "movegen.h"

Movegen::Movegen() {
}

int Movegen::getLegalMove(int move) {
	int	m_from = (move & FROM_MASK) >> 6;
	int m_to = (move & TO_MASK) >> 12;
	int m_promotion = (move & PROMOTION_MASK) >> 24;

	for (int legalMove : moves) {
		int lm_from = (legalMove & FROM_MASK) >> 6;
		int lm_to = (legalMove & TO_MASK) >> 12;
		int lm_promotion = (legalMove & PROMOTION_MASK) >> 24;
		// move is legal if 'from', 'to'  and 'promotion piece' are the same
		if (lm_from == m_from && lm_to == m_to && lm_promotion == m_promotion) {
			return legalMove;
		}
	}
	throw std::invalid_argument("Illegal move specified");
}

void Movegen::generateMoves(int color, const Board& board) {
	if (color == WHITE) {
		whitePawnsPush(board);
		whitePawnsDoublePush(board);
		whitePawnsCaptureLeft(board);
		whitePawnsCaptureRight(board);
		whitePawnsEnpassant(board);
		whiteKnightMoves(board);
		whiteBishopMoves(board);
		whiteRookMoves(board);
		whiteQueenMoves(board);
		whiteKingMoves(board);
	} else {
		blackPawnsPush(board);
		blackPawnsDoublePush(board);
		blackPawnsCaptureLeft(board);
		blackPawnsCaptureRight(board);
		blackPawnsEnpassant(board);
		blackKnightMoves(board);
		blackBishopMoves(board);
		blackRookMoves(board);
		blackQueenMoves(board);
		blackKingMoves(board);
	}
	castling(color, board);
}

void Movegen::addPawnMoves(int color, int piece, uint64_t from_squares, uint64_t to_squares, int flag) {
	while (from_squares) {
		int from = Utils::getLS1B(from_squares);
		int to = Utils::getLS1B(to_squares);
		// check for promotion
		if (to <= 7 || to >= 56) {
			int color_shift = color * NR_PIECES;
			for (int promotion = WHITE_QUEEN + color_shift; promotion >= WHITE_KNIGHT + color_shift; promotion--) {
				int move = piece | (from << 6) | (to << 12) | flag + (promotion << 24);
				moves.push_back(move);
			}
		} else {
			int move = piece + (from << 6) + (to << 12) + (flag);
			moves.push_back(move);
		}
		from_squares &= (from_squares - 1); // clear LSB
		to_squares &= (to_squares - 1); // clear LSB
	}
}

void Movegen::addEnPassantMoves(int piece, uint64_t from_squares, int to) {
	while (from_squares) {
		int from = Utils::getLS1B(from_squares);
		int move = piece | (from << 6) | (to << 12) | (EN_PASSANT);
		moves.push_back(move);
		from_squares &= (from_squares - 1); // clear LSB
	}
}

void Movegen::addPieceMoves(int piece, int from, uint64_t to_squares, uint64_t enemies) {
	while (to_squares) {
		int to = Utils::getLS1B(to_squares);
		uint64_t to_square = to_squares & (0 - to_squares); // get LSB
		int flag = (to_square & enemies) ? CAPTURE : NO_FLAG;
		int move = piece | (from << 6) | (to << 12) | (flag);
		moves.push_back(move);
		to_squares &= (to_squares - 1); // clear LSB
	}
}

void Movegen::whitePawnsPush(const Board& board) {
	uint64_t to_squares = (board.piece_list[WHITE_PAWN] << 8) & ~board.occupiedBB;
	uint64_t from_squares = to_squares >> 8;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, NO_FLAG);
}

void Movegen::whitePawnsDoublePush(const Board& board) {
	uint64_t to_squares = ((((board.piece_list[WHITE_PAWN] & RANK_2) << 8) & ~board.occupiedBB) << 8) & ~board.occupiedBB;
	uint64_t from_squares = to_squares >> 16;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, DOUBLE_PUSH);
}

void Movegen::whitePawnsCaptureLeft(const Board& board) {
	uint64_t to_squares = ((board.piece_list[WHITE_PAWN] & ~FILE_A) << 7) & board.colorBB[BLACK];
	uint64_t from_squares = to_squares >> 7;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, CAPTURE);
}

void Movegen::whitePawnsCaptureRight(const Board& board) {
	uint64_t to_squares = ((board.piece_list[WHITE_PAWN] & ~FILE_H) << 9) & board.colorBB[BLACK];
	uint64_t from_squares = to_squares >> 9;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, CAPTURE);
}

void Movegen::whitePawnsEnpassant(const Board& board) {
	if (board.enpassant_square) {
		uint64_t from_squares = ((Utils::getPower(board.enpassant_square) >> 9) | (Utils::getPower(board.enpassant_square) >> 7)) & board.piece_list[WHITE_PAWN] & RANK_5;

		addEnPassantMoves(WHITE_PAWN, from_squares, board.enpassant_square);
	}
}

void Movegen::blackPawnsPush(const Board& board) {
	uint64_t to_squares = (board.piece_list[BLACK_PAWN] >> 8) & ~board.occupiedBB;
	uint64_t from_squares = to_squares << 8;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, NO_FLAG);
}

void Movegen::blackPawnsDoublePush(const Board& board) {
	uint64_t to_squares = ((((board.piece_list[BLACK_PAWN] & RANK_7) >> 8) & ~board.occupiedBB) >> 8) & ~board.occupiedBB;
	uint64_t from_squares = to_squares << 16;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, DOUBLE_PUSH);
}

void Movegen::blackPawnsCaptureLeft(const Board& board) {
	uint64_t to_squares = ((board.piece_list[BLACK_PAWN] & ~FILE_A) >> 9) & board.colorBB[WHITE];
	uint64_t from_squares = to_squares << 9;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, CAPTURE);
}

void Movegen::blackPawnsCaptureRight(const Board& board) {
	uint64_t to_squares = ((board.piece_list[BLACK_PAWN] & ~FILE_H) >> 7) & board.colorBB[WHITE];
	uint64_t from_squares = to_squares << 7;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, CAPTURE);
}

void Movegen::blackPawnsEnpassant(const Board& board) {
	if (board.enpassant_square) {
		uint64_t from_squares = ((Utils::getPower(board.enpassant_square) << 9) | (Utils::getPower(board.enpassant_square) << 7)) & board.piece_list[BLACK_PAWN] & RANK_4;

		addEnPassantMoves(BLACK_PAWN, from_squares, board.enpassant_square);
	}
}

void Movegen::whiteKnightMoves(const Board& board) {
	uint64_t knights = board.piece_list[WHITE_KNIGHT];

	while (knights) {
		int from = Utils::getLS1B(knights);
		uint64_t to_squares = KNIGHT_MOVES[from] & ~board.colorBB[WHITE];

		addPieceMoves(WHITE_KNIGHT, from, to_squares, board.colorBB[BLACK]);
		knights &= (knights - 1); // clear LSB
	}
}

void Movegen::blackKnightMoves(const Board& board) {
	uint64_t knights = board.piece_list[BLACK_KNIGHT];

	while (knights) {
		int from = Utils::getLS1B(knights);
		uint64_t to_squares = KNIGHT_MOVES[from] & ~board.colorBB[BLACK];

		addPieceMoves(BLACK_KNIGHT, from, to_squares, board.colorBB[WHITE]);
		knights &= (knights - 1); // clear LSB
	}
}

void Movegen::whiteBishopMoves(const Board& board) {
	uint64_t bishops = board.piece_list[WHITE_BISHOP];

	while (bishops) {
		int from = Utils::getLS1B(bishops);
		uint64_t to_squares = Bmagic(from, board.occupiedBB) & ~board.colorBB[WHITE];

		addPieceMoves(WHITE_BISHOP, from, to_squares, board.colorBB[BLACK]);

		bishops &= (bishops - 1); // clear LSB
	}
}

void Movegen::blackBishopMoves(const Board& board) {
	uint64_t bishops = board.piece_list[BLACK_BISHOP];

	while (bishops) {
		int from = Utils::getLS1B(bishops);
		uint64_t to_squares = Bmagic(from, board.occupiedBB) & ~board.colorBB[BLACK];

		addPieceMoves(BLACK_BISHOP, from, to_squares, board.colorBB[WHITE]);
		bishops &= (bishops - 1); // clear LSB
	}
}

void Movegen::whiteRookMoves(const Board& board) {
	uint64_t rooks = board.piece_list[WHITE_ROOK];

	while (rooks) {
		int from = Utils::getLS1B(rooks);
		uint64_t to_squares = Rmagic(from, board.occupiedBB) & ~board.colorBB[WHITE];

		addPieceMoves(WHITE_ROOK, from, to_squares, board.colorBB[BLACK]);
		rooks &= (rooks - 1); // clear LSB
	}
}

void Movegen::blackRookMoves(const Board& board) {
	uint64_t rooks = board.piece_list[BLACK_ROOK];

	while (rooks) {
		int from = Utils::getLS1B(rooks);
		uint64_t to_squares = Rmagic(from, board.occupiedBB) & ~board.colorBB[BLACK];

		addPieceMoves(BLACK_ROOK, from, to_squares, board.colorBB[WHITE]);
		rooks &= (rooks - 1); // clear LSB
	}
}

void Movegen::whiteQueenMoves(const Board& board) {
	uint64_t queens = board.piece_list[WHITE_QUEEN];

	while (queens) {
		int from = Utils::getLS1B(queens);
		uint64_t to_squares = (Bmagic(from, board.occupiedBB) | Rmagic(from, board.occupiedBB)) & ~board.colorBB[WHITE];

		addPieceMoves(WHITE_QUEEN, from, to_squares, board.colorBB[BLACK]);
		queens &= (queens - 1); // clear LSB
	}
}

void Movegen::blackQueenMoves(const Board& board) {
	uint64_t queens = board.piece_list[BLACK_QUEEN];

	while (queens) {
		int from = Utils::getLS1B(queens);
		uint64_t to_squares = (Bmagic(from, board.occupiedBB) | Rmagic(from, board.occupiedBB)) & ~board.colorBB[BLACK];

		addPieceMoves(BLACK_QUEEN, from, to_squares, board.colorBB[WHITE]);
		queens &= (queens - 1); // clear LSB
	}
}

void Movegen::whiteKingMoves(const Board& board) {
	uint64_t from = board.piece_list[WHITE_KING];
	int from_nr = Utils::getLS1B(from);
	uint64_t to_squares = KING_MOVES[from_nr] & ~board.colorBB[WHITE];

	addPieceMoves(WHITE_KING, from_nr, to_squares, board.colorBB[BLACK]);
}

void Movegen::blackKingMoves(const Board& board) {
	uint64_t from = board.piece_list[BLACK_KING];
	int from_nr = Utils::getLS1B(from);
	uint64_t to_squares = KING_MOVES[from_nr] & ~board.colorBB[BLACK];

	addPieceMoves(BLACK_KING, from_nr, to_squares, board.colorBB[WHITE]);
}

void Movegen::castling(int color, const Board& board) {
	if (board.castling_rights & (Castling::KING_SIDE << color)) {
		uint64_t short_castle = KING_SIDE_SQUARES << color * 56;
		if ((~board.occupiedBB & short_castle) == short_castle) {
			int king = WHITE_KING + color * NR_PIECES;
			uint64_t king_square = board.piece_list[king];
			int king_square_nr = Utils::getLS1B(king_square);

			// TODO: delay isAttacked to the search algorithm
			if (!(square::isAttacked(color, board, king_square) || (square::isAttacked(color, board, king_square << 1)))) {
				int to = Utils::getLS1B(king_square << 2);
				int move = king + (king_square_nr << 6) + (to << 12) + (CASTLING);
				moves.push_back(move);
			}
		}
	}
	if (board.castling_rights & (Castling::QUEEN_SIDE << color)) {
		uint64_t long_castle = QUEEN_SIDE_SQUARES << color * 56;
		if ((~board.occupiedBB & long_castle) == long_castle) {
			int king = WHITE_KING + color * NR_PIECES;
			uint64_t king_square = board.piece_list[king];
			int king_square_nr = Utils::getLS1B(king_square);

			if (!(square::isAttacked(color, board, king_square) || (square::isAttacked(color, board, king_square >> 1)))) {
				int to = Utils::getLS1B(king_square >> 2);
				int move = king + (king_square_nr << 6) + (to << 12) + (CASTLING);
				moves.push_back(move);
			}
		}
	}
}

Movegen::~Movegen() {
}
