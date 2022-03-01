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

/**************************
* Move is built up like:
* bit 0-5: from_square
* bit 6-11 to_square
* bit 12-15: piece type
* bit 16-18 flag
* bit 19-22 captured piece
* bit 23-26 promotion piece
**************************/

// MVV_VLA[victim][attacker]
constexpr int MVV_LVA[NR_PIECES + 1][NR_PIECES + 1] = {
	{0,   0,  0,  0,  0,  0, 0},  // victim King,   attacker K, Q, R, B, N, p, None
	{50, 51, 52, 53, 54, 55, 0},  // victim Queen,  attacker K, Q, R, B, N, p, None
	{40, 41, 42, 43, 44, 45, 0},  // victim Rook,   attacker K, Q, R, B, N, p, None
	{30, 31, 32, 33, 34, 35, 0},  // victim Bishop, attacker K, Q, R, B, N, p, None
	{20, 21, 22, 23, 24, 25, 0},  // victim Knight  attacker K, Q, R, B, N, p, None
	{10, 11, 12, 13, 14, 15, 0},  // victim pawn,   attacker K, Q, R, B, N, p, None
	{0,   0,  0,  0,  0,  0, 0}   // victim None,   attacker K, Q, R, B, N, p, None
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



