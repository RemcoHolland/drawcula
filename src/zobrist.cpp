#include <random>
#include "zobrist.h"
#include "utils.h"

U64 ZOBRIST[COLORS][PIECES][SQUARES] = { {{}} };
U64 BLACK_TO_MOVE = 0;

void zobrist::init() {
	// initialise once
	std::random_device rd;
	std::mt19937_64 gen(rd());

	// define random number generator for U64
	std::uniform_int_distribution<U64> dis;

	// init ZOBRIST array
	for (int color = 0; color < COLORS; color++) {
		for (int piece = 0; piece < PIECES; piece++) {
			for (int square = 0; square < SQUARES; square++) {
				ZOBRIST[color][piece][square] = dis(gen);
			}
		}
	}
	BLACK_TO_MOVE = dis(gen);
}

U64 zobrist::hash(const Position& position) {
	U64 key = position.color == WHITE ? 0 : BLACK_TO_MOVE;
	for (int color = 0; color < COLORS; color++) {
		for (int piece = 0; piece < PIECES; piece++) {
			U64 from_squares = position.piece_list[color][piece];
			while (from_squares) {
				const int from = Utils::getLS1B(from_squares);
				key ^= ZOBRIST[color][piece][from];

				from_squares &= from_squares - 1; // clear LSB
			}
		}
	}
	return key;
}
