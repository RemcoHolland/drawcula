#include "utils.h"

U64 Utils::flipBitVertical(const U64 bit) {
	return bit ^ 56;
}

U64 Utils::flip1BBVertical(U64 bb) {
	constexpr U64 k1 = 0x00FF00FF00FF00FF;
	constexpr U64 k2 = 0x0000FFFF0000FFFF;
	bb = bb >> 8 & k1 | (bb & k1) << 8;
	bb = bb >> 16 & k2 | (bb & k2) << 16;
	bb = bb >> 32 | bb << 32;
	return bb;
}

/**
* getLS1B
* @author Kim Walisch (2012)
* @precondition bb != 0
* @return index (0..63) of least significant one bit
*/
int Utils::getLS1B(const U64 bb) {
	constexpr U64 debruijn64 = 0x03f79d71b4cb0a89;
	return index64[((bb ^ bb - 1) * debruijn64) >> 58];
}
