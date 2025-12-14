#include <gtest/gtest.h>
#include "evaluation.h"
#include "magicmoves.h"
#include "zobrist.h"

int main(int argc, char **argv) {
	initmagicmoves();
	zobrist::init();
	evaluation::initPieceSquareTable();
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
