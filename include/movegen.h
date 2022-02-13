#pragma once
#include "board.h"
#include "piece.h"
#include "color.h"
#include "flag.h"
#include "utils.h"
#include "magicmoves.h"
#include "castling.h"
#include "square.h"
#include <stdexcept>
#include <vector>

class Movegen {

public:
	Movegen();
	int getLegalMove(int);
	void generateMoves(int, const Board&);
	std::vector<int> moves;
	~Movegen();

private:	
	void addPawnMoves(int, int, uint64_t, uint64_t, int);
	void addEnPassantMoves(int, uint64_t, int);
	void addPieceMoves(int, int, uint64_t, uint64_t);
	void whitePawnsPush(const Board&);
	void whitePawnsDoublePush(const Board&);
	void whitePawnsCaptureLeft(const Board&);
	void whitePawnsCaptureRight(const Board&);
	void whitePawnsEnpassant(const Board&);
	void blackPawnsPush(const Board&);
	void blackPawnsDoublePush(const Board&);
	void blackPawnsCaptureLeft(const Board&);
	void blackPawnsCaptureRight(const Board&);
	void blackPawnsEnpassant(const Board&);
	void whiteKnightMoves(const Board&);
	void blackKnightMoves(const Board&);
	void whiteBishopMoves(const Board&);
	void blackBishopMoves(const Board&);
	void whiteRookMoves(const Board&);
	void blackRookMoves(const Board&);
	void whiteQueenMoves(const Board&);
	void blackQueenMoves(const Board&);
	void whiteKingMoves(const Board&);
	void blackKingMoves(const Board&);
	void castling(int, const Board&);
};



