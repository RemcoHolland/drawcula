#include "movelist.h"


Movelist::Movelist() {
}

bool Movelist::containsMove(Move move) {
	bool found = (std::find(moves.begin(), moves.end(), move) != moves.end());
	return found;
}

void Movelist::generateMoves(int color, Board board) {
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
	}
	else {
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
		}
		else {
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

void Movelist::whitePawnsPush(Board board) {
	uint64_t to_squares = (board.getPiece(WHITE_PAWN) << 8) & ~board.getOccupiedBB();
	uint64_t from_squares = to_squares >> 8;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, Flag::NO_FLAG);
}

void Movelist::whitePawnsDoublePush(Board board) {
	uint64_t to_squares = ((((board.getPiece(WHITE_PAWN) & Board::RANK_2) << 8) & ~board.getOccupiedBB()) << 8) & ~board.getOccupiedBB();
	uint64_t from_squares = to_squares >> 16;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, Flag::DOUBLE_PUSH);
}

void Movelist::whitePawnsCaptureLeft(Board board) {
	uint64_t to_squares = ((board.getPiece(WHITE_PAWN) & ~Board::FILE_1) << 7) & board.getColorBB(BLACK);
	uint64_t from_squares = to_squares >> 7;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, Flag::CAPTURE);
}

void Movelist::whitePawnsCaptureRight(Board board) {
	uint64_t to_squares = ((board.getPiece(WHITE_PAWN) & ~Board::FILE_8) << 9) & board.getColorBB(BLACK);
	uint64_t from_squares = to_squares >> 9;

	addPawnMoves(WHITE, WHITE_PAWN, from_squares, to_squares, Flag::CAPTURE);
}

void Movelist::whitePawnsEnpassant(Board board) {
	uint64_t enpassant_square = board.getEnpassantSquare();

	if (enpassant_square != 0) {
		uint64_t from_squares = ((enpassant_square >> 9) | (enpassant_square >> 7)) & board.getPiece(WHITE_PAWN) & Board::RANK_5;

		addEnPassantMoves(WHITE_PAWN, from_squares, enpassant_square);
	}
}

void Movelist::blackPawnsPush(Board board) {
	uint64_t to_squares = (board.getPiece(BLACK_PAWN) >> 8) & ~board.getOccupiedBB();
	uint64_t from_squares = to_squares << 8;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, Flag::NO_FLAG);
}

void Movelist::blackPawnsDoublePush(Board board) {
	uint64_t to_squares = ((((board.getPiece(BLACK_PAWN) & Board::RANK_7) >> 8) & ~board.getOccupiedBB()) >> 8) & ~board.getOccupiedBB();
	uint64_t from_squares = to_squares << 16;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, Flag::DOUBLE_PUSH);
}

void Movelist::blackPawnsCaptureLeft(Board board) {
	uint64_t to_squares = ((board.getPiece(BLACK_PAWN) & ~Board::FILE_1) >> 9) & board.getColorBB(WHITE);
	uint64_t from_squares = to_squares << 9;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, Flag::CAPTURE);
}

void Movelist::blackPawnsCaptureRight(Board board) {
	uint64_t to_squares = ((board.getPiece(BLACK_PAWN) & ~Board::FILE_8) >> 7) & board.getColorBB(WHITE);
	uint64_t from_squares = to_squares << 7;

	addPawnMoves(BLACK, BLACK_PAWN, from_squares, to_squares, Flag::CAPTURE);
}

void Movelist::blackPawnsEnpassant(Board board) {
	uint64_t enpassant_square = board.getEnpassantSquare();

	if (enpassant_square != 0) {
		uint64_t from_squares = ((enpassant_square << 9) | (enpassant_square << 7)) & board.getPiece(BLACK_PAWN) & Board::RANK_4;

		addEnPassantMoves(BLACK_PAWN, from_squares, enpassant_square);
	}
}

void Movelist::whiteKnightMoves(Board board) {
	uint64_t knights = board.getPiece(WHITE_KNIGHT);

	while (knights != 0) {
		uint64_t from = Utils::getLSB(knights);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = board.getKnightMoves(from_nr) & ~board.getColorBB(WHITE);

		addPieceMoves(WHITE_KNIGHT, from, to_squares, board.getColorBB(BLACK));
		knights = Utils::clearLSB(knights);
	}
}

void Movelist::blackKnightMoves(Board board) {
	uint64_t knights = board.getPiece(BLACK_KNIGHT);

	while (knights != 0) {
		uint64_t from = Utils::getLSB(knights);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = board.getKnightMoves(from_nr) & ~board.getColorBB(BLACK);

		addPieceMoves(BLACK_KNIGHT, from, to_squares, board.getColorBB(WHITE));
		knights = Utils::clearLSB(knights);
	}
}

void Movelist::whiteBishopMoves(Board board) {
	uint64_t bishops = board.getPiece(WHITE_BISHOP);

	while (bishops != 0) {
		uint64_t from = Utils::getLSB(bishops);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = Bmagic(from_nr, board.getOccupiedBB()) & ~board.getColorBB(WHITE);

		addPieceMoves(WHITE_BISHOP, from, to_squares, board.getColorBB(BLACK));
		bishops = Utils::clearLSB(bishops);
	}
}

void Movelist::blackBishopMoves(Board board) {
	uint64_t bishops = board.getPiece(BLACK_BISHOP);

	while (bishops != 0) {
		uint64_t from = Utils::getLSB(bishops);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = Bmagic(from_nr, board.getOccupiedBB()) & ~board.getColorBB(BLACK);

		addPieceMoves(BLACK_BISHOP, from, to_squares, board.getColorBB(WHITE));
		bishops = Utils::clearLSB(bishops);
	}
}

void Movelist::whiteRookMoves(Board board) {
	uint64_t rooks = board.getPiece(WHITE_ROOK);

	while (rooks != 0) {
		uint64_t from = Utils::getLSB(rooks);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = Rmagic(from_nr, board.getOccupiedBB()) & ~board.getColorBB(WHITE);

		addPieceMoves(WHITE_ROOK, from, to_squares, board.getColorBB(BLACK));
		rooks = Utils::clearLSB(rooks);
	}
}

void Movelist::blackRookMoves(Board board) {
	uint64_t rooks = board.getPiece(BLACK_ROOK);

	while (rooks != 0) {
		uint64_t from = Utils::getLSB(rooks);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = Rmagic(from_nr, board.getOccupiedBB()) & ~board.getColorBB(BLACK);

		addPieceMoves(BLACK_ROOK, from, to_squares, board.getColorBB(WHITE));
		rooks = Utils::clearLSB(rooks);
	}
}

void Movelist::whiteQueenMoves(Board board) {
	uint64_t queens = board.getPiece(WHITE_QUEEN);

	while (queens != 0) {
		uint64_t from = Utils::getLSB(queens);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = (Bmagic(from_nr, board.getOccupiedBB()) | Rmagic(from_nr, board.getOccupiedBB())) & ~board.getColorBB(WHITE);

		addPieceMoves(WHITE_QUEEN, from, to_squares, board.getColorBB(BLACK));
		queens = Utils::clearLSB(queens);
	}
}

void Movelist::blackQueenMoves(Board board) {
	uint64_t queens = board.getPiece(BLACK_QUEEN);

	while (queens != 0) {
		uint64_t from = Utils::getLSB(queens);
		int from_nr = Utils::getLS1B(from);
		uint64_t to_squares = (Bmagic(from_nr, board.getOccupiedBB()) | Rmagic(from_nr, board.getOccupiedBB())) & ~board.getColorBB(BLACK);

		addPieceMoves(BLACK_QUEEN, from, to_squares, board.getColorBB(WHITE));
		queens = Utils::clearLSB(queens);
	}
}

void Movelist::whiteKingMoves(Board board) {
	uint64_t from = board.getPiece(WHITE_KING);
	int from_nr = Utils::getLS1B(from);
	uint64_t to_squares = board.getKingMoves(from_nr) & ~board.getColorBB(WHITE);

	addPieceMoves(WHITE_KING, from, to_squares, board.getColorBB(BLACK));
}

void Movelist::blackKingMoves(Board board) {
	uint64_t from = board.getPiece(BLACK_KING);
	int from_nr = Utils::getLS1B(from);
	uint64_t to_squares = board.getKingMoves(from_nr) & ~board.getColorBB(BLACK);

	addPieceMoves(BLACK_KING, from, to_squares, board.getColorBB(WHITE));
}

void Movelist::castling(int color, Board board) {
	int castling_rights = board.getCastlingRights();

	if (castling_rights & (Castling::KING_SIDE << color)) {
		uint64_t short_castle = Board::KING_SIDE_SQUARES << color * 56;
		if ((~board.getOccupiedBB() & short_castle) == short_castle) {
			int king = WHITE_KING + color * NR_PIECES;
			uint64_t king_square = board.getPiece(king);

			// TODO: delay isAttacked to the search algorithm
			if (!(Square::isAttacked(color, board, king_square) || (Square::isAttacked(color, board, king_square << 1)))) {
				Move move = Move(king, king_square, king_square << 2, Flag::CASTLING);
				moves.push_back(move);
			}
		}
	}
	if (castling_rights & (Castling::QUEEN_SIDE << color)) {
		uint64_t long_castle = Board::QUEEN_SIDE_SQUARES << color * 56;
		if ((~board.getOccupiedBB() & long_castle) == long_castle) {
			int king = WHITE_KING + color * NR_PIECES;
			uint64_t king_square = board.getPiece(king);

			if (!(Square::isAttacked(color, board, king_square) || (Square::isAttacked(color, board, king_square >> 1)))) {
				Move move = Move(king, king_square, king_square >> 2, Flag::CASTLING);
				moves.push_back(move);
			}
		}
	}
}

Movelist::~Movelist() {
}
