#include "utils.h"

 uint64_t Utils::getPower(int exp) {
	return power[exp];
}

uint64_t Utils::getLSB(uint64_t bb) {
	return bb & (0 - bb);
}

uint64_t Utils::clearLSB(uint64_t bb) {
	return bb & (bb - 1);
}

uint64_t Utils::flipBitVertical(uint64_t bit) {
	return bit ^ 56;
}

uint64_t Utils::flip1BBVertical(uint64_t bb) {
	const uint64_t k1 = uint64_t(0x00FF00FF00FF00FF);
	const uint64_t k2 = uint64_t(0x0000FFFF0000FFFF);
	bb = ((bb >> 8) & k1) | ((bb & k1) << 8);
	bb = ((bb >> 16) & k2) | ((bb & k2) << 16);
	bb = (bb >> 32) | (bb << 32);
	return bb;
}

/**
* getLS1B
* @author Kim Walisch (2012)
* @param squares bitboard to scan
* @precondition bb != 0
* @return index (0..63) of least significant one bit
*/
int Utils::getLS1B(uint64_t bb) {
	const uint64_t debruijn64 = uint64_t(0x03f79d71b4cb0a89);
	return index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
}
