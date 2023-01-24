#pragma once

constexpr int WHITE_KING_SIDE = 0b1;
constexpr int BLACK_KING_SIDE = 0b10;
constexpr int WHITE_QUEEN_SIDE = 0b100;
constexpr int BLACK_QUEEN_SIDE = 0b1000;

constexpr int KING_SIDE[COLORS] = { WHITE_KING_SIDE, BLACK_KING_SIDE };
constexpr int QUEEN_SIDE[COLORS] = { WHITE_QUEEN_SIDE, BLACK_QUEEN_SIDE };