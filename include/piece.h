#pragma once

constexpr int NO_PIECE = 0;
constexpr int NO_PROMOTION = 0;
constexpr int PAWN = 0;
constexpr int KNIGHT = 1;
constexpr int BISHOP = 2;
constexpr int ROOK = 3;
constexpr int QUEEN = 4;
constexpr int KING = 5;

constexpr int PIECES = 6;

constexpr int NR_PAWNS = 16;
constexpr int NR_KNIGHTS = 4;
constexpr int NR_BISHOPS = 4;
constexpr int NR_ROOKS = 4;
constexpr int NR_QUEENS = 2;

namespace Piece {
    char getPiece(int);
    char getPromotion(int);
    int getPromotion(char);
};
