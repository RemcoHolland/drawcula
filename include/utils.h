#pragma once
#include <cstdint>

class Utils {
public:
	Utils();
	static uint64_t getPower(int);
	static uint64_t getLSB(uint64_t);
	static uint64_t clearLSB(uint64_t);
	static uint64_t flipBitVertical(uint64_t);
	static uint64_t flip1BBVertical(uint64_t);
	static int getLS1B(uint64_t);
	~Utils();

private:
	
};

