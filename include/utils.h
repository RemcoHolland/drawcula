#pragma once
#include <cstdint>

typedef uint64_t U64;

constexpr int index64[64] = {
	0, 47,  1, 56, 48, 27,  2, 60,
	57, 49, 41, 37, 28, 16,  3, 61,
	54, 58, 35, 52, 50, 42, 21, 44,
	38, 32, 29, 23, 17, 11,  4, 62,
	46, 55, 26, 59, 40, 36, 15, 53,
	34, 51, 20, 43, 31, 22, 10, 45,
	25, 39, 14, 33, 19, 30,  9, 24,
	13, 18,  8, 12,  7,  6,  5, 63
};

class Utils {
public:	
	static U64 flipBitVertical(U64);
	static U64 flip1BBVertical(U64);
	static int getLS1B(U64);
private:
	
};

