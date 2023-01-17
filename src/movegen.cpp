#include "board.h"
#include "castling.h"
#include "color.h"
#include "flag.h"
#include "magicmoves.h"
#include "movegen.h"
#include "piece.h"
#include "square.h"
#include "utils.h"

#include <stdexcept>
#include <algorithm>

Movegen::Movegen() {
}

int Movegen::getLegalMove(int move) {
	int m_from = (move & FROM_MASK) >> 6;
	int m_to = (move & TO_MASK) >> 12;
	int m_promotion = (move & PROMOTION_MASK) >> 27;

	for (int legalMove : moves) {
		int lm_from = (legalMove & FROM_MASK) >> 6;
		int lm_to = (legalMove & TO_MASK) >> 12;
		int lm_promotion = (legalMove & PROMOTION_MASK) >> 27;
		// move is legal if 'from', 'to'  and 'promotion piece' are the same
		if (lm_from == m_from && lm_to == m_to && lm_promotion == m_promotion) {
			return legalMove;
		}
	}
	throw std::invalid_argument("Illegal move specified");
}

void Movegen::sortMoves() {
	std::sort(moves.begin(), moves.end(), std::greater<int>());
}

void Movegen::generateMoves(int color, const Board& board) {
	moves.reserve(AVAILABLE_MOVES);
	if (color == WHITE) {
		whitePawnsCaptureLeft(board);
		whitePawnsCaptureRight(board);
		whitePawnsEnpassant(board);
		whiteKnightMoves(board);
		whiteBishopMoves(board);
		whitePawnsDoublePush(board);
		whitePawnsPush(board);
		whiteRookMoves(board);
		whiteQueenMoves(board);
		whiteKingMoves(board);
		castleWhite(board);
	} else {
		blackPawnsCaptureLeft(board);
		blackPawnsCaptureRight(board);
		blackPawnsEnpassant(board);
		blackKnightMoves(board);
		blackBishopMoves(board);
		blackPawnsDoublePush(board);
		blackPawnsPush(board);
		blackRookMoves(board);
		blackQueenMoves(board);
		blackKingMoves(board);
		castleBlack(board);
	}
}

void Movegen::addPawnMoves(U64 from_squares, U64 to_squares, int flag) {
	while (from_squares) {
		int from = Utils::getLS1B(from_squares);
		int to = Utils::getLS1B(to_squares);

		// check for promotion
		if (to <= 7 || to >= 56) {
			for (int promotion_piece = QUEEN; promotion_piece >= KNIGHT; promotion_piece--) {
				// create 4 promotion moves
				int move = from << 6 | (to << 12) | (PAWN << 18) | PROMOTION | (promotion_piece << 27);
				moves.push_back(move);
			}
		} else { // no promotion
			int move = from << 6 | (to << 12) | (PAWN << 18) | flag;
			moves.push_back(move);
		}
		from_squares &= (from_squares - 1); // clear LSB
		to_squares &= (to_squares - 1); // clear LSB
	}
}

void Movegen::addPawnCaptures(const Board& board, int color, U64 from_squares, U64 to_squares) {
	while (from_squares) {
		int from = Utils::getLS1B(from_squares);
		int to = Utils::getLS1B(to_squares);

		int captured_piece = determineCapture(board, color ^ 1, (U64)1 << to);
		int	sort_key = MVV_LVA[captured_piece][PAWN];

		// check for promotion
		if (to <= 7 || to >= 56) {
			for (int promotion_piece = QUEEN; promotion_piece >= KNIGHT; promotion_piece--) {
				// create 4 promotion moves
				int move = sort_key | (from << 6) | (to << 12) | (PAWN << 18) | PROMOCAPT | (captured_piece << 24) | (promotion_piece << 27);
				moves.push_back(move);
			}
		} else { // no promotion
			int move = sort_key | (from << 6) | (to << 12) | (PAWN << 18) | CAPTURE | (captured_piece << 24);
			moves.push_back(move);
		}
		from_squares &= (from_squares - 1); // clear LSB
		to_squares &= (to_squares - 1); // clear LSB
	}
}

void Movegen::addEnPassantMoves(U64 from_squares, int to) {
	while (from_squares) {
		int from = Utils::getLS1B(from_squares);
		int sort_key = MVV_LVA[PAWN][PAWN];
		int move = sort_key | (from << 6) | (to << 12) | (PAWN << 18) | EN_PASSANT | (PAWN << 24);
		moves.push_back(move);
		from_squares &= (from_squares - 1); // clear LSB
	}
}

void Movegen::addPieceMoves(const Board& board, int color, int piece, int from, U64 to_squares, U64 enemies) {
	while (to_squares) {
		int to = Utils::getLS1B(to_squares);
		U64 to_square = to_squares & (0 - to_squares); // get LSB

		int flag = NO_FLAG;
		int captured_piece = NO_PIECE;
		int sort_key = 0b00000;
		if (to_square & enemies) {
			flag = CAPTURE;
			captured_piece = determineCapture(board, color ^ 1, to_square);
			sort_key = MVV_LVA[captured_piece][piece];
		}
		int move = sort_key | (from << 6) | (to << 12) | (piece << 18) | flag | (captured_piece << 24);
		moves.push_back(move);
		to_squares &= (to_squares - 1); // clear LSB
	}
}

int Movegen::determineCapture(const Board& board, int enemy_color, U64 capture_square) {
	for (int piece = PAWN; piece <= QUEEN; ++piece) {
		if (board.piece_list[enemy_color][piece] & capture_square) {
			return piece;
		}
	}
	throw std::invalid_argument("No captured piece found");
}

void Movegen::whitePawnsPush(const Board& board) {
	U64 to_squares = (board.piece_list[WHITE][PAWN] << 8) & ~board.occupiedBB;
	U64 from_squares = to_squares >> 8;

	addPawnMoves(from_squares, to_squares, NO_FLAG);
}

void Movegen::whitePawnsDoublePush(const Board& board) {
	U64 to_squares = ((((board.piece_list[WHITE][PAWN] & RANK_2) << 8) & ~board.occupiedBB) << 8) & ~board.occupiedBB;
	U64 from_squares = to_squares >> 16;

	addPawnMoves(from_squares, to_squares, DOUBLE_PUSH);
}

void Movegen::whitePawnsCaptureLeft(const Board& board) {
	U64 to_squares = ((board.piece_list[WHITE][PAWN] & ~FILE_A) << 7) & board.colorBB[BLACK];
	U64 from_squares = to_squares >> 7;

	addPawnCaptures(board, WHITE, from_squares, to_squares);
}

void Movegen::whitePawnsCaptureRight(const Board& board) {
	U64 to_squares = ((board.piece_list[WHITE][PAWN] & ~FILE_H) << 9) & board.colorBB[BLACK];
	U64 from_squares = to_squares >> 9;

	addPawnCaptures(board, WHITE, from_squares, to_squares);
}

void Movegen::whitePawnsEnpassant(const Board& board) {
	if (board.enpassant_square) {
		U64 from_squares = (((U64)1 << (board.enpassant_square) >> 9) | ((U64)1 << (board.enpassant_square) >> 7)) & board.piece_list[WHITE][PAWN] & RANK_5;

		addEnPassantMoves(from_squares, board.enpassant_square);
	}
}

void Movegen::blackPawnsPush(const Board& board) {
	U64 to_squares = (board.piece_list[BLACK][PAWN] >> 8) & ~board.occupiedBB;
	U64 from_squares = to_squares << 8;

	addPawnMoves(from_squares, to_squares, NO_FLAG);
}

void Movegen::blackPawnsDoublePush(const Board& board) {
	U64 to_squares = ((((board.piece_list[BLACK][PAWN] & RANK_7) >> 8) & ~board.occupiedBB) >> 8) & ~board.occupiedBB;
	U64 from_squares = to_squares << 16;

	addPawnMoves(from_squares, to_squares, DOUBLE_PUSH);
}

void Movegen::blackPawnsCaptureLeft(const Board& board) {
	U64 to_squares = ((board.piece_list[BLACK][PAWN] & ~FILE_A) >> 9) & board.colorBB[WHITE];
	U64 from_squares = to_squares << 9;

	addPawnCaptures(board, BLACK, from_squares, to_squares);
}

void Movegen::blackPawnsCaptureRight(const Board& board) {
	U64 to_squares = ((board.piece_list[BLACK][PAWN] & ~FILE_H) >> 7) & board.colorBB[WHITE];
	U64 from_squares = to_squares << 7;

	addPawnCaptures(board, BLACK, from_squares, to_squares);
}

void Movegen::blackPawnsEnpassant(const Board& board) {
	if (board.enpassant_square) {
		U64 from_squares = (((U64)1 << (board.enpassant_square) << 9) | ((U64)1 << (board.enpassant_square) << 7)) & board.piece_list[BLACK][PAWN] & RANK_4;

		addEnPassantMoves(from_squares, board.enpassant_square);
	}
}

void Movegen::whiteKnightMoves(const Board& board) {
	U64 knights = board.piece_list[WHITE][KNIGHT];

	while (knights) {
		int from = Utils::getLS1B(knights);
		U64 to_squares = KNIGHT_MOVES[from] & ~board.colorBB[WHITE];

		addPieceMoves(board, WHITE, KNIGHT, from, to_squares, board.colorBB[BLACK]);
		knights &= (knights - 1); // clear LSB
	}
}

void Movegen::blackKnightMoves(const Board& board) {
	U64 knights = board.piece_list[BLACK][KNIGHT];

	while (knights) {
		int from = Utils::getLS1B(knights);
		U64 to_squares = KNIGHT_MOVES[from] & ~board.colorBB[BLACK];

		addPieceMoves(board, BLACK, KNIGHT, from, to_squares, board.colorBB[WHITE]);
		knights &= (knights - 1); // clear LSB
	}
}

void Movegen::whiteBishopMoves(const Board& board) {
	U64 bishops = board.piece_list[WHITE][BISHOP];

	while (bishops) {
		int from = Utils::getLS1B(bishops);
		U64 to_squares = Bmagic(from, board.occupiedBB) & ~board.colorBB[WHITE];

		addPieceMoves(board, WHITE, BISHOP, from, to_squares, board.colorBB[BLACK]);

		bishops &= (bishops - 1); // clear LSB
	}
}

void Movegen::blackBishopMoves(const Board& board) {
	U64 bishops = board.piece_list[BLACK][BISHOP];

	while (bishops) {
		int from = Utils::getLS1B(bishops);
		U64 to_squares = Bmagic(from, board.occupiedBB) & ~board.colorBB[BLACK];

		addPieceMoves(board, BLACK, BISHOP, from, to_squares, board.colorBB[WHITE]);
		bishops &= (bishops - 1); // clear LSB
	}
}

void Movegen::whiteRookMoves(const Board& board) {
	U64 rooks = board.piece_list[WHITE][ROOK];

	while (rooks) {
		int from = Utils::getLS1B(rooks);
		U64 to_squares = Rmagic(from, board.occupiedBB) & ~board.colorBB[WHITE];

		addPieceMoves(board, WHITE, ROOK, from, to_squares, board.colorBB[BLACK]);
		rooks &= (rooks - 1); // clear LSB
	}
}

void Movegen::blackRookMoves(const Board& board) {
	U64 rooks = board.piece_list[BLACK][ROOK];

	while (rooks) {
		int from = Utils::getLS1B(rooks);
		U64 to_squares = Rmagic(from, board.occupiedBB) & ~board.colorBB[BLACK];

		addPieceMoves(board, BLACK, ROOK, from, to_squares, board.colorBB[WHITE]);
		rooks &= (rooks - 1); // clear LSB
	}
}

void Movegen::whiteQueenMoves(const Board& board) {
	U64 queens = board.piece_list[WHITE][QUEEN];

	while (queens) {
		int from = Utils::getLS1B(queens);
		U64 to_squares = Qmagic(from, board.occupiedBB) & ~board.colorBB[WHITE];

		addPieceMoves(board, WHITE, QUEEN, from, to_squares, board.colorBB[BLACK]);
		queens &= (queens - 1); // clear LSB
	}
}

void Movegen::blackQueenMoves(const Board& board) {
	U64 queens = board.piece_list[BLACK][QUEEN];

	while (queens) {
		int from = Utils::getLS1B(queens);
		U64 to_squares = Qmagic(from, board.occupiedBB) & ~board.colorBB[BLACK];

		addPieceMoves(board, BLACK, QUEEN, from, to_squares, board.colorBB[WHITE]);
		queens &= (queens - 1); // clear LSB
	}
}

void Movegen::whiteKingMoves(const Board& board) {
	U64 from = board.piece_list[WHITE][KING];
	int from_nr = Utils::getLS1B(from);
	U64 to_squares = KING_MOVES[from_nr] & ~board.colorBB[WHITE];

	addPieceMoves(board, WHITE, KING, from_nr, to_squares, board.colorBB[BLACK]);
}

void Movegen::blackKingMoves(const Board& board) {
	U64 from = board.piece_list[BLACK][KING];
	int from_nr = Utils::getLS1B(from);
	U64 to_squares = KING_MOVES[from_nr] & ~board.colorBB[BLACK];

	addPieceMoves(board, BLACK, KING, from_nr, to_squares, board.colorBB[WHITE]);
}

void Movegen::castleWhite(const Board& board) {
	if (board.castling_rights & WHITE_KING_SIDE) {
		if ((~board.occupiedBB & WHITE_KING_SIDE_SQUARES) == WHITE_KING_SIDE_SQUARES) {
			// TODO: delay isAttacked to the search algorithm
			if (!(square::isAttacked(WHITE, board, E1) || (square::isAttacked(WHITE, board, F1)))) {
				int move = (E1_nr << 6) | (G1_nr << 12) | (KING << 18) | CASTLING;
				moves.push_back(move);
			}
		}
	}
	if (board.castling_rights & WHITE_QUEEN_SIDE) {
		if ((~board.occupiedBB & WHITE_QUEEN_SIDE_SQUARES) == WHITE_QUEEN_SIDE_SQUARES) {
			if (!(square::isAttacked(WHITE, board, E1) || (square::isAttacked(WHITE, board, D1)))) {
				int move = (E1_nr << 6) | (C1_nr << 12) | (KING << 18) | CASTLING;
				moves.push_back(move);
			}
		}
	}
}

void Movegen::castleBlack(const Board& board) {
	if (board.castling_rights & BLACK_KING_SIDE) {
		if ((~board.occupiedBB & BLACK_KING_SIDE_SQUARES) == BLACK_KING_SIDE_SQUARES) {
			// TODO: delay isAttacked to the search algorithm
			if (!(square::isAttacked(BLACK, board, E8) || (square::isAttacked(BLACK, board, F8)))) {
				int move = (E8_nr << 6) | (G8_nr << 12) | (KING << 18) | CASTLING;
				moves.push_back(move);
			}
		}
	}
	if (board.castling_rights & BLACK_QUEEN_SIDE) {
		if ((~board.occupiedBB & BLACK_QUEEN_SIDE_SQUARES) == BLACK_QUEEN_SIDE_SQUARES) {
			if (!(square::isAttacked(BLACK, board, E8) || (square::isAttacked(BLACK, board, D8)))) {
				int move = (E8_nr << 6) | (C8_nr << 12) | (KING << 18) | CASTLING;
				moves.push_back(move);
			}
		}
	}
}

Movegen::~Movegen() {
}
