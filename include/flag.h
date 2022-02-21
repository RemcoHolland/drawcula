#pragma once

constexpr int NO_FLAG = 0;
constexpr int DOUBLE_PUSH = 1 << 16;
constexpr int CAPTURE = 2 << 16;
constexpr int EN_PASSANT = 3 << 16;
constexpr int CASTLING = 4 << 16;