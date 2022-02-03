#include "movelist.h"


Movelist::Movelist() {
}

bool Movelist::containsMove(const Move& move) {
	bool found = (std::find(moves.begin(), moves.end(), move) != moves.end());
	return found;
}

Move Movelist::getLegalMove(const Move& move) {
	for (std::list<Move>::iterator it = moves.begin(); it != moves.end(); ++it) {
		Move legalMove = *it;
		if (legalMove == move) {
			return move;
		}
	}
	throw std::invalid_argument("Illegal move specified");
}

void Movelist::generateMoves(int color, const Board& board) {
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

std::list<Move> Movelist::getMoves() {
	return moves;
}

void Movelist::addPawnMoves(int color, int piece, uint64_t from_squares, uint64_t to_squares, Flag flag) {
	while (from_squares != 0) {
		uint64_t from = Utils::getLSB(from_squares);
		uint64_t to = Utils::getLSB(to_squares);
		// check for promotion. TODO: make one number for this check
		if (to & Board::RANK_8 || to & Board::RANK_1) {
			int color_shift = color * NR_PIECES;
			for (int promotion = WHITE_QUEEN + color_shift; promotion >= WHITE_KNIGHT + color_shift; promotion--) {
				Move move = Move(piece, from, to, flag, promotion);
				moves.push_back(move);
			}
		} else {
			Move move = Move(piece, from, to, flag);
			moves.push_back(move);
		}
		from_squares = Utils::clearLSB(from_squares);
		to_squares = Utils::clearLSB(to_squares);
	}
}

void Movelist::addEnPassantMoves(int piece, uint64_t from_squares, uint64_t to) {
	while (from_squares != 0) {
		uint64_t from = Utils::getLSB(from_squares);
		Move move = Move(piece, from, to, Flag::EN_PASSANT);
		moves.push_back(move);
		from_squares = Utils::clearLSB(from_squares);
	}
}

void Movelist::addPieceMoves(int piece, uint64_t from, uint64_t to_squares, uint64_t enemies) {
	while (to_squares != 0) {
		uint64_t to = Utils::getLSB(to_squares);
		Flag flag = (to & enemies) != 0 ? Flag::CAPTURE : Flag::NO_FLAG;
		Move move = Move(piece, from, to, flag);
		moves.push_back(move);
		to_squares = Utils::clearLSB(to_squares);
	}
}

void Movelist::whitePawnsPush(const Board& board) {
	uint64_t to_squares = (board.piece_list[WHITE_PAWN] << 8) & ~board.occupiedBB;
	uint64_t from_squares = to_squares >> 8;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, Flag::NO_FLAG);
}

void Movelist::whitePawnsDoublePush(const Board& board) {
	uint64_t to_squares = ((((board.piece_list[WHITE_PAWN] & Board::RANK_2) << 8) & ~board.occupiedBB) << 8) & ~board.occupiedBB;
	uint64_t from_squares = to_squares >> 16;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, Flag::DOUBLE_PUSH);
}

void Movelist::whitePawnsCaptureLeft(const Board& board) {
	uint64_t to_squares = ((board.piece_list[WHITE_PAWN] & ~Board::FILE_1) << 7) & board.colorBB[BLACK];
	uint64_t from_squares = to_squares >> 7;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, Flag::CAPTURE);
}

void Movelist::whitePawnsCaptureRight(const Board& board) {
	uint64_t to_squares = ((board.piece_list[WHITE_PAWN] & ~Board::FILE_8) << 9) & board.colorBB[BLACK];
	uint64_t from_squares = to_squares >> 9;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, Flag::CAPTURE);
}

void Movelist::whitePawnsEnpassant(const Board& board) {
	uint64_t enpassant_square = board.enpassant_square;

	if (enpassant_square != 0) {
		uint64_t from_squares = ((enpassant_square >> 9) | (enpassant_square >> 7)) & board.piece_list[WHITE_PAWN] & Board::RANK_5;

		addEnPassantMoves(WHITE_PAWN, from_squares, enpassant_square);
	}
}

void Movelist::blackPawnsPush(const Board& board) {
	uint64_t to_squares = (board.piece_list[BLACK_PAWN] >> 8) & ~board.occupiedBB;
	uint64_t from_squares = to_squares << 8;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, Flag::NO_FLAG);
}

void Movelist::blackPawnsDoublePush(const Board& board) {
	uint64_t to_squares = ((((board.piece_list[BLACK_PAWN] & Board::RANK_7) >> 8) & ~board.occupiedBB) >> 8) & ~board.occupiedBB;
	uint64_t from_squares = to_squares << 16;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, Flag::DOUBLE_PUSH);
}

void Movelist::blackPawnsCaptureLeft(const Board& board) {
	uint64_t to_squares = ((board.piece_list[BLACK_PAWN] & ~Board::FILE_1) >> 9) & board.colorBB[WHITE];
	uint64_t from_squares = to_squares << 9;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, Flag::CAPTURE);
}

void Movelist::blackPawnsCaptureRight(const Board& board) {
	uint64_t to_squares = ((board.piece_list[BLACK_PAWN] & ~Board::FILE_8) >> 7) & board.colorBB[WHITE];
	uint64_t from_squares = to_squares << 7;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, Flag::CAPTURE);
}

void Movelist::blackPawnsEnpassant(const Board& board) {
	uint64_t enpassant_square = board.enpassant_square;

	if (enpassant_square != 0) {
		uint64_t from_squares = ((enpassant_square << 9) | (enpassant_square << 7)) & board.piece_list[BLACK_PAWN] & Board::RANK_4;

		addEnPassantMoves(BLACK_PAWN, from_squares, enpassant_square);
	}
}

void Movelist::whiteKnightMoves(const Board& board) {
	uint64_t knights = board.piece_list[WHITE_KNIGHT];

	while (knights != 0) {
		uint64_t from = Utils::getLSB(knights);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = board.KNIGHT_MOVES[from_nr] & ~board.colorBB[WHITE];

		addPieceMoves(WHITE_KNIGHT, from, to_squares, board.colorBB[BLACK]);
		knights = Utils::clearLSB(knights);
	}
}

void Movelist::blackKnightMoves(const Board& board) {
	uint64_t knights = board.piece_list[BLACK_KNIGHT];

	while (knights != 0) {
		uint64_t from = Utils::getLSB(knights);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = board.KNIGHT_MOVES[from_nr] & ~board.colorBB[BLACK];

		addPieceMoves(BLACK_KNIGHT, from, to_squares, board.colorBB[WHITE]);
		knights = Utils::clearLSB(knights);
	}
}

void Movelist::whiteBishopMoves(const Board& board) {
	uint64_t bishops = board.piece_list[WHITE_BISHOP];

	while (bishops != 0) {
		uint64_t from = Utils::getLSB(bishops);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = Bmagic(from_nr, board.occupiedBB) & ~board.colorBB[WHITE];

		addPieceMoves(WHITE_BISHOP, from, to_squares, board.colorBB[BLACK]);
		bishops = Utils::clearLSB(bishops);
	}
}

void Movelist::blackBishopMoves(const Board& board) {
	uint64_t bishops = board.piece_list[BLACK_BISHOP];

	while (bishops != 0) {
		uint64_t from = Utils::getLSB(bishops);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = Bmagic(from_nr, board.occupiedBB) & ~board.colorBB[BLACK];

		addPieceMoves(BLACK_BISHOP, from, to_squares, board.colorBB[WHITE]);
		bishops = Utils::clearLSB(bishops);
	}
}

void Movelist::whiteRookMoves(const Board& board) {
	uint64_t rooks = board.piece_list[WHITE_ROOK];

	while (rooks != 0) {
		uint64_t from = Utils::getLSB(rooks);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = Rmagic(from_nr, board.occupiedBB) & ~board.colorBB[WHITE];

		addPieceMoves(WHITE_ROOK, from, to_squares, board.colorBB[BLACK]);
		rooks = Utils::clearLSB(rooks);
	}
}

void Movelist::blackRookMoves(const Board& board) {
	uint64_t rooks = board.piece_list[BLACK_ROOK];

	while (rooks != 0) {
		uint64_t from = Utils::getLSB(rooks);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = Rmagic(from_nr, board.occupiedBB) & ~board.colorBB[BLACK];

		addPieceMoves(BLACK_ROOK, from, to_squares, board.colorBB[WHITE]);
		rooks = Utils::clearLSB(rooks);
	}
}

void Movelist::whiteQueenMoves(const Board& board) {
	uint64_t queens = board.piece_list[WHITE_QUEEN];

	while (queens != 0) {
		uint64_t from = Utils::getLSB(queens);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = (Bmagic(from_nr, board.occupiedBB) | Rmagic(from_nr, board.occupiedBB)) & ~board.colorBB[WHITE];

		addPieceMoves(WHITE_QUEEN, from, to_squares, board.colorBB[BLACK]);
		queens = Utils::clearLSB(queens);
	}
}

void Movelist::blackQueenMoves(const Board& board) {
	uint64_t queens = board.piece_list[BLACK_QUEEN];

	while (queens != 0) {
		uint64_t from = Utils::getLSB(queens);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = (Bmagic(from_nr, board.occupiedBB) | Rmagic(from_nr, board.occupiedBB)) & ~board.colorBB[BLACK];

		addPieceMoves(BLACK_QUEEN, from, to_squares, board.colorBB[WHITE]);
		queens = Utils::clearLSB(queens);
	}
}

void Movelist::whiteKingMoves(const Board& board) {
	uint64_t from = board.piece_list[WHITE_KING];
	int from_nr = Utils::getLS1B(from);
	uint64_t to_squares = board.KING_MOVES[from_nr] & ~board.colorBB[WHITE];

	addPieceMoves(WHITE_KING, from, to_squares, board.colorBB[BLACK]);
}

void Movelist::blackKingMoves(const Board& board) {
	uint64_t from = board.piece_list[BLACK_KING];
	int from_nr = Utils::getLS1B(from);
	uint64_t to_squares = board.KING_MOVES[from_nr] & ~board.colorBB[BLACK];

	addPieceMoves(BLACK_KING, from, to_squares, board.colorBB[WHITE]);
}

void Movelist::castling(int color, const Board& board) {
	int castling_rights = board.castling_rights;

	if (castling_rights & (Castling::KING_SIDE << color)) {
		uint64_t short_castle = Board::KING_SIDE_SQUARES << color * 56;
		if ((~board.occupiedBB & short_castle) == short_castle) {
			int king = WHITE_KING + color * NR_PIECES;
			uint64_t king_square = board.piece_list[king];

			// TODO: delay isAttacked to the search algorithm
			if (!(Square::isAttacked(color, board, king_square) || (Square::isAttacked(color, board, king_square << 1)))) {
				Move move = Move(king, king_square, king_square << 2, Flag::CASTLING);
				moves.push_back(move);
			}
		}
	}
	if (castling_rights & (Castling::QUEEN_SIDE << color)) {
		uint64_t long_castle = Board::QUEEN_SIDE_SQUARES << color * 56;
		if ((~board.occupiedBB & long_castle) == long_castle) {
			int king = WHITE_KING + color * NR_PIECES;
			uint64_t king_square = board.piece_list[king];

			if (!(Square::isAttacked(color, board, king_square) || (Square::isAttacked(color, board, king_square >> 1)))) {
				Move move = Move(king, king_square, king_square >> 2, Flag::CASTLING);
				moves.push_back(move);
			}
		}
	}
}

Movelist::~Movelist() {
}
