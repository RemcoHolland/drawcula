#include <gtest/gtest.h>
#include "magicmoves.h"

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	initmagicmoves();
	return RUN_ALL_TESTS();
}