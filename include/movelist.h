#pragma once
#include <list>
#include <algorithm>
#include "move.h"
#include "board.h"
#include "piece.h"
#include "color.h"
#include "flag.h"
#include "utils.h"
#include "magicmoves.h"
#include "castling.h"
#include "square.h"

class Movelist {

public:
	Movelist();
	bool containsMove(Move);
	Move getLegalMove(Move);
	void generateMoves(int, Board);
	std::list<Move> getMoves();
	~Movelist();

private:
	std::list<Move> moves;
	void addPawnMoves(int, int, uint64_t, uint64_t, Flag);
	void addEnPassantMoves(int, uint64_t, uint64_t);
	void addPieceMoves(int, uint64_t, uint64_t, uint64_t);
	void whitePawnsPush(Board);
	void whitePawnsDoublePush(Board);
	void whitePawnsCaptureLeft(Board);
	void whitePawnsCaptureRight(Board);
	void whitePawnsEnpassant(Board);
	void blackPawnsPush(Board);
	void blackPawnsDoublePush(Board);
	void blackPawnsCaptureLeft(Board);
	void blackPawnsCaptureRight(Board);
	void blackPawnsEnpassant(Board);
	void whiteKnightMoves(Board);
	void blackKnightMoves(Board);
	void whiteBishopMoves(Board);
	void blackBishopMoves(Board);
	void whiteRookMoves(Board);
	void blackRookMoves(Board);
	void whiteQueenMoves(Board);
	void blackQueenMoves(Board);
	void whiteKingMoves(Board);
	void blackKingMoves(Board);
	void castling(int, Board);
};



