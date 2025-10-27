#include <gtest/gtest.h>
#include "magicmoves.h"

int main(int argc, char **argv) {
	initmagicmoves();
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}