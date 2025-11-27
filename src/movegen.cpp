#include "board.h"
#include "castling.h"
#include "flag.h"
#include "magicmoves.h"
#include "movegen.h"
#include "piece.h"
#include "square.h"
#include "utils.h"
#include <stdexcept>
#include <algorithm>

Movegen::Movegen() = default;

int Movegen::getLegalMove(const int move) const
{
	const int m_from = (move & FROM_MASK) >> 6;
	const int m_to = (move & TO_MASK) >> 12;
	const int m_promotion = (move & PROMOTION_MASK) >> 27;

	for (const int legalMove : moves) {
		const int lm_from = (legalMove & FROM_MASK) >> 6;
		const int lm_to = (legalMove & TO_MASK) >> 12;
		const int lm_promotion = (legalMove & PROMOTION_MASK) >> 27;
		// move is legal if 'from', 'to'  and 'promotion piece' are the same
		if (lm_from == m_from && lm_to == m_to && lm_promotion == m_promotion) {
			return legalMove;
		}
	}
	throw std::invalid_argument("Illegal move specified");
}

void Movegen::sortMoves() {
	// sort moves based on the sort_key value by using a lambda expression
	std::ranges::sort(moves, [](const int& a, const int& b) { return (a & SORT_KEY_MASK) > (b & SORT_KEY_MASK); });
}

void Movegen::generateMoves(const int color, const Board& board) {
	moves.reserve(AVAILABLE_MOVES);
	if (color == WHITE) {
		whitePawnsCaptureLeft(board);
		whitePawnsCaptureRight(board);
		whitePawnsEnpassant(board);
		whitePawnsDoublePush(board);
		whitePawnsPush(board);
		castleWhite(board);
	} else {
		blackPawnsCaptureLeft(board);
		blackPawnsCaptureRight(board);
		blackPawnsEnpassant(board);
		blackPawnsDoublePush(board);
		blackPawnsPush(board);
		castleBlack(board);
	}
	knightMoves(board, color);
	bishopMoves(board, color);
	rookMoves(board, color);
	queenMoves(board, color);
	kingMoves(board, color);
}

void Movegen::whitePawnsPush(const Board& board) {
	const U64 to_squares = board.piece_list[WHITE][PAWN] << 8 & ~board.occupiedBB;
	const U64 from_squares = to_squares >> 8;

	addPawnMoves(from_squares, to_squares, NO_FLAG);
}

void Movegen::whitePawnsDoublePush(const Board& board) {
	const U64 to_squares = ((board.piece_list[WHITE][PAWN] & RANK_2) << 8 & ~board.occupiedBB) << 8 & ~board.occupiedBB;
	const U64 from_squares = to_squares >> 16;

	addPawnMoves(from_squares, to_squares, DOUBLE_PUSH);
}

void Movegen::whitePawnsCaptureLeft(const Board& board) {
	const U64 to_squares = (board.piece_list[WHITE][PAWN] & ~FILE_A) << 7 & board.colorBB[BLACK];
	const U64 from_squares = to_squares >> 7;

	addPawnCaptures(board, WHITE, from_squares, to_squares);
}

void Movegen::whitePawnsCaptureRight(const Board& board) {
	const U64 to_squares = (board.piece_list[WHITE][PAWN] & ~FILE_H) << 9 & board.colorBB[BLACK];
	const U64 from_squares = to_squares >> 9;

	addPawnCaptures(board, WHITE, from_squares, to_squares);
}

void Movegen::whitePawnsEnpassant(const Board& board) {
	if (board.enpassant_square) {
		const U64 from_squares = (board.enpassant_square >> 9 | board.enpassant_square >> 7) & board.piece_list[WHITE][PAWN] & RANK_5;

		addEnPassantMoves(from_squares, board.enpassant_square);
	}
}

void Movegen::blackPawnsPush(const Board& board) {
	const U64 to_squares = board.piece_list[BLACK][PAWN] >> 8 & ~board.occupiedBB;
	const U64 from_squares = to_squares << 8;

	addPawnMoves(from_squares, to_squares, NO_FLAG);
}

void Movegen::blackPawnsDoublePush(const Board& board) {
	const U64 to_squares = ((board.piece_list[BLACK][PAWN] & RANK_7) >> 8 & ~board.occupiedBB) >> 8 & ~board.occupiedBB;
	const U64 from_squares = to_squares << 16;

	addPawnMoves(from_squares, to_squares, DOUBLE_PUSH);
}

void Movegen::blackPawnsCaptureLeft(const Board& board) {
	const U64 to_squares = (board.piece_list[BLACK][PAWN] & ~FILE_A) >> 9 & board.colorBB[WHITE];
	const U64 from_squares = to_squares << 9;

	addPawnCaptures(board, BLACK, from_squares, to_squares);
}

void Movegen::blackPawnsCaptureRight(const Board& board) {
	const U64 to_squares = (board.piece_list[BLACK][PAWN] & ~FILE_H) >> 7 & board.colorBB[WHITE];
	const U64 from_squares = to_squares << 7;

	addPawnCaptures(board, BLACK, from_squares, to_squares);
}

void Movegen::blackPawnsEnpassant(const Board& board) {
	if (board.enpassant_square) {
		const U64 from_squares = (board.enpassant_square << 9 | board.enpassant_square << 7) & board.piece_list[BLACK][PAWN] & RANK_4;

		addEnPassantMoves(from_squares, board.enpassant_square);
	}
}

void Movegen::knightMoves(const Board& board, const int color) {
	U64 knights = board.piece_list[color][KNIGHT];

	while (knights) {
		const int from = Utils::getLS1B(knights);
		const U64 to_squares = KNIGHT_MOVES[from] & ~board.colorBB[color];

		addPieceMoves(board, color, KNIGHT, from, to_squares, board.colorBB[color ^ 1]);
		knights &= knights - 1; // clear LSB
	}
}

void Movegen::bishopMoves(const Board& board, const int color) {
	U64 bishops = board.piece_list[color][BISHOP];

	while (bishops) {
		const int from = Utils::getLS1B(bishops);
		const U64 to_squares = Bmagic(from, board.occupiedBB) & ~board.colorBB[color];

		addPieceMoves(board, color, BISHOP, from, to_squares, board.colorBB[color ^ 1]);

		bishops &= bishops - 1; // clear LSB
	}
}

void Movegen::rookMoves(const Board& board, const int color) {
	U64 rooks = board.piece_list[color][ROOK];

	while (rooks) {
		const int from = Utils::getLS1B(rooks);
		const U64 to_squares = Rmagic(from, board.occupiedBB) & ~board.colorBB[color];

		addPieceMoves(board, color, ROOK, from, to_squares, board.colorBB[color ^ 1]);
		rooks &= rooks - 1; // clear LSB
	}
}

void Movegen::queenMoves(const Board& board, const int color) {
	U64 queens = board.piece_list[color][QUEEN];

	while (queens) {
		const int from = Utils::getLS1B(queens);
		const U64 to_squares = Qmagic(from, board.occupiedBB) & ~board.colorBB[color];

		addPieceMoves(board, color, QUEEN, from, to_squares, board.colorBB[color ^ 1]);
		queens &= queens - 1; // clear LSB
	}
}

void Movegen::kingMoves(const Board& board, const int color) {
	const U64 from = board.piece_list[color][KING];
	const int from_nr = Utils::getLS1B(from);
	const U64 to_squares = KING_MOVES[from_nr] & ~board.colorBB[color];

	addPieceMoves(board, color, KING, from_nr, to_squares, board.colorBB[color ^ 1]);
}

void Movegen::castleWhite(const Board& board) {
	if (board.castling_rights & WHITE_KING_SIDE) {
		if ((~board.occupiedBB & WHITE_KING_SIDE_SQUARES) == WHITE_KING_SIDE_SQUARES) {
			// TODO: delay isAttacked to the search algorithm
			if (!(square::isAttacked(WHITE, board, E1) || square::isAttacked(WHITE, board, F1))) {
				constexpr int move = E1_nr << 6 | G1_nr << 12 | KING << 18 | CASTLING;
				moves.push_back(move);
			}
		}
	}
	if (board.castling_rights & WHITE_QUEEN_SIDE) {
		if ((~board.occupiedBB & WHITE_QUEEN_SIDE_SQUARES) == WHITE_QUEEN_SIDE_SQUARES) {
			if (!(square::isAttacked(WHITE, board, E1) || square::isAttacked(WHITE, board, D1))) {
				constexpr int move = E1_nr << 6 | C1_nr << 12 | KING << 18 | CASTLING;
				moves.push_back(move);
			}
		}
	}
}

void Movegen::castleBlack(const Board& board) {
	if (board.castling_rights & BLACK_KING_SIDE) {
		if ((~board.occupiedBB & BLACK_KING_SIDE_SQUARES) == BLACK_KING_SIDE_SQUARES) {
			// TODO: delay isAttacked to the search algorithm
			if (!(square::isAttacked(BLACK, board, E8) || square::isAttacked(BLACK, board, F8))) {
				constexpr int move = E8_nr << 6 | G8_nr << 12 | KING << 18 | CASTLING;
				moves.push_back(move);
			}
		}
	}
	if (board.castling_rights & BLACK_QUEEN_SIDE) {
		if ((~board.occupiedBB & BLACK_QUEEN_SIDE_SQUARES) == BLACK_QUEEN_SIDE_SQUARES) {
			if (!(square::isAttacked(BLACK, board, E8) || square::isAttacked(BLACK, board, D8))) {
				constexpr int move = E8_nr << 6 | C8_nr << 12 | KING << 18 | CASTLING;
				moves.push_back(move);
			}
		}
	}
}

void Movegen::addPawnMoves(U64 from_squares, U64 to_squares, const int flag) {
	while (from_squares) {
		const int from = Utils::getLS1B(from_squares);
		const int to = Utils::getLS1B(to_squares);

		// check for promotion
		if (to < 8 || to > 55) {
			for (int promotion_piece = QUEEN; promotion_piece >= KNIGHT; promotion_piece--) {
				// create 4 promotion moves
				const int promo_sort_key = MVV_LVA[promotion_piece][PAWN];
				int move = promo_sort_key | from << 6 | to << 12 | PAWN << 18 | PROMOTION | promotion_piece << 27;
				moves.push_back(move);
			}
		} else { // no promotion
			int move = from << 6 | to << 12 | PAWN << 18 | flag;
			moves.push_back(move);
		}
		from_squares &= from_squares - 1; // clear LSB
		to_squares &= to_squares - 1; // clear LSB
	}
}

void Movegen::addPawnCaptures(const Board& board, const int color, U64 from_squares, U64 to_squares) {
	while (from_squares) {
		const int from = Utils::getLS1B(from_squares);
		const int to = Utils::getLS1B(to_squares);

		const int captured_piece = determineCapture(board, color ^ 1, A1 << to);
		const int sort_key = MVV_LVA[captured_piece][PAWN];

		// check for promotion
		if (to < 8 || to > 55) {
			for (int promotion_piece = QUEEN; promotion_piece >= KNIGHT; promotion_piece--) {
				// create 4 promotion moves
				const int promo_sort_key = MVV_LVA[promotion_piece][PAWN]; // increase the value of the sort_key for promotions
				int move = sort_key + promo_sort_key | from << 6 | to << 12 | PAWN << 18 | PROMOCAPT | captured_piece << 24 | promotion_piece << 27;
				moves.push_back(move);
			}
		} else { // no promotion
			int move = sort_key | from << 6 | to << 12 | PAWN << 18 | CAPTURE | captured_piece << 24;
			moves.push_back(move);
		}
		from_squares &= from_squares - 1; // clear LSB
		to_squares &= to_squares - 1; // clear LSB
	}
}

void Movegen::addEnPassantMoves(U64 from_squares, const U64 to_square) {
	while (from_squares) {
		const int from = Utils::getLS1B(from_squares);
		const int to = Utils::getLS1B(to_square);
		constexpr int sort_key = MVV_LVA[PAWN][PAWN];
		int move = sort_key | from << 6 | to << 12 | PAWN << 18 | EN_PASSANT | PAWN << 24;
		moves.push_back(move);
		from_squares &= from_squares - 1; // clear LSB
	}
}

void Movegen::addPieceMoves(const Board& board, const int color, const int piece, const int from, U64 to_squares, const U64 enemies) {
	while (to_squares) {
		const int to = Utils::getLS1B(to_squares);
		const U64 to_square = to_squares & 0 - to_squares; // get LSB

		if (to_square & enemies) {
			const int captured_piece = determineCapture(board, color ^ 1, to_square);
			const int sort_key = MVV_LVA[captured_piece][piece];
			int move = sort_key | from << 6 | to << 12 | piece << 18 | CAPTURE | captured_piece << 24;
			moves.push_back(move);
		} else {
			int move = from << 6 | to << 12 | piece << 18;
			moves.push_back(move);
		}
		to_squares &= to_squares - 1; // clear LSB
	}
}

int Movegen::determineCapture(const Board& board, const int enemy_color, const U64 capture_square) {
	for (int piece = PAWN; piece <= QUEEN; ++piece) {
		if (board.piece_list[enemy_color][piece] & capture_square) {
			return piece;
		}
	}
	throw std::invalid_argument("No captured piece found");
}

Movegen::~Movegen() = default;
