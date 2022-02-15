#pragma once
#include "color.h"
#include "board.h"
#include "magicmoves.h"
#include "utils.h"
#include "piece.h"

namespace square {
	bool isAttacked(int, const Board&, uint64_t);
};

