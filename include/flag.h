#pragma once

constexpr int NO_FLAG = 0b000;
constexpr int DOUBLE_PUSH = 0b001 << 21;
constexpr int CAPTURE = 0b010 << 21;   // Remove capture as a flag? Can be seen when captured piece is filled
constexpr int EN_PASSANT = 0b011 << 21;
constexpr int CASTLING = 0b100 << 21;
constexpr int PROMOTION = 0b101 << 21;
constexpr int PROMOCAPT = 0b110 << 21;