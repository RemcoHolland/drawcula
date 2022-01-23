#pragma once
#include "color.h"
#include "board.h"
#include "magicmoves.h"
#include "utils.h"
#include "piece.h"

class Square {
public:
	Square();
	static bool isAttacked(int, Board, uint64_t);
	~Square();
};

