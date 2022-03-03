#pragma once

#include "board.h"

#include <vector>

/****************************
* Move is built up like:    *
* bit 0-5   from_square     *
* bit 6-11  to_square       *
* bit 12-15 piece type      *
* bit 16-18 flag            *
* bit 19-22 captured piece  *
* bit 23-26 promotion piece *
* bit 27-31 MVV/LVA         *
*****************************/

// MVV_LVA[victim][attacker]
constexpr int MVV_LVA[NR_PIECES][NR_PIECES] = {
	{ 6,  5,  4,  3,  2,  1},  // victim pawn,   attacker p, N, B, R, Q, K, None
	{12, 11, 10,  9,  8,  7},  // victim Knight  attacker p, N, B, R, Q, K, None
	{18, 17, 16, 15, 14, 13},  // victim Bishop, attacker p, N, B, R, Q, K, None
	{24, 23, 22, 21, 20, 19},  // victim Rook,   attacker p, N, B, R, Q, K, None
	{30, 29, 28, 27, 26, 25},  // victim Queen,  attacker p, N, B, R, Q, K, None
	{0,   0,  0,  0,  0,  0}   // victim King,   attacker p, N, B, R, Q, K, None
};

// number of moves available in a position
constexpr int AVAILABLE_MOVES = 40;

class Movegen {

public:
	Movegen();
	int getLegalMove(int);
	void generateMoves(int, const Board&);
	std::vector<int> moves;
	~Movegen();

private:
	void addPawnMoves(const Board&, int, int, uint64_t, uint64_t, int);
	void addEnPassantMoves(int, int, uint64_t, int);
	void addPieceMoves(const Board&, int, int, int, uint64_t, uint64_t);
	int determineCapture(const Board&, int, uint64_t);
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



